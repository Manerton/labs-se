#include <iostream>
#include <fstream>

#include "antlr4-runtime.h"
#include "TLexer.h"
#include "TParser.h"
#include "CodeGenerator.h"
#include "CompilerException.h"

using namespace rubyCompiler;
using namespace antlr4;
using namespace std;

int main(int argc, char* argv[]) {
    system("chcp 65001");
    if (argc > 0 /*1*/ )
    {
        std::ifstream file(/*argv[1]*/ "test.txt");

        if (file)
        {
            // входной файл
            ANTLRInputStream input(file);

            // лексер
            TLexer lexer(&input);
            CommonTokenStream tokens(&lexer);
            tokens.fill();

            // парсер
            TParser parser(&tokens);
            auto tree = parser.programm();

            // генератор кода (паттерн Visitor по дереву АСД после парсера)
            if (lexer.getNumberOfSyntaxErrors() == 0
                    && parser.getNumberOfSyntaxErrors() == 0)
            {
                try
                {
                    CodeGenerator generator;
                    generator.generateCode(tree);
                    // вывод кода в буфер (консоль, файл)
                    for (const auto &codeLine: generator.getCode())
                    {
                        cout << codeLine << endl;
                    }
                }
                catch (CompilerException &c)
                {
                    cout << "test.txt" << ":" << c.what() << endl;
                }
            }
        }
    }
    system("pause");
    return 0;
}
