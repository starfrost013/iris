
/*
    C    O    H    E    R    E    N    T
    Extensible Emulator Debugging Tools!

    Coherent is an extensible debugger for emulators that is intended to allow the debugging of multiple types of CPU cores in an easy way.
*/

#pragma once
#include <Iris.hpp>
#include <base/emulation.hpp>
#include <component/addrspace.hpp>
#include <component/component.hpp>
#include <coherent/coherent_gui_imgui.hpp>

namespace Iris
{
    #define COHERENT_LOG_PREFIX     "Debugger"
    #define COHERENT_VERSION        "Coherent Debugging Engine v0.5 (July 2026)"

    extern Cvar* startPaused;

    class CoherentCommand
    {
        char name[STRING_MAX_SHORT];
    };

    /// @brief defines a command extension object. all types that implement this must inherit from this class.
    /// the objects are automatically added to the menu
    class CoherentExtension
    {
    public:
        Component* component; 
        bool enabled = false; 

        CoherentExtension(Component* component)
        {
            this->component = component;
        }

        /// @brief Adds a command to a Coherent extension.
        /// @param command A pointer to teh command object to add.
        void AddCommand(CoherentCommand* command)
        {
            if (!command)
            {
                Logger::Log("CoherentExtension::AddCommand - command is nullptr", LogChannels::Error);
                return;
            }

            commands.push_back(command);
        }

        /// @brief Add the UI for a Coherent extension. Currently it gets added to the Peripherals menu.
        virtual void AddUI() { };

    private:
        std::vector<CoherentCommand*> commands;
    };

    /// @brief Defines a coherent system. A system is e.g. a CPU which is being debugged
    class CoherentSystem
    {

    public: 

        class ExceptionVectorBase
        {
        public: 
            const char* name; 
        };

        // exception vectors can be different sizes
        template <typename T>
        class ExceptionVector : public ExceptionVectorBase
        {
            T id; 
        public:
            ExceptionVector(const char* name, T id)
            {
                this->id = id;
                this->name = name;
            }
        };

        // We can allow the user to write custom implementations of the Register class with this.
        // Member templates are not allowed for variables, so provide a common base and make the templated register inherit from it. 
        // Registers are stored type-erased in the 'registers' map below.
        class RegisterBase
        {
        public:
            const char* name;

            virtual std::any Read() = 0; 
            virtual void Write(std::any& value) = 0;
        };

        template <typename T>
        class Register : public RegisterBase
        {
        public:
            Register(T* value, const char* name)
            {
                this->name = name;
                this->value = value;
            }
            
            /// @brief This DEREFERENCES the value of the register
            /// @return the register value
            std::any Read() override { return *value; };

            /// @brief Write the register
            /// @param value The register value to write - *MUST* Be a pointer. It gets converted to a pointer automatically so make sure it isn't automatically destroyed
            void Write(std::any& value) override { this->value = std::any_cast<T>(&value); }; 
        private: 
            T* value; 

        };

        /// @brief Disassemble a range of instructions.
        /// @param start The instrruction to disassemble.
        /// @param end The instruction to stop disassembling at.
        /// @return note: If you provide an unaligned instruction, it will just stop before the end. It's up to you to figure out the buffer size.
        virtual char* DisasmInstruction(size_t start) { return nullptr; };

        /// @brief Get the Program counter
        /// @return The program counter of the current system.
        virtual size_t GetPC() { return 0; };

        /// @brief enumerates the run states of the system
        enum RunState
        {
            Running = 0,
            Paused = 1,
            Reset = 2,
            SingleStep = 3,
        };

        /// @brief Add a register to this system
        /// @tparam T The type of the register to add.
        /// @param reg The Register<T> object to ad.
        /// @param name The friendly name of the register.
        template <typename T>
        void AddRegister(Register<T>* reg)
        {
            Logger::Log(std::format("CoherentSystem::AddRegister - Adding register with name {}", reg->name).c_str(), LogChannels::Debug);
            registers.push_back(reg); 
        }

        void Shutdown()
        {
            for (auto reg : registers)
                delete reg;

            registers.clear();
        }

        /// @brief might be slow. this really needs to have a custom access only iterators.
        std::vector<RegisterBase*> registers;

