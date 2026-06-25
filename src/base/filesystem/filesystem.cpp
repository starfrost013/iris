#include <base/filesystem/filesystem.hpp>

//
// It's a FIlesystem API! THERE IS NO HOPE! 
// THis one just provides a very narrow thing...
//

namespace Iris
{
    FileStream* Filesystem::Open(const char* path, FileMode mode)
    {
        FileStream* fc = new FileStream;

        auto flags = std::ios_base::in | std::ios_base::out;

        if (mode & FileMode::Binary)
            flags |= std::ios_base::binary;
        
        fc->stream.open(path, flags);
    
        if (fc->stream.bad())
        {
            delete fc;
            return nullptr;
        }

        fc->open = true; 
        return fc; 

    }

    void Filesystem::Close(FileStream* fc)
    {
        // YOU MUST SET TO NULLPTR!
        delete fc; 
    }
}