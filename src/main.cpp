#include <iostream>
#include <string>
#include <cstring>
#include "mmu.h"
#include "pagetable.h"
#include <vector>

//using namespace std;

void printStartMessage(int page_size);
void createProcess(int text_size, int data_size, Mmu *mmu, PageTable *page_table);
void allocateVariable(uint32_t pid, std::string var_name, DataType type, uint32_t num_elements, Mmu *mmu, PageTable *page_table);
void setVariable(uint32_t pid, std::string var_name, uint32_t offset, void *value, Mmu *mmu, PageTable *page_table, void *memory);
void freeVariable(uint32_t pid, std::string var_name, Mmu *mmu, PageTable *page_table);
void terminateProcess(uint32_t pid, Mmu *mmu, PageTable *page_table);

int main(int argc, char **argv)
{
    // Ensure user specified page size as a command line parameter
    if (argc < 2)
    {
        fprintf(stderr, "Error: you must specify the page size\n");
        return 1;
    }

    // Print opening instuction message
    int page_size = std::stoi(argv[1]);
    printStartMessage(page_size);

    // Create physical 'memory'
    uint32_t mem_size = 67108864;
    void *memory = malloc(mem_size); // 64 MB (64 * 1024 * 1024)

    // Create MMU and Page Table
    Mmu *mmu = new Mmu(mem_size);
    PageTable *page_table = new PageTable(page_size);
    Process p1;
    //string var_name;
    //string var_data_type;

    // Prompt loop
    std::string command;
    std::string var_name;
    std::string print_object;
    //char delimiter = ' ';
    //uint8_t var_data_type;
    //uint32_t allocate_num_elements;
    uint32_t offset;
    //void* value;
    std::cout << "> ";
    std::getline (std::cin, command);
    while (command != "exit") {
        // Handle command (create, allocate, set, free, terminate, print)
        // TODO: implement this!
        
        if(command == "create"){

            createProcess(mem_size, page_size, mmu, page_table);
        }

        // Get next command
        std::cout << "> ";
        std::getline (std::cin, command);


        /*
        else if(command == "allocate"){  

            //string var_name;
            //string var_data_type

            std::cout << "what is the variable name? ";
            std::cin >> var_name;
            std::cout << "what is the variable's data type? ";
            std::cin >> var_data_type;
            std::cout << "how many elements to allocate? ";
            std::cin >> allocate_num_elements;

            allocateVariable(p1.pid, var_name, (DataType)var_data_type, allocate_num_elements, mmu, page_table);
            
            //std::cout << var_name + var_data_type;
        }
        else if(command == "set"){
            
            std::cout << "what is the variable name? ";
            std::cin >> var_name;
            std::cout << "what is the offset? ";
            std::cin >> offset;
            std::cout << "what is the value? ";
            std::cin >> value;


            setVariable(p1.pid, var_name, offset, value, mmu, page_table, memory);

        }else if(command == "free"){
            
            std::cout << "what is the variable name? ";
            std::cin >> var_name;

            freeVariable(p1.pid, var_name, mmu, page_table);

        }else if(command == "terminate"){

            terminateProcess(p1.pid, mmu, page_table);
            

        }else if(command == "print"){

            std::cout << "What object do you want to print? ";
            std::cin >> print_object;

            if(print_object == "mmu"){

            }else if(print_object == "page"){
                
            }else if(print_object == "processes"){
                
            }else if(print_object == "pid"){
                
            }else{

                std::cout << "please try again and enter a real object to be printed ";
            }

        
        }else{

            std::cout << "please try again and enter a valid command";
        }*/

       
    }

    // Clean up
    free(memory);
    delete mmu;
    delete page_table;

    return 0;
}

void printStartMessage(int page_size)
{
    std::cout << "Welcome to the Memory Allocation Simulator! Using a page size of " << page_size << " bytes." << std:: endl;
    std::cout << "Commands:" << std:: endl;
    std::cout << "  * create <text_size> <data_size> (initializes a new process)" << std:: endl;
    std::cout << "  * allocate <PID> <var_name> <data_type> <number_of_elements> (allocated memory on the heap)" << std:: endl;
    std::cout << "  * set <PID> <var_name> <offset> <value_0> <value_1> <value_2> ... <value_N> (set the value for a variable)" << std:: endl;
    std::cout << "  * free <PID> <var_name> (deallocate memory on the heap that is associated with <var_name>)" << std:: endl;
    std::cout << "  * terminate <PID> (kill the specified process)" << std:: endl;
    std::cout << "  * print <object> (prints data)" << std:: endl;
    std::cout << "    * If <object> is \"mmu\", print the MMU memory table" << std:: endl;
    std::cout << "    * if <object> is \"page\", print the page table" << std:: endl;
    std::cout << "    * if <object> is \"processes\", print a list of PIDs for processes that are still running" << std:: endl;
    std::cout << "    * if <object> is a \"<PID>:<var_name>\", print the value of the variable for that process" << std:: endl;
    std::cout << std::endl;
}

void createProcess(int text_size, int data_size, Mmu *mmu, PageTable *page_table)
{
    // TODO: implement this!
    //   - create new process in the MMU
    //   - allocate new variables for the <TEXT>, <GLOBALS>, and <STACK>
    //   - print pid


    mmu->createProcess();

    // not sure on this - 04/15/2021
    //int *text_size_mem_alloc = malloc(sizeof(text_size));
    //int *globals_mem_alloc = malloc();
    //int *stack_mem_alloc = malloc();

    //printf("%lu \n", );
}

void allocateVariable(uint32_t pid, std::string var_name, DataType type, uint32_t num_elements, Mmu *mmu, PageTable *page_table)
{
    // TODO: implement this!
    //   - find first free space within a page already allocated to this process that is large enough to fit the new variable
    //   - if no hole is large enough, allocate new page(s)
    //   - insert variable into MMU
    //   - print virtual memory address 
}

void setVariable(uint32_t pid, std::string var_name, uint32_t offset, void *value, Mmu *mmu, PageTable *page_table, void *memory)
{
    // TODO: implement this!
    //   - look up physical address for variable based on its virtual address / offset
    //   - insert `value` into `memory` at physical address
    //   * note: this function only handles a single element (i.e. you'll need to call this within a loop when setting
    //           multiple elements of an array) 
}

void freeVariable(uint32_t pid, std::string var_name, Mmu *mmu, PageTable *page_table)
{
    // TODO: implement this!
    //   - remove entry from MMU
    //   - free page if this variable was the only one on a given page
}

void terminateProcess(uint32_t pid, Mmu *mmu, PageTable *page_table)
{
    // TODO: implement this!
    //   - remove process from MMU
    //   - free all pages associated with given process

    
    
}
