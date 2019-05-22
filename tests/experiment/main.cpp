#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

int main()
{
    ofstream toBin("number.bin", ios::binary);
    /*time_t t; srand((unsigned) time (&t));
    for (int i = 0; i < 1000; ++i)
    {
        int t = rand();
        toBin.write(reinterpret_cast<char *>(&t), sizeof(int));
    }*/
    char t = 'z';
    toBin.write((char*)&t, sizeof(char));
    t = 'e';
    toBin.write((char*)&t, sizeof(char));
    //cout << t;
    toBin.close();
    //ofstream toTxt("number.txt");
    /*srand((unsigned) time(&t));
    for (int i =0; i < 1000; i++) toTxt << rand() << endl;*/
    //toTxt << t;
    //toTxt.close();
    return 0;
}
