#include <iostream> //Для работы с клавиатурой
#include <fstream> //Для работы с файлами
#include <cstdlib> //Для очистки экрана

using namespace std;

int main()
{
  system("chcp 1251 > null");
  int N;
  cout << "N: ";
  cin >> N;
  N = 100;
  int *A = new int[N];
  for (int i = 0; i < N; i++)
  {
      A[i] = i;
  }
  cin >> N;
  N = 10000;
  int P = 100000;
  int K = 100000;
  for (int i = 0; i < K; i++)
  {
      double *B = new (nothrow) double[N];
      double *C = new (nothrow) double[P];
  }
  cout << "done" << endl;
  //A = nullptr;
  if (!A) cout << "empty ptr";
  cin >> N;
  return 0;
}
