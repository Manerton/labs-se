#include "CodeGenerator.h"

#include "antlr4-runtime.h"
#include "TLexer.h"
#include "TParser.h"
#include "../thirdparty/SafeInt.hpp"
#include "CompilerException.h"

using namespace std;
using namespace rubyCompiler;
using namespace antlrcpp;
using NIEError = NotImplementedException::error;
using SafeInt64 = SafeInt<int64_t>;

bool CodeGenerator::isInteger(const string& str) const
{
    if (str.empty()) return false;

    char * p = nullptr;
    const int radix = 10;
    strtol(str.c_str(), &p, radix);

    return (*p == 0);
}

bool CodeGenerator::isFloat(const string& str) const
{
    if (str.empty()) return false;

    char * p = nullptr;
    strtof(str.c_str(), &p);

    return (*p == 0);
}

void CodeGenerator::generateTempValues()
{
    // итератор
    auto it = code.begin();

    // переменная, в которую можно сохранять ненужные значения из стека
    code.insert(it, "temp: int 0");

    const size_t tempValuesSize = tempValues.size();
    for (size_t i = 0; i < tempValuesSize; ++i)
    {
        const string type = isInteger(tempValues[i]) ? "int" : "float";
        // записываем в начале кода все служебные дополнительные переменные
        code.insert(it, "val" + to_string(i) + ": " + type + " " + tempValues[i]);
    }
}

string CodeGenerator::getStrOfType(Type type) const
{
    if (type == Type::uint_t) return "Integer";
    if (type == Type::int_t) return "Integer";
    if (type == Type::float_t) return "Float";
    if (type == Type::bool_t) return "Boolean";
    return "Unknown";
}

void rubyCompiler::CodeGenerator::putsCompileTime(const Value &expr)
{
    if (expr.type == Type::int_t || expr.type == Type::uint_t) // если целое число
    {
        const int64_t val = get<SafeInt64>(expr.value);
        const string valStr = to_string(val);
        // если число в диапазоне int16_t
        if (val >= numeric_limits<int16_t>::min() && val <= numeric_limits<int16_t>::max())
        {
            code.push_back("LOADI " + valStr);
            code.push_back("OUTSTI");
            code.push_back("SAVEP temp");
        }
        // если число в диапазоне uint16_t
        else if (val > 0 && val <= numeric_limits<uint16_t>::max())
        {
            code.push_back("LOADU " + valStr);
            code.push_back("OUTSTU");
            code.push_back("SAVEP temp");
        }
        // если число не влезает в 16 бит
        else
        {
            const size_t val_i = tempValues.size();
            tempValues.push_back(valStr);
            if (isSignedOrNot(val) == Type::int_t)
                code.push_back("OUTMEMI val" + to_string(val_i));

            else if (isSignedOrNot(val) == Type::uint_t)
                code.push_back("OUTMEMU val" + to_string(val_i));
        }
    }
    else if (expr.type == Type::float_t) // если дробное
    {
        const string valStr = to_string(get<float>(expr.value));
        const size_t val_i = tempValues.size();
        tempValues.push_back(valStr);
        code.push_back("OUTMEMF val" + to_string(val_i));
    }
    else if (expr.type == Type::bool_t) // если булевая переменная
    {
        const string valStr = get<bool>(expr.value)? "1" : "0";
        code.push_back("LOADI " + valStr);
        code.push_back("OUTSTI");
        code.push_back("SAVEP temp");
    }
}

void CodeGenerator::putsRuntime(const Value &expr)
{
    const auto& name = get<internalVarName>(expr.value);
    if (expr.type == Type::int_t) // если целое число
    {
        if (name == "stack") code.push_back("OUTSTI");
        else code.push_back("OUTMEMI " + name);
    }
    else if (expr.type == Type::uint_t) // если беззнаковое целое число
    {
        if (name == "stack") code.push_back("OUTSTU");
        else code.push_back("OUTMEMU " + name);
    }
    else if (expr.type == Type::float_t) // если дробное
    {
        if (name == "stack") code.push_back("OUTSTF");
        else code.push_back("OUTMEMF " + name);
    }
}

CodeGenerator::Type CodeGenerator::isSignedOrNot(int64_t val)
{
    if (val >= numeric_limits<int32_t>::min() && val <= numeric_limits<int32_t>::max())
        return Type::int_t;

    if (val >= numeric_limits<uint32_t>::min() && val <= numeric_limits<uint32_t>::max())
        return Type::uint_t;

    throw out_of_range("cannot fit " + to_string(val) + " into Integer type");
}

const CodeGenerator::SourceCode& CodeGenerator::getCode() const
{
    return code;
}

void CodeGenerator::generateCode(antlr4::tree::ParseTree *tree)
{
    visit(tree);
    // после того, как пройдемся по всему дереву и сгенерируем код
    // поместим в начало кода - код для объявления служебных переменных
    generateTempValues();
    // последней строкой сгенерированного ASM кода будет эта строчка
    code.push_back("end start");
}