        /// getters for private fields
        size_t GetNextInstructionSize() { return nextInstructionSize; };
        /// @brief get the run state of the system
        CoherentSystem::RunState GetRunState();

        /// setters for private fields

        /// @brief set the run state of the system
        void SetRunState(CoherentSystem::RunState runState);

    protected: 
        /// @brief the run state of the system
        inline static RunState runState;

        inline static size_t nextInstructionSize;
    };

    class Coherent
    {
        friend class CoherentUI;

    public: 
    
        /// @brief Initialise the coherent system
        static void Init();

        /// @brief Enters the Coherent system on command.
        static void Enter();
        
        /// @brief Tick the debugger. Called before all emulation components are ticked.
        static void Tick();
        
        /// @brief Render a frame of the debugger (see coherent_gui.cpp)
        static void Frame();
        
        /// @brief Called when the coherent system entered a breakpoint.
        static void OnBreakpointHit() 
        {
            // breakpoint is hit pause the system
            currentSystem->SetRunState(CoherentSystem::RunState::Paused);
        }

        /// This is the base class for all types of guards.
        class Guard
        {
        public: 
            size_t addr; 
            bool enabled;
            bool active; 

            /// @brief help for ui. set to true if the user selected this
            bool selected; 

            Guard()
            {
                this->addr = 0x0;
                this->enabled = false;
                this->active = false; 
            }
        
            Guard(size_t addr)
            {
                this->addr = addr;
                this->enabled = false;
                this->active = false; 
            }
        };

        /// defines a breakpoint
        class Breakpoint : public Guard
        {
        public:
            Breakpoint() : Guard() { }
            Breakpoint(size_t addr) : Guard(addr) { }
        };

        class Watchpoint : public Guard
        {
        public: 
            Watchpoint() : Guard() { }
            Watchpoint(size_t addr) : Guard(addr) { }

            // TODO: Add templates for these & use std::any
            uint32_t GetValue() { return AddrSpace::ReadU32(addr); }; 
        }; 

        class Catchpoint : public Guard
        {
        public: 
            Catchpoint() : Guard() { }
            Catchpoint(size_t addr, size_t exceptionId) : Guard(addr) 
            { 
                this->exceptionId = exceptionId;
            }

            size_t exceptionId;
        };

        /// @brief Called when the coherent system was requested to remove a breakpoint.
        static void AddBreakpoint(Breakpoint bp);
        static void AddWatchpoint(Watchpoint wp);
        static void AddCatchpoint(Catchpoint cp);

        /// @brief Called when the coherent system was requested to remove a breakpoint.
        static void RemoveBreakpoint(Breakpoint bp);
        static void RemoveWatchpoint(Watchpoint wp);
        static void RemoveCatchpoint(Catchpoint cp);

        static Breakpoint GetBreakpointByAddr(size_t addr);
        static Watchpoint GetWatchpointByAddr(size_t addr);
        static Catchpoint GetCatchpointByAddr(size_t addr);

        // @brief Exit the coherent system.
        static void Leave();

        /// @brief SHut down the coherent system.
        static void Shutdown();

        /// @brief Register a coherent extension.
        /// @param extension A pointer to a valid CoherentExtension* object
        static void RegisterExtension(CoherentExtension* extension);

        // Getters for private members
        static bool GetInitialised() { return initialised; };
        static CoherentSystem* GetSystem() { return currentSystem; };

        // Setters for private members
        static void SetSystem(CoherentSystem* system) 
        { 
            currentSystem = system; 
        
            // start pausd if we configured to do so (for debugging)
            if (currentSystem != nullptr && startPaused->GetValue())
            {
                currentSystem->SetRunState(CoherentSystem::RunState::Paused);
            }
        }; 
        
        /// @brief If this is true, the coherent system is currently active. (needs to be public because of imgui)
        inline static bool active;


    private:
        /// @brief If this value is true, the coherent system has been initialised. 
        inline static bool initialised;

        /// @brief the list of extensions
        inline static std::vector<CoherentExtension*> extensions;

        /// @brief the current coherent system
        inline static CoherentSystem* currentSystem;

        // key is the size_t
        inline static std::unordered_map<size_t, Breakpoint> breakpoints;
        inline static std::unordered_map<size_t, Watchpoint> watchpoints;
        inline static std::unordered_map<size_t, Catchpoint> catchpoints;
    };
}
