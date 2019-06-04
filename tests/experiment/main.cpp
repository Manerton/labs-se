#include <windows.h>
#include <iostream>
#include <set>
#include <algorithm>

using namespace std;
typedef set<int, less<int> > set_type;

int main()
{
    set_type M1 {1, 4, 6};
    set_type M2 {2, 4, 6};
    set_type M3 {1, 3, 2};
    set_type M4 {7, 3, 6};

    set_type temp;
    set_type M12;
    set_intersection (M1.begin(), M1.end(), M2.begin(), M2.end(), inserter(M12, M12.begin()));
    set_intersection (M1.begin(), M1.end(), M2.begin(), M2.end(), inserter(temp, temp.begin()));
    cout << M12.size();
    set_type M13;
    set_intersection (M1.begin(), M1.end(), M3.begin(), M3.end(), inserter(M13, M13.begin()));
    set_intersection (M1.begin(), M1.end(), M3.begin(), M3.end(), inserter(temp, temp.begin()));
    cout << M13.size();
    set_type result;
    set_union (M12.begin(), M12.end(), M13.begin(), M13.end(), inserter(result, result.begin()));
    cout << result.size();
    cout << temp.size();
    /*set_type M14;
    set_intersection (M1.begin(), M1.end(), M4.begin(), M4.end(), inserter(M14, M14.begin()));
    set_type M23;
    set_intersection (M2.begin(), M2.end(), M3.begin(), M3.end(), inserter(M23, M23.begin()));
    set_type M24;
    set_intersection (M2.begin(), M2.end(), M4.begin(), M4.end(), inserter(M24, M24.begin()));
    set_type M34;
    set_intersection (M3.begin(), M3.end(), M4.begin(), M4.end(), inserter(M34, M34.begin()));
*/
    return 0;
}
