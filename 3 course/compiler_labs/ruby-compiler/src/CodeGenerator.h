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
    using internalVarName = std::string;

    struct Value
    {
        enum class Type : uint8_t
        {
            uint_t,   // беззнаковое целое
            int_t,    // знаковое целое
            float_t,  // дробное
            bool_t    // булева
        };

        Type type;            // тип
        std::variant<internalVarName, SafeInt<int64_t>, float, bool> value;  // значение
    };

    using Type = CodeGenerator::Value::Type;

// поля
    SourceCode code;                            // сгенерированный код ассемблера
    std::vector<internalVarName> tempValues;    // метки со служебными значениями в памяти ВМ
    std::map<string, Value> varTable;           // таблица переменных
    std::map<string, Value> arrTable;           // таблица с переменными, который являются первыми элементами массивов
    uint8_t stack_i = 0;                        // счетчик стека ВМ
    static constexpr uint8_t stack_size = 32;   // размер стека ВМ
    size_t asmLabel_i = 0;                      // счетчик меток в ВМ (для условий)
    bool runtimeThisCompExpr = false;           // для циклов, принудительный расчет условия в рантайме

// методы
    bool isInteger(const string &str) const;
    bool isFloat(const string &str) const;
    void generateTempValues();
    string getStrOfType(Value::Type type) const;
    void putsCompileTime(const Value &expr);
    void putsRuntime(const Value &expr);
    Type isSignedOrNot(int64_t val);
    Value arifExprCompileTime(TParser::ArifExprContext *ctx, Value left, Value right);
    Value arifExprRuntime(TParser::ArifExprContext *ctx, Value left, Value right);
    Value compExprCompileTime(TParser::CompExprContext *ctx, Value left, Value right);
    Value compExprRuntime(TParser::CompExprContext *ctx, size_t label_i, Value left, Value right);
    Value createRuntimeVar(const Value &val);
public:
    const SourceCode& getCode() const;

    void generateCode(antlr4::tree::ParseTree *tree);
    // обход дерева
    virtual Any visitProgramm(TParser::ProgrammContext *ctx) override;
    virtual Any visitPuts(TParser::PutsContext *ctx) override;
    virtual Any visitArifExpr(TParser::ArifExprContext *ctx) override;
    virtual Any visitCompExpr(TParser::CompExprContext *ctx) override;
    // <-- листья в Expr (возвращают Value) --
    virtual Any visitIdExpr(TParser::IdExprContext *ctx) override;
    virtual Any visitBoolExpr(TParser::BoolExprContext *ctx) override;
    virtual Any visitFloatExpr(TParser::FloatExprContext *ctx) override;
    virtual Any visitIntExpr(TParser::IntExprContext *ctx) override;
    virtual Any visitIgetsExpr(TParser::IgetsExprContext *) override;
    virtual Any visitUgetsExpr(TParser::UgetsExprContext *ctx) override;
    virtual Any visitFgetsExpr(TParser::FgetsExprContext *) override;
    virtual Any visitAssignment(TParser::AssignmentContext *ctx) override;
    virtual Any visitAssignExpr(TParser::AssignExprContext *ctx) override;
    virtual Any visitUnaryMinusExpr(TParser::UnaryMinusExprContext *ctx) override;
    // -------------------------------------->
    virtual Any visitBracketsExpr(TParser::BracketsExprContext *ctx) override;
    virtual Any visitIf_statement(TParser::If_statementContext *ctx) override;
    virtual Any visitWhile_statement(TParser::While_statementContext *ctx) override;
    Any visitRuntimeCompExpr(TParser::CompExprContext *ctx);
    virtual Any visitArrDefExpr(TParser::ArrDefExprContext *ctx) override;
    virtual Any visitArray_definition(TParser::Array_definitionContext *ctx) override;
    virtual Any visitArray_definition_elements(TParser::Array_definition_elementsContext *ctx) override;
    virtual Any visitArrSelExpr(TParser::ArrSelExprContext *ctx) override;
    virtual Any visitArray_selector(TParser::Array_selectorContext *ctx) override;

};
}
#endif // CODEGENERATOR_H
