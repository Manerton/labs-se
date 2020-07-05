#ifndef LOADER_H
#define LOADER_H
#include <fstream>
#include <sstream>
#include <string>

class loader
{
public:
    loader(){}

    static std::string getText_from_file(const std::string &filename)
    {
       std::ifstream file(filename);
       if (file)
       {
           std::ostringstream oss;
           oss << file.rdbuf();
           return {oss.str()};
       }
       else
       {
           return std::string("");
       }
    }
};

#endif // LOADER_H
