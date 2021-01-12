#include "translator.h"
#include <iostream>

using namespace std;

int main (int argc, char* argv[])
{
    if (argc > 1)
    {
        string_view filename = argv[1];
        cout << "File " << filename << endl;
        size_t pointIndex = filename.rfind('.');
        std::string filename_without_type{filename.substr(0,pointIndex)};
        Translator ASM(filename);
        if (ASM.createListingFile(filename_without_type + ".log"))
        { ASM.createBinFile(filename_without_type + ".bin"); }
    } else cout << "Program has been started without arguments." << endl;
    system("pause");

    return 0;
}
