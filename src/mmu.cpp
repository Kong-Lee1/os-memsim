#include "mmu.h"
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <algorithm>

Mmu::Mmu(int memory_size)
{
    _next_pid = 1024;
    _max_size = memory_size;
}

Mmu::~Mmu()
{
}

uint32_t Mmu::createProcess()
{
    Process *proc = new Process();
    proc->pid = _next_pid;

    Variable *var = new Variable();
    var->name = "<FREE_SPACE>";
    var->type = DataType::FreeSpace;
    var->virtual_address = 0;//does this change for each created process?
    var->size = _max_size;
    proc->variables.push_back(var);

    _processes.push_back(proc);

    _next_pid++;
    return proc->pid;
}

void Mmu::addVariableToProcess(uint32_t pid, std::string var_name, DataType type, uint32_t size, uint32_t address)
{
    int i;
    Process *proc = NULL;
    for (i = 0; i < _processes.size(); i++)
    {
        if (_processes[i]->pid == pid)
        {
            proc = _processes[i];
        }
    }

    Variable *var = new Variable();
    var->name = var_name;
    var->type = type;
    var->virtual_address = address;
    var->size = size;
    if (proc != NULL)
    {
        proc->variables.push_back(var);
    }
}

void Mmu::print()
{
    int i, j;
    int space_counter;



    std::cout << " PID  | Variable Name | Virtual Addr | Size" << std::endl;
    std::cout << "------+---------------+--------------+------------" << std::endl;
    for (i = 0; i < _processes.size(); i++)
    {
        for (j = 0; j < _processes[i]->variables.size(); j++)
        {
            // TODO: print all variables (excluding <FREE_SPACE> entries)
                std::string name = _processes[i]->variables[j]->name;
                //std::stringstream ss = (int)_processes[i]->variables[j]->virtual_address;

                std::string virtual_addr;// = std::to_string(_processes[i]->variables[j]->virtual_address);

            if(name.length() >= 15){

                name = name.substr(0, 15);

            }else{

                while(name.length() < 15){
                    name = name + " ";
                }
            }

            virtual_addr = decimalToBinary(_processes[i]->variables[j]->virtual_address);

            if(virtual_addr.length() >= 8){

                virtual_addr = virtual_addr.substr(0, 8);

            }else{

                while(virtual_addr.length() < 8){
                    virtual_addr = "0" + virtual_addr;
                }
            }
            
            virtual_addr = "0x" + virtual_addr;

           std::string size = std::to_string(_processes[i]->variables[j]->size);

            if((size.length() >= 11)){

                size = size.substr(0, 11);

            }else{

                while(size.length() < 11){
                    size = " " + size;
                }
            }


            std::cout << _processes[i]->pid << "  |" << name << "|  " << virtual_addr << "  |" << size << "\n";
            //std::cout << _processes[i]->variables[j]->name;
            //std::cout << _processes[i]->variables[j]->virtual_address;
            //std::cout << _processes[i]->variables[j]->size;
        }
    }
}

uint32_t Mmu::getVirtualAddress(int pid_index, int variable_index){

    return _processes[pid_index]->variables[variable_index]->virtual_address;
}

int Mmu::getVariableIndex(int pid_index, std::string var_name){

    int variableIndex = -1;

    for (int j = 0; j < _processes[pid_index]->variables.size(); j++)
    {

        if(_processes[pid_index]->variables[j]->name == var_name){

            variableIndex = j;
        }

    }
    
    
    return variableIndex;
}

DataType Mmu::getVariableType(int pid_index, int variable_index){



    return _processes[pid_index]->variables[variable_index]->type;

}

int Mmu:: getVariableVecSize(int pid_index){

    return _processes[pid_index]->variables.size();
}

int Mmu:: getVariableSize(int pid_index, int variable_index){

    return _processes[pid_index]->variables[variable_index]->size;
}

uint32_t Mmu:: getMemorySize(){

    return _max_size;
}

std::vector<Process *> Mmu:: getProcessVector(){

    return _processes;
}

std::string Mmu:: decimalToBinary(int decimal)
{

    std::stringstream sstream;
    sstream << std::hex << decimal;
    std::string result = sstream.str();
    

    std::transform(result.begin(), result.end(), result.begin(), ::toupper);

    return result;

}


/*
int Mmu:: getVariablePIDVirtualAddress(int pid_index, int variable_index){

    return _processes[pid_index]->variables[variable_index]->virtual_address;
}*/


//std::vector<Process*> getProcessesVec(std::vector<Process*> _Processes, uint32_t *pid){

    //return _Processes.erase();
//}
