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
uint32_t handleMemory(std::string command, uint32_t memory_copy, uint32_t pid, DataType type, std::string var_name, int num_elements, int text_size, int data_size);
int getPageNumber(uint32_t address, int page_size);

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
    uint32_t mem_size_copy = mem_size;
    void *memory = malloc(mem_size); // 64 MB (64 * 1024 * 1024)

    // Create MMU and Page Table
    Mmu *mmu = new Mmu(mem_size);
    PageTable *page_table = new PageTable(page_size);
    Process p1;


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
    std::vector<Process*> process_vector;
    while (command != "exit") {
	std::string arguments[50];
	int arg_num = 0;
    int set_num_args = 0;
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

        if (text_size < 2048 || text_size > 16384) {
		    printf("text_size is not within range: 2048-16384, please try again.\n");
	    } else if (data_size < 0 || data_size > 1024) {
		    printf("data_size is not within range: 2048-16384, please try again.\n");
	    } else {
            createProcess(text_size, data_size, mmu, page_table);
            processCreatorCounter++;
            process_vector = mmu->getProcessVector();
	    }
        } else if(arguments[0] == "allocate"){  
	        pid = (uint32_t)std::stoi(arguments[1]);

            bool validPID = mmu->checkValidPID(pid);

            

            if(validPID == false){

                printf(" error: process not found \n");
                break;
            }

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

            if (var_data_type != 00)
            {
                allocateVariable(pid, var_name, (DataType)var_data_type, allocate_num_elements, mmu, page_table);
            }
                //also need to handle if allocation would exceed system memory -- this would happen in main?
                //printf("%s error: allocation would exceed system memory \n")

            //if(allocate_num_elements > 1){

            //}

        } else if(arguments[0] == "set"){
            /*
    

            set <PID> <var_name> <offset> <value_0> <value_1> <value_2> ... <value_N>
            Set the value for variable <var_name> starting at <offset>
            Note: multiple contiguous values can be set with one command
            */
        
           // need to check if user enter PID is valid
           // maybe have a curPID variable?

            //std::cout << "got 0 here!!" << "\n"; 
           pid = (uint32_t)std::stoi(arguments[1]);
           //std::cout << "pid is " << pid << "\n"; 
           //std::cout << "got 1 here!!" << "\n"; 
            var_name = arguments[2];
            
            //std::cout << "varible name is " << var_name << "\n";

           bool validPID = mmu->checkValidPID(pid);
           
        
            if(validPID == false){

                printf(" error: process not found \n");
                //break;//need to figure out how to loop through instead of getting seg faults
            }

            bool validVarName = mmu->checkValidVarName(pid, var_name);

            if(validVarName == false){

                printf(" error: variable does not exist \n");
                //break;//need to figure out how to loop through instead of getting seg faults
            }
           
           offset = (uint32_t)std::stoi(arguments[3]);

           int empty_index_holder;

            //we need keep track of the number of n-values provided
            for(int i = 0; i < sizeof(arguments); i++){

                if(arguments[i] == ""){
                    empty_index_holder = i;
                    break;
                }
                
            }

            //std::cout << "got 2 here!!" << "\n"; 

            set_num_args = empty_index_holder - 4;

            //do we need to handle data type when storing n-values into memory/array???
            //probably don't need this anymore since we check pid within each function we call.
            
           /*if(pid < 1024 || pid > (1024 + processCreatorCounter)){

               std::cout << "pid is invalid, enter a valid pid \n";
               std::cout << mmu->createProcess() << "\n";
           }*/


            //std::cout << "got here!!"; 
            for(int i = 0; i < set_num_args - 1; i++){

                std::string value = arguments[4+i];
                setVariable(pid, var_name, offset+i*data_size, &value, mmu, page_table, memory);
            }
		
        } else if(arguments[0] == "free"){

            pid = (uint32_t)std::stoi(arguments[1]);
            var_name = arguments[2];
            
            bool validPID = mmu->checkValidPID(pid);
           
            

            if(validPID == false){

                printf(" error: process not found \n");
                //break;//need to figure out how to loop through instead of getting seg faults
            }

            bool validVarName = mmu->checkValidVarName(pid, var_name);

            if(validVarName == true){

                printf(" error: variable already exists \n");
                //break;//need to figure out how to loop through instead of getting seg faults
                
            }else{

                 freeVariable(pid, var_name, mmu, page_table);
            }
            
           
	        
        } else if(arguments[0] == "terminate"){

             pid = (uint32_t)std::stoi(arguments[1]);

             bool validPID = mmu->checkValidPID(pid);

            if(validPID == false){

                printf(" error: process not found \n");
                //break;//need to figure out how to loop through instead of getting seg faults
            }else{

                terminateProcess(pid, mmu, page_table);
            }
            
            
            
        } else if(arguments[0] == "print"){

            print_object = arguments[1];

            if(print_object == "mmu"){
                
                //prints the mmu table
                mmu->print();
               
            } else if(print_object == "page"){
                
                //prints the page table
                page_table->print();
                
            }  else if(print_object == "processes"){
                         
                for(int i = 0; i < process_vector.size(); i++){

                    printf("%u \n", process_vector[i]->pid);
                }

            } else {
                
                //
                size_t sep1 = print_object.find(":");
                std::uint32_t object_pid = stoi(print_object.substr(0, sep1));
                std::string object_var_name = print_object.substr(sep1+1, print_object.size());
                //std::cout << "object var name is " << object_var_name << "\n";
                DataType var_name_datatype;
                int type_size = -1;

                //std::cout << "1" << "\n";

                //char, int, long

                char char_variable_datatype;
                int int_variable_datatype;
                long long_variable_datatype;
                short short_variable_datatype;

                for(int i = 0; i < process_vector.size(); i++){
                    //std::cout << "2" << "\n";

                    if(process_vector[i]->pid == object_pid){

                        //std::cout << "3" << "\n";

                        for(int j = 0; j < process_vector[i]->variables.size(); j++)
                        {
                            //std::cout << "4" << "\n";

                            if(process_vector[i]->variables[j]->name == object_var_name){
                                //std::cout << "5" << "\n";

                                var_name_datatype = process_vector[i]->variables[j]->type;
                                
                                //char
                                 if(var_name_datatype == 1U){
                                    char_variable_datatype = 1U;
                                    type_size = 1;

                                //short
                                }else if (var_name_datatype == 2U){
                                    short_variable_datatype = 2U;
                                    type_size = 2;

                                //int
                                }else if(var_name_datatype == 3U){
                                    int_variable_datatype = 3U;
                                    type_size = 4;

                                //long
                                }else if(var_name_datatype == 4U){
                                    long_variable_datatype = 4U;
                                    type_size = 8;

                                }

                                //std::cout << "6" << "\n"; 
                                int num_elements_of_variable = process_vector[i]->variables[j]->size/type_size;
                                int page_num = floor(mmu->getVirtualAddress(i, j)/page_size);
                                uint32_t variable_physical_address = page_table->getPhysicalAddress(process_vector[i]->pid, process_vector[i]->variables[j]->virtual_address);//this is incorrect as it is currently 0
                                void* ptr_variable_physical_address = &variable_physical_address;//want to pass this as a void pointer but it is not currently working
                                
                                //std::cout << "the virtual address is " << process_vector[i]->variables[j]->virtual_address << "\n" << std::endl;
                                //std::cout << "variable physical address is " << variable_physical_address << std::endl;

                                std::string filled_str;

                                if(var_name_datatype == 1U)
                                {
                                    //std::cout << "7" << "\n";

                                    if(num_elements_of_variable > 4)
                                    {
                                        //std::cout << "8" << "\n";

                                        for(int k = 0; k < 4; k++){

                                            //std::cout << "9" << "\n";

                                            memcpy(&char_variable_datatype, (uint8_t*)memory+variable_physical_address+k, 1);

                                            //filled_str = &process_vector[i]->variables[j]->virtual_address+type_size*k;
                                            
                                            if(k == 3){

                                                //std::cout << "10" << "\n";
                                                
                                                filled_str.push_back(char_variable_datatype);

                                                filled_str = filled_str  + ", ...";
                                            
                                            }else{

                                                filled_str.push_back(char_variable_datatype);

                                                filled_str = filled_str + ", ";

                                                //std::cout << "11" << "\n";
                                            }

                                        }
                                            //std::cout << filled_str << "\n" << std::endl;
                                            //std::cout << "12" << "\n";
                                    }else{
                                            //std::cout << "everything " << (uint8_t*)memory+variable_physical_address << std::endl;
                                            //std::cout << "13" << "\n";

                                        for(int k = 0; k < num_elements_of_variable - 1; k++){
                                            
                                            //std::cout << "14" << "\n";

                                            //std::cout << &process_vector[i]->variables[j]->virtual_address+type_size*k;
                                            
                                            memcpy(&char_variable_datatype, (uint8_t*)memory+variable_physical_address+k, 1);

                                                if(k == num_elements_of_variable - 1){

                                                    std::cout << "15" << "\n";
                                                    filled_str.push_back(char_variable_datatype);

                                                    filled_str = filled_str + "***";
                                                    std::cout << "char data type is " << char_variable_datatype << "\n";

                                                }else{
                                                    
                                                    std::cout << "16" << "\n";
                                                    filled_str.push_back(char_variable_datatype);

                                                    filled_str = filled_str + ", ";

                                                }
                                                
                                        }
                                        std::cout << "17" << "\n";
                                        std::cout << filled_str << "\n" << std::endl;
                                }

                            }

                            }
                        }

                    }   
                }
                
                
                }/*if(print_object == "pid"){
                // print 1024:page
                //If <object> is a "<PID>:<var_name>", print the value of the variable for that process

                uint32_t object_pid = 


            } *//*else {

                std::cout << "please try again and enter a real object to be printed ";
            }*/

       
        } else{
            //error check command if command if not 'create', 'allocate', and 'set', 'print', 'free', 'terminate' or 'exit'
            std::cout << "error: command not recognized" << "\n" << std::endl;
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

    uint32_t memory_size = mmu->getMemorySize();
    uint32_t page_size = page_table->getPageSize();
    uint32_t pid = mmu->createProcess();

    int pid_index = pid - 1024;
    printf("%u", pid);
    printf("\n");
    //std::cout << "allocated nothing";

    uint8_t dataType = 1U;
    //std::cout << "allocated nothing";

    uint32_t total_number_pages_to_add = ceil((text_size + data_size + 65536)/page_size);
    uint32_t new_virtual_address = text_size + data_size + 65536;

    std::string var_name = "<FREE_SPACE>";

    mmu->setAddress(pid_index, var_name, new_virtual_address);

    //std::cout << "total number of pages to add is " << total_number_pages_to_add << std::endl;

    for(int i = 0; i < total_number_pages_to_add; i++){

        page_table->addEntry(pid, i);
    }
    
    //allocateVariable(pid, "TEXT", (DataType)dataType, text_size, mmu, page_table);
    mmu->addVariableToProcess(pid, "TEXT", (DataType)dataType, text_size, 0);
    //mmu->addVariableToProcess(pid, "TEXT", (DataType)dataType, text_size, 0);
    
    
    //allocateVariable(pid, "GLOBALS", (DataType)dataType, data_size, mmu, page_table);
    mmu->addVariableToProcess(pid, "GLOBALS", (DataType)dataType, data_size, text_size);
    
    //allocateVariable(pid, "STACK", (DataType)dataType, 65536, mmu, page_table);
    mmu->addVariableToProcess(pid, "STACK", (DataType)dataType, 65536, data_size+text_size);
    

    
}

