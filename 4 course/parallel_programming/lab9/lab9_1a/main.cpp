#include <iostream>
#include <string>
#include <algorithm>

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

using namespace std;
using namespace boost::interprocess;

using StrPtr_t = string::iterator::pointer;

int main()
{
    try
    {
        shared_memory_object shm_obj (open_or_create,
                                      "lab9_shared_memory",
                                      read_write
                                      );

        string str = "String from A application";

        shm_obj.truncate(offset_t(str.size()));

        mapped_region region(shm_obj, read_write);

        copy(str.begin(), str.end(), reinterpret_cast<StrPtr_t>(region.get_address()));
    }
    catch (interprocess_exception &ex)
    {
        cout << ex.what() << endl;
        return 1;
    }

    return 0;

}
