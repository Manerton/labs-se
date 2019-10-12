#ifndef UNITTEST_H_INCLUDED
#define UNITTEST_H_INCLUDED
#include <iostream>
#include <string>
#include <typeinfo>
// -- проверяющий макрос -- смотри метод dotest() --
#define check(cond) dotest(cond, #cond, __FILE__, __LINE__)
// -- макрос - вывод сообщения об ошибке --
// -- именно поэтому dofail() сделана отдельно --
#define error(str) dofail(str, __FILE__, __LINE__)

namespace UnitTest			// -- тестовое пространство имен -–
{
    using std::cout;
    using std::endl;
    using std::string;
// -- базовый абстрактный тестовый класс --
class Test
{
    size_t nPass = 0;            // -- количество пройденных тестов --
    size_t nFail = 0;            // -- количество проваленных тестов --
    // -- эти конструкторы-присваивания создавать не надо --
    Test(const Test &)  = delete;
    Test(const Test &&) = delete;
    Test& operator=(const Test &)  = delete;
    Test& operator=(const Test &&) = delete;
    // -- вырезаем чистое имя типа из  typeid().name() – особенность g++ ??? --
    string getTypeName(const string &t) const
    {
        size_t k = 0;
        stoi(t, &k);            // -- результат не нужен - нужен индекс k --
        string s = t.substr(k); // -- отрезаем начальную длину --
        return s;
    }
public:
    Test():nPass(0), nFail(0){}   // -- тривиальный конструктор --
    virtual void run() = 0;       // -- определяется в классах-наследниках от теста --
    void report() const           // -- вывод результатов тестирования --
    {
        string name = typeid(*this).name();		// -- имя класса --
        cout << "-----\n";
        cout  << "Testing <" << getTypeName(name) << ">\n"
              << "All tests    = " << (nPass+nFail) << ";\n"
              << "Passed tests = " << nPass << ";\n"
              << "Failed tests = " << nFail << ";\n"
              << "-----\n";
    }
    void reset() { nPass = nFail = 0; }
    void success(){ ++nPass; }		// -- только ради исключений –
    void fail()   { ++nFail; }			// -- только ради исключений –

protected:		// -- наследуемые защищенные методы
    // -- параметр cond - это и есть проверка --
    void dotest(bool cond, const string &message, const char *filename, long nline)
    {
        if(cond) ++nPass;         // -- тест прошел --
        else                      // -- тест НЕ прошел --
        {
            ++nFail;
            dofail(message, filename, nline);	// -- условие, файл и строка файла
        }
    }
    // -- вывод провального теста --
    void dofail(const string &message, const char *filename, long nline)
    { // -- вырезаем имя файла из полного имени filename --
        auto f = [](const string &name)-> string
        {
            auto idx = name.rfind("\\");
            return name.substr(idx+1);
        };
        string name = typeid(*this).name();
        cout  << "<" << getTypeName(name) << ">\n"
              << "Failure: (" << message << ") \n"
              << "File: " << f(string(filename)) << "; line = " << nline
              << endl;
    }
};
}   // -- end namespace --
#endif // UNITTEST_H_INCLUDED
