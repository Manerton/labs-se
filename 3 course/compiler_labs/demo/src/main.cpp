/* Copyright (c) 2012-2017 The ANTLR Project. All rights reserved.
 * Use of this file is governed by the BSD 3-clause license that
 * can be found in the LICENSE.txt file in the project root.
 */

//
//  main.cpp
//  antlr4-cpp-demo
//
//  Created by Mike Lischke on 13.03.16.
//

#include <iostream>
#include <fstream>

#include "antlr4-runtime.h"
#include "TLexer.h"
#include "TParser.h"

using namespace antlrcpptest;
using namespace antlr4;

int main(int , const char **) {
    system("chcp 65001");
    std::ifstream f("test.txt");
    if (f.is_open())
    {
        ANTLRInputStream input(f);
        TLexer lexer(&input);
        CommonTokenStream tokens(&lexer);

        tokens.fill();
        for (auto token : tokens.getTokens()) {
            std::cout << token->toString() << std::endl;
        }

        TParser parser(&tokens);
        tree::ParseTree* tree = parser.programm();

        std::cout << tree->toStringTree(&parser, true) << std::endl << std::endl;
    }
    system("pause");
    return 0;
}
