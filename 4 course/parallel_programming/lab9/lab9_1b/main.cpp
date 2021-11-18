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
        const string filename = "lab9_shared_memory";

        const shared_memory_object shm_obj (open_only,
                                            filename.c_str(),
                                            read_only
                                            );

        const mapped_region region(shm_obj, read_only);

        string outStr;
        outStr.resize(region.get_size());

        copy_n(reinterpret_cast<StrPtr_t>(region.get_address()), region.get_size(), outStr.begin());

        cout << outStr << endl;

        shared_memory_object::remove(filename.c_str());
    }
    catch (interprocess_exception &ex)
    {
        cout << "Error: " << ex.what() << endl;
        return 1;
    }

    return 0;

}