void allocateVariable(uint32_t pid, std::string var_name, DataType type, uint32_t num_elements, Mmu *mmu, PageTable *page_table)
{
    //allocate <PID> <var_name> <data_type> <number_of_elements> (allocated memory on the heap)

    // TODO: implement this!
    //   - find first free space within a page already allocated to this process that is large enough to fit the new variable
    //   - if no hole is large enough, allocate new page(s)
    //   - insert variable into MMU
    //   - print virtual memory address 


    int pid_index = pid - 1024;
    int variable_index = mmu->getVariableIndex(pid_index, var_name);
    int type_size = -1;
    int var_type = -1;
    int page_break = -1;
    int page_start;

    bool validPID;

    std::vector<Process *> processVec = mmu->getProcessVector();

    uint32_t memory_size = mmu->getMemorySize();
    uint32_t page_size = page_table->getPageSize();
    uint32_t virtual_addr;

    uint32_t new_virtual_address;
    std::string new_var_name = "<FREE_SPACE>";


    //checks for exisiting variable in the process vector
    for(int i = 0; i < processVec.size()-1; i++){
        
        for(int j = 0; j < processVec[i]->variables.size()-1; j++ ){

            //std::cout << "011 \n";
            if(processVec[i]->variables[j]->name == var_name){

                printf(" error: variable already exists \n");
            }
        }
    }

    if(type == 1U){

        type_size = 1;

    }else if (type == 2U){
        type_size = 2;

    }else if(type == 3U){

        type_size = 4;

    }else if(type == 4U){

        type_size = 8;

    }else{

        std::cout << "please provide a valid variable data type." << std::endl;
    }

    std::string var_name_holder;

    for(int i = 0; i < mmu->getVariableVecSize(pid_index); i++){

        var_name_holder = processVec[pid_index]->variables[i]->name;
        variable_index = mmu->getVariableIndex(pid_index, var_name_holder);

        
        if(mmu->getVariableType(pid_index, i) == 0U && mmu->getVariableSize(pid_index, variable_index) >= num_elements*type_size)
        {

            //need to implement & check page boundaries

            for(int j = 0; j < memory_size/page_size; j++)//mmu->getMemorySize()/page_table->getPageSize() -> the # of pages we have 
            {
                
                page_start = page_size*j;

                //why are the statements different?
                if(page_start > mmu->getVirtualAddress(pid_index, variable_index) || page_start < mmu->getVirtualAddress(pid_index, variable_index) + num_elements*type_size){
                    
                    
                    page_start = (page_start - mmu->getVirtualAddress(pid_index, variable_index))%type_size;
                   
                    
                    virtual_addr = mmu->getVirtualAddress(pid_index, variable_index) + page_start;



                    break;

                }/*else{

                    //need to allocate a new page if no hole is large enough
                    

                    

                    ///page_table->addEntry(pid, )

                    std::cout << "not enough space, need to allocate new page";
                
                }*/

            }
            
            new_virtual_address = virtual_addr + type_size*num_elements;

            mmu->setAddress(pid_index, new_var_name, new_virtual_address);
            mmu->addVariableToProcess(pid, var_name, type, type_size*num_elements, virtual_addr);

  

                // need to check if the added variable still fits after adjusted page break(s);--04/27/21
            break;
        
        }else{

            //std::cout << "nothing happened" << std::endl;
        }

       //std::cout << "something" << std::endl;

    }

     printf("%d \n ", virtual_addr);

}

