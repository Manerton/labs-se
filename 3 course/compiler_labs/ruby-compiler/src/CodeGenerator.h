#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include <list>
#include <string>
#include <vector>
#include <variant>
#include "../thirdparty/SafeInt.hpp"
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
            uint_t,   // беззнаковое целое
            int_t,    // знаковое целое
            float_t,      // дробное
            bool_t     // булева
        };

        bool runtime = false; // значение известно на этапе компиляции
                              // или на рантайме?

        Type type;            // тип
        std::variant<std::monostate, SafeInt<int64_t>, float, bool> value;  // значение
    };

    using Type = CodeGenerator::Value::Type;
private:
    // поля
    SourceCode code;                    // сгенерированный код ассемблера
    std::vector<string> tempValues;     // метки со служебными значениями в памяти ВМ
    std::map<string, Value> varTable;   // таблица переменных

    // методы
    bool isInteger(const string &str) const;
    bool isFloat(const string &str) const;
    void generateTempValues();
    string getStrOfType(Value::Type type) const;
    void putsCompileTime(const Value &expr);
    void putsRuntime(const Value &expr);
    Type isSignedOrNot(int64_t val);
public:
    const SourceCode& getCode() const;

    void generateCode(antlr4::tree::ParseTree *tree);
    // обход дерева
    virtual Any visitProgramm(TParser::ProgrammContext *ctx) override;
    virtual Any visitPuts(TParser::PutsContext *ctx) override;
    virtual Any visitArifExpr(TParser::ArifExprContext *ctx) override;
    // <-- листья в Expr (возвращают Value) --
    virtual Any visitIdExpr(TParser::IdExprContext *ctx) override;
    virtual Any visitBoolExpr(TParser::BoolExprContext *ctx) override;
    virtual Any visitFloatExpr(TParser::FloatExprContext *ctx) override;
    virtual Any visitIntExpr(TParser::IntExprContext *ctx) override;
    virtual Any visitIgetsExpr(TParser::IgetsExprContext *) override;
    virtual Any visitFgetsExpr(TParser::FgetsExprContext *) override;
    virtual Any visitAssignment(TParser::AssignmentContext *ctx) override;
    // -------------------------------------->
    virtual Any visitBracketsExpr(TParser::BracketsExprContext *ctx) override;
    virtual Any visitIf_statement(TParser::If_statementContext *ctx) override;
};
}
#endif // CODEGENERATOR_H
