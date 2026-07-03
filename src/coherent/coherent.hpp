
/*
    C    O    H    E    R    E    N    T
    Extensible Emulator Debugging Tools!

    Coherent is an extensible debugger for emulators that is intended to allow the debugging of multiple types of CPU cores in an easy way.
*/

#pragma once
#include <Iris.hpp>
#include <component/component.hpp>

namespace Iris
{
    #define COHERENT_LOG_PREFIX     "Debugger"
    #define COHERENT_VERSION        "Coherent Debugger - Version 0.1, July 2, 2026"
    #define LOGBUF_MAX_SIZE         16384
    #define LOGBUF_PURGE_SIZE       2048

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

        virtual void AddUI() { };
    private:
        std::vector<CoherentCommand*> commands;
    };

    /// @brief Defines a coherent system. A system is e.g. a CPU which is being debugged
    class CoherentSystem
    {

    public: 

        // We can allow the user to write custom implementations of the Register class with this.
        // Member templates are not allowed for variables, so provide a common base and make the templated register inherit from it. 
        // Registers are stored type-erased in the 'registers' map below.
        class RegisterBase
        {
        public:
            virtual std::any Read() = 0; 
            virtual void Write(std::any& value) = 0;
        };

        template <typename T>
        class Register : public RegisterBase
        {
        public:
            Register(T* value)
            {
                this->value = value;
            }
            
            /// @brief This DEREFERENCES the value of the register
            /// @return the register value
            std::any Read() override { return *value; };
            void Write(std::any& value) override { this->value = std::any_cast<T*>(value); };
                       
            void TextifyDecimal(T value, char* buf);
            void TextifyHex(T value, char* buf);            
        private: 
            T* value; 

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
        void AddRegister(Register<T>* reg, const char* name)
        {
            Logger::Log(std::format("CoherentSystem::AddRegister - Adding register with name {}", name).c_str(), LogChannels::Debug);
            registers[name] = reg; 
        }

        void Shutdown()
        {
            for (auto value : registers)
                delete value.second;

            registers.clear();
        }

        /// @brief might be slow. this really needs to have a custom access only iterators.
        std::unordered_map<const char*, RegisterBase*> registers;
    private: 

    };

    class Coherent
    {
    public: 
        /// @brief Initialise the coherent system
        static void Init();

        /// @brief Enters the Coherent system on command.
        static void Enter();

        /// @brief On the start of a tick for a certain component, call this method if enabled.
        static void ExecStart(Component* component);

        /// @brief On the end of an instruction, call this method.
        static void ExecEnd(Component* component);

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

        // SHOULD NOT BE PUBLIC, but because of some things with the design of SSLS 5, it is.
        static void AddTextToLogWindowBuffer(const char* str);

    private:
        /// @brief If this value is true, the coherent system has been initialised. 
        inline static bool initialised;

        /// @brief the list of extensions
        inline static std::vector<CoherentExtension*> extensions;

        /// @brief the current coherent system
        inline static CoherentSystem* currentSystem;

        // Methods for drawing specific user interfaces

        static void DrawMainWindow();
        static void DrawLogWindow();

        inline static char logBuffer[LOGBUF_MAX_SIZE] = {0};
        inline static bool logWindowTextIsStale = false; // don't bother recopying if it's not stale!
    };
}