void setVariable(uint32_t pid, std::string var_name, uint32_t offset, void *value, Mmu *mmu, PageTable *page_table, void *memory)
{
    // TODO: implement this!
    //   - look up physical address for variable based on its virtual address / offset
    //   - insert `value` into `memory` at physical address
    //   * note: this function only handles a single element (i.e. you'll need to call this within a loop when setting
    //           multiple elements of an array) 

    int pid_index = pid - 1024;
    int variable_index = mmu->getVariableIndex(pid_index, var_name);
    int type_size = -1;
    DataType var_type = mmu->getVariableType(pid_index, variable_index);
    std::vector<Process *> processVec = mmu->getProcessVector();
    

    uint32_t var_virtual_addr = mmu->getVirtualAddress(pid_index, variable_index);
    uint32_t var_physical_addr = page_table->getPhysicalAddress(pid, var_virtual_addr);

    //checks for exisiting process in the process vector
    /*
    for(int i = 0; i < processVec.size(); i++){
        
        if((uint32_t)processVec[i]->pid != pid){

            printf(" error: process not found \n");
        }
    }

    //checks for no matching variable in the process vector
    for(int i = 0; i < processVec.size(); i++){
        
        for(int j = 0; i < processVec[i]->variables.size(); j++ ){

            if(processVec[i]->variables[j]->name != var_name){

                printf(" error: variable not found \n");
            }
        }
    }*/

    if(var_type == 1U){

        type_size = 1;
        memcpy((uint8_t*)memory + var_physical_addr, &value, type_size);

       
    }else if (var_type == 2U){
        type_size = 2;
        memcpy((uint8_t*)memory + var_physical_addr, &value, type_size);

        
    }else if(var_type == 3U){

        type_size = 4;
        memcpy((uint8_t*)memory + var_physical_addr, &value, type_size);

         //(int)*memory[var_physical_addr];// how do we write into memory based on variable data type??

    }else if(var_type == 4U){

        type_size = 8;
        memcpy((uint8_t*)memory + var_physical_addr, &value, type_size);

    }else{

        std::cout << "please provide a valid variable data type." << std::endl;
    }

    //memcpy((uint8_t*)memory + var_physical_addr, &value, type_size);
    


}

