#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include <list>
#include <string>
#include <vector>
#include "TParserBaseVisitor.h"

using std::string;
using std::vector;
using std::list;

class CodeGenerator : public rubyCompiler::TParserBaseVisitor {
    using CodeLine = string;
    using SourceCode = list<CodeLine>;
// поля
    SourceCode code;            // сгенерированный код ассемблера
    vector<string> tempValues;  // метки со служебными значениями в памяти ВМ

// методы
    void generateTempValues();
    bool isInteger(const string &str) const;
    bool isFloat(const string &str) const;

public:
    const SourceCode& getCode() const;

    void generateCode(antlr4::tree::ParseTree *tree);
    // обход дерева
    virtual antlrcpp::Any visitProgramm(rubyCompiler::TParser::ProgrammContext *ctx) override;
    virtual antlrcpp::Any visitPuts(rubyCompiler::TParser::PutsContext *ctx) override;
    virtual antlrcpp::Any visitArifExpr(rubyCompiler::TParser::ArifExprContext *ctx) override;
    virtual antlrcpp::Any visitIntExpr(rubyCompiler::TParser::IntExprContext *ctx) override;
    virtual antlrcpp::Any visitFloatExpr(rubyCompiler::TParser::FloatExprContext *ctx) override;
    virtual antlrcpp::Any visitLiteralExpr(rubyCompiler::TParser::LiteralExprContext *ctx) override;
    virtual antlrcpp::Any visitBracketsExpr(rubyCompiler::TParser::BracketsExprContext *ctx) override;
    virtual antlrcpp::Any visitIdExpr(rubyCompiler::TParser::IdExprContext *ctx) override;
};

#endif // CODEGENERATOR_H
