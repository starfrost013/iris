
/*
    C    O    H    E    R    E    N    T
    Extensible Emulator Debugging Tools!

    Coherent is an extensible debugger for emulators that is intended to allow the debugging of multiple types of CPU cores in an easy way.
*/

#pragma once
#include <Iris.hpp>

namespace Iris
{
    #define COHERENT_VERSION        "StarfrostLib/Coherent 0.0 - June 16, 2026"

    class CoherentCommand
    {
        char name[STRING_MAX_SHORT];

    };

    /// @brief defines a command extension object. all types that implement this must inherit from this class.
    class CoherentExtension
    {

    private:
        std::vector<CoherentCommand*> commands;
    };

    /// @brief Defines a coherent system. A system is e.g. a CPU which is being debugged
    class CoherentSystem
    {

    public: 

        // We can allow the user to write custom implementations of the REgister class with this.
        template <typename T>
        class Register
        {
            T value; 

            T Read() { return value; };
            void Write(T newValue) { value = newValue; };

            void TextifyDecimal(T value, char* buf);
            void TextifyHex(T value, char* buf);            
        };

        template <typename T>
        void AddRegister(Register<T> reg);

    private: 

    };

    class Coherent
    {
    public: 
        /// @brief Initialise the coherent system
        static void Init();

        /// @brief Enters the Coherent system on command.
        static void Enter();

        /// @brief On the start of an instruction, call this method if enabled.
        static void ExecStart();

        /// @brief On the end of an instruction, call this method.
        static void ExecEnd();

        /// @brief Exit the coherent system.
        static void Leave();

        /// @brief SHut down the coherent system.
        static void Shutdown();

    private:
        std::vector<CoherentExtension> extensions;
    };
}