void freeVariable(uint32_t pid, std::string var_name, Mmu *mmu, PageTable *page_table)
{
    // TODO: implement this!
    //   - remove entry from MMU
    //   - free page if this variable was the only one on a given page

    //std::cout << "1" << "\n" << std::endl;

    int pid_index = pid - 1024;
    int variable_index = mmu->getVariableIndex(pid_index, var_name);
    //std::cout << "variable index is " << variable_index << std::endl;

    //std::cout << "here " << "\n" << std::endl;


    int var_type_size = -1;
    DataType var_type = mmu->getVariableType(pid_index, variable_index);

    //std::cout << "start" << "\n" << std::endl;


    std::vector<Process *> process_Vec = mmu->getProcessVector();
    std::map<std::string, int> table = page_table->getTable();
    std::vector<std::string> keys = page_table->getKeys();

                //std::cout << "testing" << "\n" << std::endl;


    var_type_size = mmu->getVariableSize(pid_index, variable_index);

                //std::cout << "****" << "\n" << std::endl;


    uint32_t var_virtual_addr = mmu->getVirtualAddress(pid_index, variable_index);

                //std::cout << "0000" << "\n" << std::endl;

    uint32_t var_physical_addr = page_table->getPhysicalAddress(pid, var_virtual_addr);

    //void *var_ptr = malloc();

    //std::cout << "2" << "\n" << std::endl;

    std::vector<Process *> processVec = mmu->getProcessVector();

    //checks for exisiting process in the process vector
    /*
    for(int i = 0; i < processVec.size(); i++){
        
        if((uint32_t)processVec[i]->pid != pid){

            printf(" error: process not found \n");
        }
    }

    //std::cout << "3" << "\n" << std::endl;

    //checks for non-exisiting variable in the process vector
    for(int i = 0; i < processVec.size(); i++){
        
        for(int j = 0; j < processVec[i]->variables.size(); j++ ){

            if(processVec[i]->variables[j]->name != var_name){

                printf(" error: variable not found \n");
            }
        }
    }*/

    //std::cout << "4" << "\n" << std::endl;

    int variable_begin;
    int variable_end;
    int variable_before_end;
    int variable_after_begin;

    for(int i = 0; i < processVec.size(); i++)
    {

        //std::cout << "5" << "\n" << std::endl;
        
        for(int j = 0; j < processVec[i]->variables.size(); j++ )
        {
            //std::cout << "6" << "\n" << std::endl;

            if(processVec[i]->variables[j]->name == var_name){
                    
                    //std::cout << "7" << "\n" << std::endl;

                    variable_begin = processVec[i]->variables[j]->virtual_address;
                    variable_end = variable_begin + processVec[i]->variables[j]->size - 1;

                    variable_begin = getPageNumber(variable_begin, page_table->getPageSize());
                    variable_end = getPageNumber(variable_end, page_table->getPageSize());



                    //std::cout << "8" << "\n" << std::endl;


                if(j > 0){

                    variable_before_end = processVec[i]->variables[j - 1]->virtual_address + processVec[i]->variables[j - 1]->size - 1;
                    variable_before_end = getPageNumber(variable_before_end, page_table->getPageSize());

                    //std::cout << "9" << "\n" << std::endl;
                }

                if(j < processVec[i]->variables[j]->size - 1){
                    
                    variable_after_begin = processVec[i]->variables[j + 1]->virtual_address;
                    variable_after_begin = getPageNumber(variable_after_begin, page_table->getPageSize());

                    //std::cout << "10" << "\n" << std::endl;
                }

                // if the variable is the beginning
                if(j == 0){
                    
                    //std::cout << "11" << "\n" << std::endl;

                    if(variable_begin > 0){
                        
                        //std::cout << "12" << "\n" << std::endl;

                        for(int i = 0; i < variable_begin; i++)
                        {

                            //std::cout << "13" << "\n" << std::endl;

                            uint32_t page_number = i;
                            std::string entry = std::to_string(pid) + "|" + std::to_string(page_number);

                            //table.erase(entry);
                            page_table->deletePage(pid, page_number);

                            
                        }
                    }
                    //this erases that the variable ends on 
                    if(variable_end != variable_after_begin){

                        
                        
                        std::string entry = std::to_string(pid) + "|" + std::to_string(variable_end);

                        //table.erase(entry);
                        page_table->deletePage(pid, variable_end);

                        //std::cout << "14" << "\n" << std::endl;
                    }

                }
                // if the variable is in the middle
                if(j > 0 && j < processVec[i]->variables[j]->size - 1){

                    //std::cout << "15" << "\n" << std::endl;

                    if(variable_before_end != variable_begin && variable_after_begin != variable_begin)
                    {
                        std::string entry = std::to_string(pid) + "|" + std::to_string(variable_begin);

                        //table.erase(entry);
                        page_table->deletePage(pid, variable_begin);

                        //std::cout << "16" << "\n" << std::endl;
                    }

                    if(variable_after_begin != variable_end && variable_end != variable_before_end){

                        std::string entry = std::to_string(pid) + "|" + std::to_string(variable_end);

                        //table.erase(entry);
                        page_table->deletePage(pid, variable_end);

                        //std::cout << "17" << "\n" << std::endl;
                    }
                }

                if(j == processVec[i]->variables[j]->size - 1)
                {

                    //std::cout << "18" << "\n" << std::endl;

                    if(variable_begin != variable_before_end){

                         std::string entry = std::to_string(pid) + "|" + std::to_string(variable_begin);

                        //table.erase(entry);
                        page_table->deletePage(pid, variable_begin);

                        //std::cout << "19" << "\n" << std::endl;
                    }

                    if(variable_end != variable_before_end){
                        
                        std::string entry = std::to_string(pid) + "|" + std::to_string(variable_end);

                        //table.erase(entry);
                        page_table->deletePage(pid, variable_end);

                        //std::cout << "20" << "\n" << std::endl;
                    }
                }

                //delete all the pages in the middle

                for(int i = variable_begin + 1; i < variable_end; i++){
                    
                    std::string entry = std::to_string(pid) + "|" + std::to_string(i);

                    //table.erase(entry);
                    page_table->deletePage(pid, i);

                    //std::cout << "21" << "\n" << std::endl;

                }
                    


                //processVec[i]->variables.erase(processVec[i]->variables.begin()+j);
                mmu->deleteVariable(i, var_name);

                //std::cout << "22" << "\n" << std::endl;
            }
        }
    }

    //if page is empty, we need to delete the page --> dealing with page size
/*
    for(int i = 0; i < keys.size(); i++){

        if(table[].empty())
    }

    delete &mmu[pid];*/
}

