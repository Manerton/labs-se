#ifndef TEO_H
#define TEO_H

#include <vector>

using namespace std;

typedef vector<string> page;
typedef vector<page> text;

string get_filename ();
void crypt(string filename);
void decrypt(string filename);
void creation_text (text &TEXT);
void one_page_display (const page &onepage);
void teo_display (text TEXT);


#endif // TEO_H
