#include <Iris.hpp>
#include <filesystem>

//
// It's a FIlesystem API! THERE IS NO HOPE! 
// THis one just provides a very narrow thing...
//

namespace Iris
{
    class FileContainer
    {
    public: 
        // the stream of the file
        std::fstream stream;
        bool open;
    };

    enum FileMode
    {
        Text = 1,
        Binary = 1 << 1,
    }; 

    class Filesystem
    {
        static FileContainer* Open(const char* path, FileMode mode);
        static void Close(FileContainer* con);

    };
}; 