void terminateProcess(uint32_t pid, Mmu *mmu, PageTable *page_table)
{
    // TODO: implement this!
    //   - remove process from MMU
    //   - free all pages associated with given process


    std::vector<Process *> processVec = mmu->getProcessVector();
    std::string entry;
    std::map<std::string, int> table = page_table->getTable();
    int memory_size = mmu->getMemorySize();
    int page_size = page_table->getPageSize();

    int flag = 0;//used if PID is found/existing


    //checks for exisiting process in the process vector
    
    for(int i = 0; i < processVec.size(); i++){
        
        if((uint32_t)processVec[i]->pid == pid){

            //printf(" error: process not found \n");
            flag = 1; 
        }
    }
    
    if(flag == 0){

        printf(" error: process not found \n");
    }

    
    for(int i = 0; i < processVec.size(); i++){
        
        if((uint32_t)processVec[i]->pid == pid){

            //processVec.erase(processVec.begin()+ i );
            mmu->deleteProcess(pid - 1024);

        
        }
    }

    for(int j = 0; j < memory_size/page_size; j++){

         entry = std::to_string(pid) + "|" + std::to_string(j);

         if(table.count(entry) > 0){

             //table.erase(entry);
             page_table->deletePage(pid, j);
         }
    }

    //delete &mmu[pid];
    //free(&page_table[pid]);
    
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

int getPageNumber(uint32_t address, int page_size){

    return floor(address/page_size);
}