Any CodeGenerator::visitProgramm(TParser::ProgrammContext *ctx)
{
    code.push_back("start:");   // устанавливаем метку старта программы
    return TParserBaseVisitor::visitProgramm(ctx);
}

Any CodeGenerator::visitPuts(TParser::PutsContext *ctx)
{
    const Any anyExpr = visit(ctx->expr());
    if (!anyExpr.is<Value>())
        throw NotImplementedException("visitPuts", NIEError::exprIsNotValue);

    // вычисляем expr
    const Value expr = anyExpr;

    if (!holds_alternative<internalVarName>(expr.value))
    {
        putsCompileTime(expr);
    }
    else
    {
        putsRuntime(expr);
    }

    return defaultResult();
}

CodeGenerator::Value CodeGenerator::arifExprCompileTime(const char op, Value left, Value right)
{
    const bool intOp = ((left.type == Type::int_t || left.type == Type::uint_t) && (right.type == left.type));
    const bool floatOp = ((left.type == Type::float_t) && (right.type == left.type));

    switch (op)
    {
    case '+':
    {
        if (intOp)
        {
            auto res = get<SafeInt64>(left.value) + get<SafeInt64>(right.value);
            return Value{isSignedOrNot(res), res};
        }
        if (floatOp) return Value{left.type, get<float>(left.value) + get<float>(right.value)};
        break;
    }
    case '-':
    {
        if (intOp)
        {
            auto res = get<SafeInt64>(left.value) - get<SafeInt64>(right.value);
            return Value{isSignedOrNot(res), res};
        }
        if (floatOp) return Value{left.type, get<float>(left.value) - get<float>(right.value)};
        break;
    }
    case '*':
    {
        if (intOp)
        {
            auto res = get<SafeInt64>(left.value) * get<SafeInt64>(right.value);
            return Value{isSignedOrNot(res), res};
        }
        if (floatOp) return Value{left.type, get<float>(left.value) * get<float>(right.value)};
        break;
    }
    case '/':
    {
        if (intOp)
        {
            auto res = get<SafeInt64>(left.value) / get<SafeInt64>(right.value);
            return Value{isSignedOrNot(res), res};
        }
        if (floatOp) return Value{left.type, get<float>(left.value) / get<float>(right.value)};
        break;
    }
    default: // и выбросится исключение ниже
        break;
    }
    throw NotImplementedException("visitArifExpr", NIEError::unknownArifOp);
}

CodeGenerator::Value CodeGenerator::createRuntimeVar(const Value &val)
{
    const size_t val_i = tempValues.size();

    if (val.type == Type::int_t || val.type == Type::uint_t)
    {
        tempValues.push_back(to_string(int64_t(get<SafeInt64>(val.value))));
    }
    else if (val.type == Type::float_t)
    {
        tempValues.push_back(to_string(get<float>(val.value)));
    }

    internalVarName name = "val" + to_string(val_i);
    return Value{val.type, name};
}

CodeGenerator::Value CodeGenerator::arifExprRuntime(const char op, Value left, Value right)
{
    const bool intOp = ((left.type == Type::int_t || left.type == Type::uint_t)
                        && (right.type == left.type));
    const bool floatOp = ((left.type == Type::float_t) && (right.type == left.type));

    if (!holds_alternative<internalVarName>(left.value))
        left = createRuntimeVar(left);
    if (!holds_alternative<internalVarName>(right.value))
        right = createRuntimeVar(right);
    if (get<internalVarName>(left.value) != "stack")
    {
        code.push_back("LOAD " + get<internalVarName>(left.value));
    }
    if (get<internalVarName>(right.value) == "stack")
    {
        code.push_back("SAVEP temp");
        right.value = internalVarName("temp");
    }

    const internalVarName &rightVarName = get<internalVarName>(right.value);

    switch (op)
    {
    case '+':
    {
        if (intOp) code.push_back("IADD " + rightVarName);
        if (floatOp) code.push_back("FADD " + rightVarName);
        break;
    }
    case '-':
    {
        if (intOp) code.push_back("ISUB " + rightVarName);
        if (floatOp) code.push_back("FSUB " + rightVarName);
        break;
    }
    case '*':
    {
        if (intOp) code.push_back("IMUL " + rightVarName);
        if (floatOp) code.push_back("FMUL " + rightVarName);
        break;
    }
    case '/':
    {
        if (intOp) code.push_back("IDIV " + rightVarName);
        if (floatOp) code.push_back("FDIV " + rightVarName);
        break;
    }
    default:
        throw NotImplementedException("arifExprRuntime", NIEError::unknownArifOp);
        break;
    }
    return Value{left.type, internalVarName("stack")};
}

