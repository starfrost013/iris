#include <filesystem.hpp>

//
// It's a FIlesystem API! THERE IS NO HOPE! 
// THis one just provides a very narrow thing...
//

namespace Iris
{
    FileContainer* Filesystem::Open(const char* path, FileMode mode)
    {
        FileContainer* fc = new FileContainer;

        auto flags = std::ios_base::in | std::ios_base::out;

        if (mode & FileMode::Binary)
            flags |= std::ios_base::binary;
        
        fc->stream.open(path, flags);
    
        if (fc->stream.bad())
        {
            delete fc;
            return nullptr;
        }

        return fc; 

    }

    void Filesystem::Close(FileContainer* fc)
    {
        // YOU MUST SET TO NULLPTR!
        delete fc; 
    }
}