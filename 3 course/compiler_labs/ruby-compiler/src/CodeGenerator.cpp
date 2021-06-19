#include "CodeGenerator.h"

#include "antlr4-runtime.h"
#include "TLexer.h"
#include "TParser.h"
#include "CompilerException.h"

using namespace std;
using namespace rubyCompiler;
using namespace antlrcpp;

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

CodeGenerator::Value::Type CodeGenerator::getTypeOfAny(const Any &any) const
{
    if (any.is<int>()) return Value::Type::integer_t;
    if (any.is<float>()) return Value::Type::float_t;
    if (any.is<bool>()) return Value::Type::boolean_t;
    throw logic_error("Not Implemented");
}

string CodeGenerator::getStrOfType(Value::Type type) const
{
    if (type == Value::Type::integer_t) return "Integer";
    if (type == Value::Type::float_t) return "Float";
    if (type == Value::Type::boolean_t) return "Boolean";
    return "Unknown";
}

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

Any CodeGenerator::visitProgramm(TParser::ProgrammContext *ctx)
{
    code.push_back("start:");   // устанавливаем метку старта программы
    return TParserBaseVisitor::visitProgramm(ctx);
}

Any CodeGenerator::visitPuts(TParser::PutsContext *ctx)
{
    // вычисляем expr
    const Any expr = visit(ctx->expr());

    if (expr.is<int>()) // если целое число
    {
        const int val = expr.as<int>();
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
            code.push_back("OUTMEMI val" + to_string(val_i));
        }
    }
    else if (expr.is<float>()) // если дробное
    {
        const string valStr = to_string(expr.as<float>());
        const size_t val_i = tempValues.size();
        tempValues.push_back(valStr);
        code.push_back("OUTMEMF val" + to_string(val_i));
    }
    else if (expr.is<bool>()) // если булевая переменная
    {
        const string valStr = expr.as<bool>()? "1" : "0";
        code.push_back("LOADI " + valStr);
        code.push_back("OUTSTI");
        code.push_back("SAVEP temp");
    }
    else
    {
        throw logic_error("Not Implemented");
    }

    return defaultResult();
}

Any CodeGenerator::visitArifExpr(TParser::ArifExprContext *ctx)
{
    Any left = visit(ctx->expr(0));
    Any right = visit(ctx->expr(1));
    if (getTypeOfAny(left) != getTypeOfAny(right))
    {
        throw TypeError(ctx->start->getLine(),
                        getStrOfType(getTypeOfAny(right)),
                        getStrOfType(getTypeOfAny(left)));
    }

    const char op = ctx->op->getText()[0];
    const bool intOp = (left.is<int>() && right.is<int>());
    const bool floatOp = (left.is<float>() && right.is<float>());
    switch (op)
    {
    case '+':
    {
        if (intOp) return left.as<int>() + right.as<int>();
        if (floatOp) return left.as<float>() + right.as<float>();
        break;
    }
    case '-':
    {
        if (intOp) return left.as<int>() - right.as<int>();
        if (floatOp) return left.as<float>() - right.as<float>();
        break;
    }
    case '*':
    {
        if (intOp) return left.as<int>() * right.as<int>();
        if (floatOp) return left.as<float>() * right.as<float>();
        break;
    }
    case '/':
    {
        if (intOp) return left.as<int>() / right.as<int>();
        if (floatOp) return left.as<float>() / right.as<float>();
        break;
    }
    case '%':
    {
        if (intOp) return left.as<int>() % right.as<int>();
        break;
    }
    default:
        throw logic_error("Not Implemented");
        break;
    }

    throw NoMethodError(ctx->start->getLine(),
                        ctx->op->getText(),
                        getStrOfType(getTypeOfAny(left))
                        );
}

Any CodeGenerator::visitIntExpr(TParser::IntExprContext *ctx)
{
    // возвращаем целое
    return Value{false, Value::Type::integer_t, stoi(ctx->INT()->toString())};
}

Any CodeGenerator::visitFloatExpr(TParser::FloatExprContext *ctx)
{
    // возвращаем дробное
    return Value{false, Value::Type::float_t, stof(ctx->FLOAT()->toString())};
}

Any CodeGenerator::visitBoolExpr(TParser::BoolExprContext *ctx)
{
    // возвращаем булеву
    return Value{false, Value::Type::boolean_t, (ctx->TRUE() != nullptr)};
}

CodeGenerator::Any CodeGenerator::visitIgetsExpr(TParser::IgetsExprContext *ctx)
{
    throw logic_error("Not Implemented igets");
}

Any CodeGenerator::visitBracketsExpr(TParser::BracketsExprContext *ctx)
{
    return visit(ctx->expr()); // возвращаем expr внутри скобок
}

Any CodeGenerator::visitIdExpr(TParser::IdExprContext *ctx)
{
    const string ID = ctx->ID()->toString();

    if (varTable.find(ID) != varTable.end())
        return varTable[ID].value;

    throw NameError(ctx->start->getLine(),
                    ctx->ID()->toString()
                    );
}

CodeGenerator::Any CodeGenerator::visitAssignment(TParser::AssignmentContext *ctx)
{
    // вычисляем expr
    const Any expr = visit(ctx->expr());
    const string ID = ctx->ID()->toString();
    varTable[ID] = Value{getTypeOfAny(expr), expr};
    return expr;
}

Any rubyCompiler::CodeGenerator::visitIf_statement(TParser::If_statementContext *ctx)
{
    // вычисляем expr
    const Any expr = visit(ctx->expr());
    auto exprType = getTypeOfAny(expr);
    if (exprType == Value::Type::boolean_t)
    {
        if (expr.as<bool>())
        {
            visit(ctx->statement_list(0));
        }
        else if (!expr.as<bool>() && ctx->statement_list(1))
        {
            visit(ctx->statement_list(1));
        }
    }
    else
    {
        throw TypeError(ctx->start->getLine(),
                        getStrOfType(exprType),
                        getStrOfType(Value::Type::boolean_t)
                        );
    }
    return defaultResult();
}
