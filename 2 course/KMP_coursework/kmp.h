#ifndef KMP_H
#define KMP_H
#include <QString>
#include <vector>

struct pref_func_step_info
{
   int Pi; // -- значение функции Pi для i-го элемента -- //
   int i;  // -- позиция для буквы i -- //
   int j;  // -- позиция для буквы j -- //
   int code_i;
};

struct kmp_step_info
{
   int l;  // -- позиция для буквы i -- //
   int k;  // -- позиция для буквы j -- //
   int code_i;
};

class kmp
{
    QString str;
    QString substr;
    std::vector<pref_func_step_info> v_pref_info;
    std::vector<kmp_step_info> v_kmp_info;
public:
    kmp(QString _str = "", QString _substr = "") : str(_str), substr(_substr) {}
    std::vector<int> prefix_function();
    bool kmp_algorithm(const std::vector<int> &Pi);
    const std::vector<pref_func_step_info>& get_pref_step_info() const;
    const std::vector<kmp_step_info>& get_kmp_step_info() const;
};

#endif // KMP_H
