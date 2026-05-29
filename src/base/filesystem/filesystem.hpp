#include <Iris.hpp>
#include <filesystem>

//
// It's a FIlesystem API! THERE IS NO HOPE! 
// THis one just provides a very narrow thing...
//

namespace Iris
{
    class FileStream
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
    public:
        static FileStream* Open(const char* path, FileMode mode = FileMode::Text);
        static void Close(FileStream* con);

    };
}; 