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
    if (expr.type == Type::int_t) // если целое число
    {
        code.push_back("INSTI");
        code.push_back("OUTSTI");
    }
    else if (expr.type == Type::uint_t) // если беззнаковое целое число
    {
        code.push_back("INSTI");
        code.push_back("OUTSTU");
    }
    else if (expr.type == Type::float_t) // если дробное
    {
        code.push_back("INSTF");
        code.push_back("OUTSTF");
    }
    code.push_back("SAVEP temp");
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

    if (!expr.runtime)
    {
        putsCompileTime(expr);
    }
    else
    {
        putsRuntime(expr);
    }

    return defaultResult();
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
    const bool intOp = ((left.type == Type::int_t || left.type == Type::uint_t) && (right.type == left.type));
    const bool floatOp = ((left.type == Type::float_t) && (right.type == left.type));
    switch (op)
    {
    case '+':
    {
        if (intOp)
        {
            auto res = get<SafeInt64>(left.value) + get<SafeInt64>(right.value);
            return Value{false, isSignedOrNot(res), res};
        }
        if (floatOp) return Value{false, left.type, get<float>(left.value) + get<float>(right.value)};
        break;
    }
    case '-':
    {
        if (intOp)
        {
            auto res = get<SafeInt64>(left.value) - get<SafeInt64>(right.value);
            return Value{false, isSignedOrNot(res), res};
        }
        if (floatOp) return Value{false, left.type, get<float>(left.value) - get<float>(right.value)};
        break;
    }
    case '*':
    {
        if (intOp)
        {
            auto res = get<SafeInt64>(left.value) * get<SafeInt64>(right.value);
            return Value{false, isSignedOrNot(res), res};
        }
        if (floatOp) return Value{false, left.type, get<float>(left.value) * get<float>(right.value)};
        break;
    }
    case '/':
    {
        if (intOp)
        {
            auto res = get<SafeInt64>(left.value) / get<SafeInt64>(right.value);
            return Value{false, isSignedOrNot(res), res};
        }
        if (floatOp) return Value{false, left.type, get<float>(left.value) / get<float>(right.value)};
        break;
    }
    case '%':
    {
        if (intOp)
        {
            auto res = get<SafeInt64>(left.value) % get<SafeInt64>(right.value);
            return Value{false, isSignedOrNot(res), res};
        }
        break;
    }
    default:
        throw NotImplementedException("visitArifExpr", NIEError::unknownArifOp);
        break;
    }

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
    return Value{false, Type::bool_t, (ctx->TRUE() != nullptr)};
}

Any CodeGenerator::visitFloatExpr(TParser::FloatExprContext *ctx)
{
    // возвращаем дробное
    return Value{false, Type::float_t, stof(ctx->FLOAT()->toString())};
}

Any CodeGenerator::visitIntExpr(TParser::IntExprContext *ctx)
{
    auto str = ctx->INT()->toString();
    str.erase(remove(str.begin(), str.end(), '_'), str.end());
    int64_t val = stoll(str);

    return Value{false, isSignedOrNot(val), SafeInt<int64_t>(val)};
}

CodeGenerator::Any CodeGenerator::visitIgetsExpr(TParser::IgetsExprContext *)
{
    return Value{true, Type::int_t, {}};
}

CodeGenerator::Any CodeGenerator::visitFgetsExpr(TParser::FgetsExprContext *)
{
    return Value{true, Type::float_t, {}};
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
