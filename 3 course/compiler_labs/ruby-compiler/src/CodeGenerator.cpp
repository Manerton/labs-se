#include "CodeGenerator.h"

#include "antlr4-runtime.h"
#include "TLexer.h"
#include "TParser.h"
#include "CodeGenerator.h"

using namespace rubyCompiler;
using namespace antlr4;
using namespace std;

const CodeGenerator::SourceCode& CodeGenerator::getCode() const
{
    return code;
}

void CodeGenerator::generateCode(tree::ParseTree *tree)
{
    visit(tree);
    // после того, как пройдемся по всему дереву и сгенерируеем код
    // поместим в начало кода объявления служебных переменных
    generateTempValues();
    // последней строкой сгенерированного ASM кода будет эта строчка
    code.push_back("end start");
}

bool CodeGenerator::isInteger(const string& str) const
{
    if (str.empty()) return false;

    char * p = nullptr;
    strtol(str.c_str(), &p, 10);

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

antlrcpp::Any CodeGenerator::visitProgramm(TParser::ProgrammContext *ctx)
{
    code.push_back("start:");        // устанавливаем метку старта программы
    return TParserBaseVisitor::visitProgramm(ctx);
}

antlrcpp::Any CodeGenerator::visitPuts(TParser::PutsContext *ctx)
{
    antlrcpp::Any expr = TParserBaseVisitor::visit(ctx->expr());

    if (expr.is<string>()) // если строка
    {
        //code.push_back("puts_string " + expr->LITERAL()->toString());
    }
    else if (expr.is<int>()) // если целое число
    {
        const int val = expr.as<int>();
        const string valStr = to_string(val);
        if (val >= numeric_limits<int16_t>::min() && val <= numeric_limits<int16_t>::max())
        {
            code.push_back("LOADI " + valStr);
            code.push_back("OUTSTI");
            code.push_back("SAVEP temp");
        }
        else if (val > 0 && val <= numeric_limits<uint16_t>::max())
        {
            code.push_back("LOADU " + valStr);
            code.push_back("OUTSTU");
            code.push_back("SAVEP temp");
        }
        else
        {
            const size_t val_i = tempValues.size();
            tempValues.push_back(valStr);
            code.push_back("OUTMEMI val" + to_string(val_i));
        }
    }
    else if (expr.is<float>())
    {
        const size_t val_i = tempValues.size();
        tempValues.push_back(to_string(expr.as<float>()));
        code.push_back("OUTMEMF val" + to_string(val_i));
    }

    return TParserBaseVisitor::visitPuts(ctx);
}

antlrcpp::Any CodeGenerator::visitArifExpr(rubyCompiler::TParser::ArifExprContext *ctx)
{
    antlrcpp::Any left = visit(ctx->expr(0));
    antlrcpp::Any right = visit(ctx->expr(1));
    const bool intOp = (left.is<int>() && right.is<int>());
    const bool floatOp = (left.is<float>() && right.is<float>());

    if (!intOp && !floatOp)
        throw std::logic_error("line " + to_string(ctx->start->getLine()) + " - type error in: " + ctx->getText());

    const char op = ctx->op->getText()[0];
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
            throw std::logic_error("Unknown arif oper");
    }

}

antlrcpp::Any CodeGenerator::visitIntExpr(rubyCompiler::TParser::IntExprContext *ctx)
{
    return stoi(ctx->INT()->toString());
}

antlrcpp::Any CodeGenerator::visitFloatExpr(rubyCompiler::TParser::FloatExprContext *ctx)
{
    return stof(ctx->FLOAT()->toString());
}

antlrcpp::Any CodeGenerator::visitLiteralExpr(rubyCompiler::TParser::LiteralExprContext *ctx)
{
    return ctx->LITERAL();
}

antlrcpp::Any CodeGenerator::visitBracketsExpr(rubyCompiler::TParser::BracketsExprContext *ctx)
{
    return visit(ctx->expr());
}

antlrcpp::Any CodeGenerator::visitIdExpr(rubyCompiler::TParser::IdExprContext *ctx)
{
    if (ctx->ID()->toString() == "testVar")
        return 55;

    throw std::logic_error("Unknown variable");
}
