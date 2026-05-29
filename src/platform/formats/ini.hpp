
/*
    INI parser
    Required Information for Config
*/

#include <Iris.hpp>
#include <base/filesystem/filesystem.hpp>

#define STRING_VERSION          "StarfrostLib/INI Version 1.0 - May 28, 2026"

namespace Iris
{
    class INISection
    {
        char name[STRING_MAX_SHORT];
        
        INISection* next; 
    };

    class INIFile
    {
        void Open(const char* path)
        {
            file = Filesystem::Open(path);
        }

        void OpenAndParse(const char* path)
        {
            Open(path);
        }

        void Close()
        {
            Filesystem::Close(file);
            open = false;
        }

    private: 
        FileStream* file;
        char path[STRING_MAX_PATH];
        bool open;
    };
}