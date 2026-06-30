
/*
    C    O    H    E    R    E    N    T
    Extensible Emulator Debugging Tools!

    Coherent is an extensible debugger for emulators that is intended to allow the debugging of multiple types of CPU cores in an easy way.
*/

#pragma once
#include <Iris.hpp>

namespace Iris
{
    #define COHERENT_LOG_PREFIX     "Debugger"
    #define COHERENT_VERSION        "StarfrostLib/Coherent 0.1 - June 20, 2026"
    #define LOGBUF_MAX_SIZE         16384

    class CoherentCommand
    {
        char name[STRING_MAX_SHORT];

    };

    /// @brief defines a command extension object. all types that implement this must inherit from this class.
    class CoherentExtension
    {
    public: 
        void AddCommand(CoherentCommand* command)
        {
            if (!command)
            {
                Logger::Log("CoherentExtension::AddCommand - command is nullptr", LogChannels::Error);
                return;
            }

            commands.push_back(command);
        }

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

        /// @brief Disassemble the next instruction. . It's up to you to figure out the buffer size.
        virtual char* DisasmNext() { return nullptr; };
        
        /// @brief Disassemble a range of instructions.
        /// @param start The instrruction to disassemble.
        /// @param end The instruction to stop disassembling at.
        /// @return note: If you provide an unaligned instruction, it will just stop before the end. It's up to you to figure out the buffer size.
        virtual char* DisasmRange(size_t start, size_t end) { return nullptr; };

        /// @brief the run state of the system
        enum RunState
        {
            Running = 0,
            Paused = 1,
            Stopped = 2,
            Reset = 3,
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

        /// @brief Render a frame of the debugger (see coherent_gui.cpp)
        static void Frame();

        /// @brief Exit the coherent system.
        static void Leave();

        /// @brief SHut down the coherent system.
        static void Shutdown();

        /// @brief Register a coherent extension.
        /// @param extension A pointer to a valid CoherentExtension* object
        static void RegisterExtension(CoherentExtension* extension);

        // Getters for private members
        bool GetInitialised() { return initialised; };

        // Setters for private members
        static void SetSystem(CoherentSystem* system) { currentSystem = system; }; 
        
        /// @brief If this is true, the coherent system is currently active. (needs to be public because of imgui)
        inline static bool active; 

    private:
        /// @brief If this value is true, the coherent system has been initialised. 
        inline static bool initialised;

        /// @brief the list of extensions
        inline static std::vector<CoherentExtension*> extensions;

        /// @brief the current coherent system
        inline static CoherentSystem* currentSystem;

        // Methods for drawing specific user interfaces

        void DrawLogWindow();

        char logBuffer[LOGBUF_MAX_SIZE];
    };
}
