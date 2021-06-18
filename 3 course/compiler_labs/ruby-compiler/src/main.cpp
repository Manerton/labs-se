#include <iostream>
#include <fstream>

#include "antlr4-runtime.h"
#include "TLexer.h"
#include "TParser.h"
#include "CodeGenerator.h"

using namespace rubyCompiler;
using namespace antlr4;
using namespace std;

int main(int , const char **) {
    system("chcp 65001");
    std::ifstream f("test.txt");
    if (f.is_open())
    {
        ANTLRInputStream input(f);
        TLexer lexer(&input);
        CommonTokenStream tokens(&lexer);

        tokens.fill();

        /*for (auto token : tokens.getTokens()) {
            cout << token->toString() << endl;
        }*/

        TParser parser(&tokens);
        auto tree = parser.programm();

       // cout << tree->toStringTree(&parser, true) << endl;

        CodeGenerator generator;
        generator.generateCode(tree);

        for (const auto &codeLine : generator.getCode())
        {
            cout << codeLine << endl;
        }
    }
    system("pause");
    return 0;
}
