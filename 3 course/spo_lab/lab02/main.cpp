#include "translator.h"
#include <iostream>

using namespace std;

int main (int argc, char* argv[])
{
    string filename = R"(C:\GIT\cpp_workspace\_qt_workspace\3 course\spo_lab\lab02\tests\test_perecilki_asm)";
    Translator ASM(filename + ".txt");
    ASM.createBinFile(filename + ".bin");
    return 0;
}
