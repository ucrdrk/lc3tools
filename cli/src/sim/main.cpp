#include <algorithm>
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

#include "helper.h"
#include "console_printer.h"
#include "console_inputter.h"

void help(void);
bool prompt(void);
bool promptMain(std::stringstream & command_tokens);
void promptBreak(std::stringstream & command_tokens);
std::string formatMem(uint32_t addr);
std::ostream & operator<<(std::ostream & out, Breakpoint const & x);
void breakpointHitCallback(core::MachineState & state, Breakpoint const & bp);

int main(int argc, char * argv[])
{
    help();

    utils::ConsolePrinter printer;
    utils::ConsoleInputter inputter;
    coreInit(printer, inputter);

    coreRegisterBreakpointHitCallback(breakpointHitCallback);

    for(int i = 1; i < argc; i += 1) {
        coreLoadSimulatorWithFile(std::string(argv[i]));
    }

    while(prompt()) {}

    return 0;
}

void help(void)
{
    std::cout << "break <action> [args...] - performs action (see break help for details)\n"
              << "help                     - display this message\n"
              << "list                     - display the next instruction to be executed\n"
              << "load filename            - loads an object file\n"
              << "mem <start> <end>        - display values in memory addresses start to end\n"
              << "quit                     - exit the simulator\n"
              << "regs                     - display register values\n"
              << "run [instructions]       - runs to end of program or, if specified, the number of instructions\n"
              << "set <loc> <value>        - sets loc (either register name or memoery address) to value\n"
              ;
}

void breakHelp(void)
{
    std::cout << "break clear <id>  - clears the given breakpoint\n"
              << "break help        - display this message\n"
              << "break list        - display the active breakpoints\n"
              << "break set <loc>   - sets a breakpoint at the given location\n"
              ;
}

bool prompt(void)
{
    std::cout << "> ";
    std::string command_line;
    std::getline(std::cin, command_line);
    std::stringstream command_tokens(command_line);

    return promptMain(command_tokens);
}

bool promptMain(std::stringstream & command_tokens)
{
    std::string command;
    command_tokens >> command;

    if(command == "break") {
        promptBreak(command_tokens);
    } else if(command == "help") {
        help();
    } else if(command == "list") {
        uint32_t pc = coreGetPC();
        for(int32_t pos = -2; pos <= 2; pos += 1) {
            if(((int32_t) pc) + pos > 0) {
                if(pos == 0) {
                    std::cout << "--> ";
                } else {
                    std::cout << "    ";
                }
                std::cout << formatMem(pc + pos) << "\n";
            }
        }
    } else if(command == "mem") {
        std::string start_s, end_s;
        command_tokens >> start_s >> end_s;
        if(command_tokens.fail()) {
            std::cout << "must supply start and end addresses\n";
            return true;
        }

        uint32_t start, end;
        try {
            start = std::stoi(start_s, 0, 0);
            end = std::stoi(end_s, 0, 0);
        } catch(std::exception const & e) {
            std::cout << "invalid value\n";
            return true;
        }

        for(uint32_t addr = start; addr <= end; addr += 1) {
            if(addr < 0xffff) {
                std::cout << formatMem(addr) << "\n";
            }
        }
    } else if(command == "load") {
        std::string filename;
        command_tokens >> filename;
        if(command_tokens.fail()) {
            std::cout << "must supply filename argument to load\n";
            return true;
        }

        coreLoadSimulatorWithFile(filename);
    } else if(command == "quit") {
        return false;
    } else if(command == "regs") {
        for(uint32_t i = 0; i < 2; i += 1) {
            for(uint32_t j = 0; j < 4; j += 1) {
                uint32_t reg = i * 4 + j;
                uint32_t value = coreGetReg(reg);
                std::cout << utils::ssprintf("R%u: 0x%0.4X (%5d)", reg, value, value);
                if(j != 3) {
                    std::cout << "    ";
                }
            }
            std::cout << "\n";
        }
        std::cout << utils::ssprintf("PC: 0x%0.4X\n", coreGetPC());
        std::cout << utils::ssprintf("PSR: 0x%0.4X\n", coreGetPSR());
        std::cout << utils::ssprintf("CC: %c\n", coreGetCC());
    } else if(command == "run") {
        uint32_t inst_count;
        command_tokens >> inst_count;
        if(command_tokens.fail()) {
            auto start_count = inst_exec_count;
            auto start_time = std::chrono::steady_clock::now();
            coreRun();
            auto end_time = std::chrono::steady_clock::now();
            auto end_count = inst_exec_count;
            std::cout << "Executed " << (end_count - start_count) << " instructions in " << std::chrono::duration<double, std::milli>(end_time - start_time).count() << " ms\n";
        } else {
            coreRunFor(inst_count);
        }
    } else if(command == "set") {
        std::string loc_s, val_s;
        command_tokens >> loc_s >> val_s;
        if(command_tokens.fail()) {
            std::cout << "must supply location and value\n";
            return true;
        }

        uint32_t val;
        try {
            val = std::stoi(val_s, 0, 0);
        } catch(std::exception const & e) {
            std::cout << "invalid value\n";
            return true;
        }

        std::transform(loc_s.begin(), loc_s.end(), loc_s.begin(), ::tolower);
        if(loc_s[0] == 'r') {
            if(loc_s.size() != 2 || loc_s[1] > '7') {
                std::cout << "invalid register\n";
                return true;
            }
            uint32_t id = loc_s[1] - '0';
            coreSetReg(id, val);
        } else if(loc_s == "pc") {
            coreSetPC(val);
        } else if(loc_s == "psr") {
            coreSetPSR(val);
        } else {
            std::cout << "invalid location\n";
        }
    } else {
        std::cout << "unknown command\n";
    }

    return true;
}

void promptBreak(std::stringstream & command_tokens)
{
    std::string command;
    command_tokens >> command;
    if(command_tokens.fail()) {
        std::cout << "must provide action\n";
        return;
    }

    if(command == "clear") {
        uint32_t id;
        command_tokens >> id;
        if(command_tokens.fail()) {
            std::cout << "must supply id\n";
            return;
        }

        bool removed = coreRemoveBreakpoint(id);
        if(! removed) {
            std::cout << "invalid id\n";
            return;
        }
    } else if(command == "help") {
        breakHelp();
    } else if(command == "list") {
        for(auto x : breakpoints) {
            std::cout << x << "\n";
        }
    } else if(command == "set") {
        std::string loc_s;
        command_tokens >> loc_s;
        if(command_tokens.fail()) {
            std::cout << "must supply location\n";
            return;
        }

        uint32_t loc;
        try {
            loc = std::stoi(loc_s, 0, 0);
        } catch(std::exception const & e) {
            std::cout << "invalid value\n";
            return;
        }

        Breakpoint bp = coreSetBreakpoint(loc);
        std::cout << bp << "\n";

    } else  {
        std::cout << "unknown command\n";
    }
}

std::string formatMem(uint32_t addr)
{
    std::stringstream out;
    uint32_t value = coreGetMemVal(addr);
    std::string line = coreGetMemLine(addr);
    out << utils::ssprintf("0x%0.4X: 0x%0.4X %s", addr, value, line.c_str());
    return out.str();
}

std::ostream & operator<<(std::ostream & out, Breakpoint const & x)
{
    out << "#" << x.id << ": " << formatMem(x.loc);
    return out;
}

void breakpointHitCallback(core::MachineState & state, Breakpoint const & bp)
{
    (void) state;
    std::cout << "hit a breakpoint\n" << bp << "\n";
}