Any CodeGenerator::visitArifExpr(TParser::ArifExprContext *ctx)
{
    const Any anyLeft = visit(ctx->expr(0));
    const Any anyRight = visit(ctx->expr(1));

    if (!anyLeft.is<Value>() || !anyRight.is<Value>())
        throw NotImplementedException("visitArifExpr", NIEError::exprIsNotValue);

    Value left = anyLeft;
    Value right = anyRight;
    if (left.type != right.type)
    {
        throw TypeError(ctx->start->getLine(),
                        getStrOfType(left.type),
                        getStrOfType(right.type)
                        );
    }

    const char op = ctx->op->getText()[0];

    if (!holds_alternative<internalVarName>(left.value)
            && !holds_alternative<internalVarName>(right.value))
    {
        return arifExprCompileTime(op,left,right);
    }
    return arifExprRuntime(op,left,right);

    throw NoMethodError(ctx->start->getLine(),
                        ctx->op->getText(),
                        getStrOfType(left.type)
                        );
}

Any CodeGenerator::visitIdExpr(TParser::IdExprContext *ctx)
{
    const string ID = ctx->ID()->toString();

    if (varTable.find(ID) != varTable.end())
        return varTable[ID];

    throw NameError(ctx->start->getLine(),
                    ctx->ID()->toString()
                    );
}

Any CodeGenerator::visitBoolExpr(TParser::BoolExprContext *ctx)
{
    // возвращаем булеву
    return Value{Type::bool_t, (ctx->TRUE() != nullptr)};
}

Any CodeGenerator::visitFloatExpr(TParser::FloatExprContext *ctx)
{
    // возвращаем дробное
    return Value{Type::float_t, stof(ctx->FLOAT()->toString())};
}

Any CodeGenerator::visitIntExpr(TParser::IntExprContext *ctx)
{
    auto str = ctx->INT()->toString();
    str.erase(remove(str.begin(), str.end(), '_'), str.end());
    int64_t val = stoll(str);

    return Value{isSignedOrNot(val), SafeInt<int64_t>(val)};
}

CodeGenerator::Any CodeGenerator::visitIgetsExpr(TParser::IgetsExprContext *)
{
    const size_t val_i = tempValues.size();
    tempValues.emplace_back("0");
    internalVarName name = "val" + to_string(val_i);
    code.push_back("INMEMI " + name);
    return Value{Type::int_t, name};
}

CodeGenerator::Any CodeGenerator::visitUgetsExpr(TParser::UgetsExprContext *)
{
    const size_t val_i = tempValues.size();
    tempValues.emplace_back("0");
    internalVarName name = "val" + to_string(val_i);
    code.push_back("INMEMI " + name);
    return Value{Type::uint_t, name};
}

CodeGenerator::Any CodeGenerator::visitFgetsExpr(TParser::FgetsExprContext *)
{
    const size_t val_i = tempValues.size();
    tempValues.emplace_back("0.0");
    internalVarName name = "val" + to_string(val_i);
    code.push_back("INMEMF " + name);
    return Value{Type::float_t, name};
}

CodeGenerator::Any CodeGenerator::visitAssignment(TParser::AssignmentContext *ctx)
{
    // вычисляем expr
    const Any anyExpr = visit(ctx->expr());
    if (!anyExpr.is<Value>())
        throw NotImplementedException("visitAssignment", NIEError::exprIsNotValue);

    const Value expr = anyExpr;
    const string ID = ctx->ID()->toString();
    return varTable[ID] = expr;
}

CodeGenerator::Any CodeGenerator::visitUnaryMinusExpr(TParser::UnaryMinusExprContext *ctx)
{
    // вычисляем expr
    const Any anyExpr = visit(ctx->expr());
    if (!anyExpr.is<Value>())
        throw NotImplementedException("visitUnaryMinusExpr", NIEError::exprIsNotValue);

    Value expr = anyExpr;
    if (expr.type == Type::int_t || expr.type == Type::uint_t)
    {
        expr.value = SafeInt64(-get<SafeInt64>(expr.value));
        expr.type = isSignedOrNot(get<SafeInt64>(expr.value));
    }
    else if (expr.type == Type::float_t)
    {
        expr.value = float(-get<float>(expr.value));
    }
    else
    {
        throw NoMethodError(ctx->start->getLine(),
                            "unary -",
                            getStrOfType(expr.type));
    }

    return expr;
}

Any CodeGenerator::visitBracketsExpr(TParser::BracketsExprContext *ctx)
{
    return visit(ctx->expr()); // возвращаем expr внутри скобок
}

Any rubyCompiler::CodeGenerator::visitIf_statement(TParser::If_statementContext *ctx)
{
    // вычисляем expr
    const Any anyExpr = visit(ctx->expr());
    if (!anyExpr.is<Value>())
        throw NotImplementedException("visitIf_statement", NIEError::exprIsNotValue);

    const Value expr = anyExpr;
    if (expr.type == Type::bool_t)
    {
        if (get<bool>(expr.value))
        {
            visit(ctx->statement_list(0));
        }
        else if (!get<bool>(expr.value) && ctx->statement_list(1))
        {
            visit(ctx->statement_list(1));
        }
    }
    else
    {
        throw TypeError(ctx->start->getLine(),
                        getStrOfType(expr.type),
                        getStrOfType(Type::bool_t)
                        );
    }
    return defaultResult();
}
