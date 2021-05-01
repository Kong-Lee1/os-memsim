#ifndef __MMU_H_
#define __MMU_H_

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

enum DataType : uint8_t {FreeSpace, Char, Short, Int, Float, Long, Double};

typedef struct Variable {
    std::string name;
    DataType type;
    uint32_t virtual_address;
    uint32_t size;
} Variable;

typedef struct Process {
    uint32_t pid;
    std::vector<Variable*> variables;
} Process;

class Mmu {
private:
    uint32_t _next_pid;
    uint32_t _max_size;
    std::vector<Process*> _processes;

public:
    Mmu(int memory_size);
    ~Mmu();

    uint32_t createProcess();
    void addVariableToProcess(uint32_t pid, std::string var_name, DataType type, uint32_t size, uint32_t address);
    void print();
    std::vector<Process *> getProcessVector();
    uint32_t getVirtualAddress(int pid_index, int variable_index);
    int getVariableIndex(int pid_index, std::string var_name);
    DataType getVariableType(int pid_index, int variable_index);
    int getVariableVecSize(int pid_index);
    int  getVariableSize(int pid_index, int variable_index);
    uint32_t getMemorySize();
    std::string decimalToBinary(int decimal);
    bool deleteProcess (int pid_index);
    bool deleteVariable (int pid_index, std::string var_name);
    bool setAddress(int pid_index, std::string var_name, uint32_t new_virtual_address);
    bool checkValidPID(uint32_t pid);
    bool checkValidVarName(uint32_t pid, std::string var_name);
    
};

#endif // __MMU_H_
