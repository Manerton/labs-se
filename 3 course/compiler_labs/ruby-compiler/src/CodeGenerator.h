#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include <list>
#include <string>
#include <vector>
#include <variant>
#include "TParserBaseVisitor.h"

using std::string;

namespace rubyCompiler
{
class CodeGenerator : public TParserBaseVisitor
{
    using Any = antlrcpp::Any;
    using CodeLine = string;
    using SourceCode = std::list<CodeLine>;

    struct Value
    {
        enum class Type : uint8_t
        {
            integer_t,    // целое
            float_t,      // дробное
            boolean_t     // булева
        };

        bool runtime = false; // значение известно на этапе компиляции
                              // или на рантайме?

        Type type;            // тип

        std::variant<std::monostate, int, float, bool> value;  // значение
    };

    // поля
    SourceCode code;                    // сгенерированный код ассемблера
    std::vector<string> tempValues;     // метки со служебными значениями в памяти ВМ
    std::map<string, Value> varTable;   // таблица переменных

    // методы
    void generateTempValues();
    bool isInteger(const string &str) const;
    bool isFloat(const string &str) const;

public:
    const SourceCode& getCode() const;

    void generateCode(antlr4::tree::ParseTree *tree);
    Value::Type getTypeOfAny(const Any &any) const;
    string getStrOfType(Value::Type type) const;
    // обход дерева
    virtual Any visitProgramm(TParser::ProgrammContext *ctx) override;
    virtual Any visitPuts(TParser::PutsContext *ctx) override;
    virtual Any visitArifExpr(TParser::ArifExprContext *ctx) override;
    virtual Any visitIntExpr(TParser::IntExprContext *ctx) override;
    virtual Any visitFloatExpr(TParser::FloatExprContext *ctx) override;
    virtual Any visitBoolExpr(TParser::BoolExprContext *ctx) override;
    virtual Any visitIgetsExpr(TParser::IgetsExprContext *ctx) override;
    virtual Any visitBracketsExpr(TParser::BracketsExprContext *ctx) override;
    virtual Any visitIdExpr(TParser::IdExprContext *ctx) override;
    virtual Any visitAssignment(TParser::AssignmentContext *ctx) override;
    virtual Any visitIf_statement(TParser::If_statementContext *ctx) override;
};
}
#endif // CODEGENERATOR_H
