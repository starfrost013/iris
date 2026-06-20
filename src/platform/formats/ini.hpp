
/*
    INI parser
    Required Information for Config
*/

#include <Iris.hpp>
#include <base/filesystem/filesystem.hpp>


#define STRING_VERSION          "StarfrostLib/INI Version 1.0 - June 4, 2026"

namespace Iris
{
    class INISection
    {
    public: 
        char name[STRING_MAX_SHORT];

        std::unordered_map<std::string_view, std::string_view> values;
    
        INISection* prev; 
        INISection* next; 

        INISection()
        {
            
        }
    };

    class INIFile
    {
        INISection* firstSection; 
        INISection* lastSection; 

        enum ParserMode
        {
            Section,
            Key,
            Value,
        };
        
        void Open(const char* path)
        {
            file = Filesystem::Open(path);
        }

        void OpenAndParse(const char* path)
        {
            Open(path);

            std::string theString;
            const char* line; 
            size_t line_length;

            char keyBuf[STRING_MAX_SHORT] = {0};
            char valueBuf[STRING_MAX_SHORT] = {0};
            ParserMode parserMode = ParserMode::Key; 
            INISection* curSection;

            size_t keySize = 0, valueSize = 0, sectionNameSize = 0;
            
            while (std::getline(file->stream, theString))
            {
                line = theString.c_str();
                line_length = strlen(line);

                for (size_t i = 0; i < line_length; i++)
                {
                    char ch = line[i];

                    // check if it is a space
                    if (isspace(ch))
                        break; 

                    // comment, skip
                    if (ch == '#')
                        break; 

                    switch (ch)
                    {
                        case '=':
                            parserMode = ParserMode::Value;
                            break; 
                        case '[': // section
                            parserMode = ParserMode::Section;
                            curSection = new INISection;

                            if (!firstSection)
                                firstSection = lastSection = curSection;
                            else
                            {
                                lastSection->next = curSection;
                                curSection->prev = lastSection;

                                lastSection = curSection;
                            }
                            
                            break;
                        case ']':
                            if (parserMode != ParserMode::Section)
                            {
                                Logger::Log("INI file parsing error: ] encountered without also [ for section", LogChannels::Error);
                            }
                            break;
                    }          
                    
                    switch (parserMode)
                    {
                        case ParserMode::Key:
                            if (keySize < sizeof(keyBuf) - 1)
                                keyBuf[keySize] = ch;
                            keySize++;
                            break;
                        case ParserMode::Value:
                            if (valueSize < sizeof(valueBuf) - 1)
                                valueBuf[valueSize] = ch;

                            valueSize++;
                            break; 
                        case ParserMode::Section:
                            
                            break;

                    }
                }

                // it may be a bad idea 
                curSection->values[keyBuf] = valueBuf;  

                memset(keyBuf, 0x00, strlen(keyBuf));
                memset(valueBuf, 0x00, strlen(valueBuf));
            }
        }

        void Close()
        {
            INISection* currentSection = lastSection;
            INISection* nextSection = lastSection->prev;

            // delete all our sections
            while (currentSection)
            {
                delete currentSection;
                currentSection = nextSection;
                nextSection = currentSection->prev; 
            }

            Filesystem::Close(file);
            open = false;
        }

    private: 
        FileStream* file;
        char path[STRING_MAX_PATH];
        bool open;
    };
}