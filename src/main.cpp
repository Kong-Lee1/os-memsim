#include <iostream>
#include <string>
#include <cstring>
#include "mmu.h"
#include "pagetable.h"
#include <vector>
#include <sstream>
#include <math.h>

//using namespace std;

void printStartMessage(int page_size);
void createProcess(int text_size, int data_size, Mmu *mmu, PageTable *page_table);
void allocateVariable(uint32_t pid, std::string var_name, DataType type, uint32_t num_elements, Mmu *mmu, PageTable *page_table);
void setVariable(uint32_t pid, std::string var_name, uint32_t offset, void *value, Mmu *mmu, PageTable *page_table, void *memory);
void freeVariable(uint32_t pid, std::string var_name, Mmu *mmu, PageTable *page_table);
void terminateProcess(uint32_t pid, Mmu *mmu, PageTable *page_table);
bool powerOfTwo(uint32_t byteSize);
uint32_t findPageOffset(uint32_t page_size);

int main(int argc, char **argv)
{
    // Ensure user specified page size as a command line parameter
    if (argc < 2)
    {
        fprintf(stderr, "Error: you must specify the page size\n");
        return 1;
    }

    // Print opening instuction message
    if (!powerOfTwo((uint32_t)std::stoi(argv[1]))) {
	printf("Unacceptable page size.\n");
	return 0;
    }
    if (std::stoi(argv[1]) > 32768 || std::stoi(argv[1]) < 1024) {
	printf("Unacceptable page size.\n");
	return 0;
    }
    uint32_t page_size = (uint32_t)std::stoi(argv[1]);
    uint32_t page_offset = findPageOffset(page_size);
    uint32_t page_numbers = 32 - page_offset; 
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
    uint32_t pid;
    int processCreatorCounter = 0;
    std::string print_object;
    uint8_t var_data_type;
    uint32_t allocate_num_elements;
    uint32_t offset;
    uint32_t text_size;
    uint32_t data_size;
    void* value;
    std::cout << "> ";
    std::getline (std::cin, command);
    while (command != "exit") {
	std::string arguments[50];
	int arg_num = 0;
        std::stringstream ss(command);
	std::string word;
	while (ss >> word) {
		arguments[arg_num] = word;
		arg_num++;
	}
        // Handle command (create, allocate, set, free, terminate, print)
        // TODO: implement this!
        if(arguments[0] == "create"){ 
	        text_size = (uint32_t)std::stoi(arguments[1]);
            data_size = (uint32_t)std::stoi(arguments[2]);
	    if ( !powerOfTwo(text_size) ) {
	    	printf("text_size is not a power of two, please try again.\n");
	    } else if ( !powerOfTwo(data_size) ) {
		    printf("data_size is not a power of two, please try again.\n");
	    } else if (text_size < 2048 || text_size > 16384) {
		    printf("text_size is not within range: 2048-16384, please try again.\n");
	    } else if (data_size < 2048 || data_size > 16384) {
		    printf("data_size is not within range: 2048-16384, please try again.\n");
	    } else {
            createProcess(text_size, data_size, mmu, page_table);
            processCreatorCounter++;
	    }
        } else if(arguments[0] == "allocate"){  
	        pid = (uint32_t)std::stoi(arguments[1]);
	        var_name = arguments[2];
	    if (arguments[3] == "int" || arguments[3] == "float") {
		    var_data_type = 3U;
	    } else if (arguments[3] == "char") {
		    var_data_type = 1U;
	    } else if (arguments[3] == "short") {
		    var_data_type = 2U;
	    } else if (arguments[3] == "double" || arguments[3] == "long") {
		    var_data_type = 4U;
	    } else {
		    printf("Data type not recognized. Must be char, short, int/float, long/double; please try again.\n");
		    var_data_type = 00;
	    }
	        allocate_num_elements = (uint32_t)std::stoi(arguments[4]);
	    if (var_data_type != 00) {
            	allocateVariable(pid, var_name, (DataType)var_data_type, allocate_num_elements, mmu, page_table);
            }
        } else if(arguments[0] == "set"){
            /*
    

            set <PID> <var_name> <offset> <value_0> <value_1> <value_2> ... <value_N>
            Set the value for variable <var_name> starting at <offset>
            Note: multiple contiguous values can be set with one command
            */
        
           // need to check if user enter PID is valid
           // maybe have a curPID variable?

           pid = (uint32_t)std::stoi(arguments[1]);
           if(pid < 1024 || pid > (1024 + processCreatorCounter)){

               std::cout << "pid is invalid, enter a valid pid \n";
               //std::cout << mmu->createProcess() << "\n";
           }

           //need to check that the variable exists in the specified process
           
            

            //setVariable(arguments[1], var_name, offset, value, mmu, page_table, memory);
		
        } else if(arguments[0] == "free"){
            /*
  

            freeVariable(p1.pid, var_name, mmu, page_table);
	        */
        } else if(arguments[0] == "terminate"){
            /*
            terminateProcess(p1.pid, mmu, page_table);
            
            */
        } else if(arguments[0] == "print"){

            print_object = arguments[1];

            if(print_object == "mmu"){
                
                //prints the mmu table
                mmu->print();
               
            } else if(print_object == "page"){
                
                //prints the page table
                page_table->print();
                
            } /* else if(print_object == "processes"){
                
            } else if(print_object == "pid"){
                
            } else {

                std::cout << "please try again and enter a real object to be printed ";
            }

  */      
        } else{
            std::cout << "please try again and enter a valid command";
        }
	std::cout << "> ";
	std::getline(std::cin, command);
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
    printf("%u", mmu->createProcess());
    printf("\n");
    //not sure on this - 04/15/2021
    //int *text_size_mem_alloc = malloc(sizeof(text_size));
    //int *globals_mem_alloc = malloc();
    //int *stack_mem_alloc = malloc();
    //printf("test");
    //printf("%lu \n", );
}

void allocateVariable(uint32_t pid, std::string var_name, DataType type, uint32_t num_elements, Mmu *mmu, PageTable *page_table)
{
    // TODO: implement this!
    //   - find first free space within a page already allocated to this process that is large enough to fit the new variable
    //   - if no hole is large enough, allocate new page(s)
    //   - insert variable into MMU
    //   - print virtual memory address 

    page_table->getPhysicalAddress(pid, pid);


    //page_table->getPhysicalAddress(pid, );
    
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
bool powerOfTwo(uint32_t byteSize)
{
	uint32_t n;
	n = byteSize;
	if(n==0) { return false; }
	while(n!=1) {
		n = n/2;
		if(n%2 != 0 && n != 1) {return false; }
	}
	return true;
}
uint32_t findPageOffset(uint32_t page_size) {
	return log2(page_size);
}
