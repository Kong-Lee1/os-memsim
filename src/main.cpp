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
	    /*if ( !powerOfTwo(text_size) ) {
	    	printf("text_size is not a power of two, please try again.\n");
	    } else if ( !powerOfTwo(data_size) ) {
		    printf("data_size is not a power of two, please try again.\n");
	    } else */
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

            if (var_data_type != 00){
                    allocateVariable(pid, var_name, (DataType)var_data_type, allocate_num_elements, mmu, page_table);
                }
                //also need to handle if allocation would exceed system memory -- this would happen in main?
                //printf("%s error: allocation would exceed system memory \n")

        } 
        /*else if(arguments[0] == "convert"){std::cout << mmu->decimalToBinary(6556) << "\n";}*/else if(arguments[0] == "set"){
            /*
    

            set <PID> <var_name> <offset> <value_0> <value_1> <value_2> ... <value_N>
            Set the value for variable <var_name> starting at <offset>
            Note: multiple contiguous values can be set with one command
            */
        
           // need to check if user enter PID is valid
           // maybe have a curPID variable?

           pid = (uint32_t)std::stoi(arguments[1]);
           var_name = arguments[2];
           offset = (uint32_t)std::stoi(arguments[3]);

           int empty_index_holder;

            //we need keep track of the number of n-values provided
            for(int i = 0; i < sizeof(arguments); i++){

                if(arguments[i] == ""){
                    empty_index_holder = i;
                    break;
                }
                
            }

            set_num_args = empty_index_holder - 4;

            //do we need to handle data type when storing n-values into memory/array???

           if(pid < 1024 || pid > (1024 + processCreatorCounter)){

               std::cout << "pid is invalid, enter a valid pid \n";
               //std::cout << mmu->createProcess() << "\n";
           }

           //need to check that the variable exists in the specified process -- this already taken care of inside set
           
            

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
                
            }  else if(print_object == "processes"){
                         
                for(int i = 0; i < process_vector.size(); i++){

                    printf("%u \n", process_vector[i]->pid);
                }

            } else {
                
                //
                size_t sep1 = print_object.find(":");
                std::uint32_t object_pid = stoi(print_object.substr(0, sep1));
                std::string object_var_name = print_object.substr(sep1+1, print_object.size());
                std::cout << "object var name is " << object_var_name << "\n";
                DataType var_name_datatype;
                int type_size = -1;

                std::cout << "1" << "\n";

                //char, int, long

                char char_variable_datatype;
                int int_variable_datatype;
                long long_variable_datatype;
                short short_variable_datatype;

                for(int i = 0; i < process_vector.size(); i++){
                    std::cout << "2" << "\n";
                    if(process_vector[i]->pid == object_pid){
                        std::cout << "3" << "\n";
                        for(int j = 0; j < process_vector[i]->variables.size(); j++)
                        {
                            std::cout << "4" << "\n";
                            if(process_vector[i]->variables[j]->name == object_var_name){
                                std::cout << "5" << "\n";

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
                                std::cout << "6" << "\n"; 
                                int num_elements_of_variable = process_vector[i]->variables[j]->size/type_size;
                                uint32_t variable_physical_address = page_table->getPhysicalAddress(process_vector[i]->pid, process_vector[i]->variables[j]->virtual_address);//this is incorrect as it is currently 0
                                void* ptr_variable_physical_address = &variable_physical_address;//want to pass this as a void pointer but it is not currently working
                                
                                std::cout << "the virtual address is " << process_vector[i]->variables[j]->virtual_address << "\n" << std::endl;
                                std::cout << "variable physical address is " << variable_physical_address << std::endl;

                                std::string filled_str;

                                if(var_name_datatype == 1U){
                                    std::cout << "7" << "\n";
                                    if(num_elements_of_variable > 4){
                                        std::cout << "8" << "\n";
                                        for(int k = 0; k < 4; k++){
                                            std::cout << "9" << "\n";
                                            memcpy(&char_variable_datatype, (uint8_t*)memory+variable_physical_address+k, 1);

                                            //filled_str = &process_vector[i]->variables[j]->virtual_address+type_size*k;
                                            
                                            if(k == 3){
                                                    std::cout << "10" << "\n";
                                                
                                                filled_str.push_back(char_variable_datatype);

                                                filled_str = filled_str  + ", ...";
                                            
                                            }else{

                                                filled_str.push_back(char_variable_datatype);

                                                filled_str = filled_str + ", ";
                                                std::cout << "11" << "\n";
                                            }

                                        }
                                        std::cout << filled_str << "\n" << std::endl;
                                            std::cout << "12" << "\n";
                                    }else{
                                            std::cout << "13" << "\n";
                                        for(int k = 0; k < num_elements_of_variable; k++){
                                                std::cout << "14" << "\n";
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


                                //need to figure out a way to print the PID's Variable's Values
                                //may have to use pointers or this way???
                                //this only prints at one address, what about the others if there are multiple values
                                //can multiple values be set at one VA???

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
            std::cout << "error: command not recognized";
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
    printf("%u", pid);
    printf("\n");
    //std::cout << "allocated nothing";

    uint8_t dataType = 1U;
    //std::cout << "allocated nothing";

    uint32_t total_number_pages_to_add = ceil((text_size + data_size + 65536)/page_size);

    std::cout << "total number of pages to add is " << total_number_pages_to_add << std::endl;

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


    //if(page_table->){
        //page_table->addEntry(pid, page_table->getNextPageNumber(pid));
    //}

    //PageTable::_table 

    //std::cout << "1 \n";

    int pid_index = pid - 1024;
    int variable_index = mmu->getVariableIndex(pid_index, var_name);
    int type_size = -1;
    int var_type = -1;
    int page_break = -1;
    int page_start;
    std::vector<Process *> processVec = mmu->getProcessVector();

    uint32_t memory_size = mmu->getMemorySize();
    uint32_t page_size = page_table->getPageSize();
    uint32_t virtual_addr;

    std::cout << "memory_size is " << memory_size << std::endl;
    std::cout << "page size is " << page_size << std::endl;
    //std::cout << "2 \n";

    //checks for exisiting process in the process vector
    for(int i = 0; i < processVec.size()-1; i++){
        
        //std::cout << "000 \n";
        if((uint32_t)processVec[i]->pid != pid){

            printf(" error: process not found \n");
        }
    }

    //std::cout << "3 \n";

    //std::cout << "1 \n";

    //checks for exisiting variable in the process vector
    for(int i = 0; i < processVec.size()-1; i++){
        
        for(int j = 0; j < processVec[i]->variables.size()-1; j++ ){

            //std::cout << "011 \n";
            if(processVec[i]->variables[j]->name == var_name){

                printf(" error: variable already exists \n");
            }
        }
    }

    //std::cout << "4 \n";
    
    //std::cout << "2 \n";

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

    //std::cout << "5 \n";

    //std::cout << "3 \n";

    std::cout << "variable vector size is " << mmu->getVariableVecSize(pid_index) << std::endl;

    std::string var_name_holder;

    for(int i = 0; i < mmu->getVariableVecSize(pid_index); i++){

        var_name_holder = processVec[pid_index]->variables[i]->name;
        variable_index = mmu->getVariableIndex(pid_index, var_name_holder);

        std::cout << "6 \n";
        std::cout << "pid_index is " << pid_index << std::endl;
        std::cout << "limit is " << mmu->getVariableVecSize(pid_index) << std::endl;

        std::cout << "variable type is " << mmu->getVariableType(pid_index, i) << std::endl;
        std::cout << "variable size is " << mmu->getVariableSize(pid_index, variable_index) << std::endl;
        std::cout << "num_elements * type_size is " << num_elements*type_size << std::endl;
        
        
        
        if(mmu->getVariableType(pid_index, i) == 0U && mmu->getVariableSize(pid_index, variable_index) >= num_elements*type_size)
        {

            //memcpy((uint8_t*)memory + var_physical_addr, &value, type_size);

            //need to implement & check page boundaries

            //std::cout << "7 \n";

            for(int j = 0; j < memory_size/page_size; j++)//mmu->getMemorySize()/page_table->getPageSize() -> the # of pages we have 
            {
                
                page_start = page_size*j;

                //std::cout << "8 \n";

                //why are the statements different?
                if(page_start > mmu->getVirtualAddress(pid_index, variable_index) || page_start < mmu->getVirtualAddress(pid_index, variable_index) + num_elements*type_size){
                    
                    
                    page_start = (page_start - mmu->getVirtualAddress(pid_index, variable_index))%type_size;
                    std::cout << "page_start is " << page_start;
                    
                    virtual_addr = mmu->getVirtualAddress(pid_index, variable_index) + page_start;

                    std::cout << "\n";
                    std::cout << "virtual_addr is " << virtual_addr;

                    break;

                }/*else{

                    //need to allocate a new page if no hole is large enough
                    

                    

                    ///page_table->addEntry(pid, )

                    std::cout << "not enough space, need to allocate new page";
                
                }*/
                /*
                if(mmu->getVirtualAddress(pid_index, variable_index) < ){

                    mmu->addVariableToProcess(pid, var_name, type, type_size*num_elements, mmu->getVirtualAddress());

                }*/

            }

            //std::cout << "4 \n";
            mmu->addVariableToProcess(pid, var_name, type, type_size*num_elements, virtual_addr);

            std::cout << "10 \n";

                // need to check if the added variable still fits after adjusted page break(s);--04/27/21
            break;
        
        }else{

            std::cout << "nothing happened" << std::endl;
        }

        /*

        */

       std::cout << "something" << std::endl;

    }

    //std::cout << "5 \n";


    //printf("%d \n ", mmu->getVirtualAddress(pid_index, variable_index));
    // or is it
     printf("%d \n ", virtual_addr);

     //std::cout << "11";

     //std::cout << "6 \n";


    //page_table->getPhysicalAddress(pid, );
    
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
    }

    /*
        if (arguments[3] == "int" || arguments[3] == "float") {
		    var_data_type = 3U;
	    } else if (arguments[3] == "char") {
		    var_data_type = 1U;
	    } else if (arguments[3] == "short") {
		    var_data_type = 2U;
	    } else if (arguments[3] == "double" || arguments[3] == "long") {
		    var_data_type = 4U;
    */
   

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

    int pid_index = pid - 1024;
    int variable_index = mmu->getVariableIndex(pid_index, var_name);

    int var_type_size = -1;
    DataType var_type = mmu->getVariableType(pid_index, variable_index);
    std::vector<Process *> process_Vec = mmu->getProcessVector();

    var_type_size = mmu->getVariableSize(pid_index, variable_index);

    uint32_t var_virtual_addr = mmu->getVirtualAddress(pid_index, variable_index);
    uint32_t var_physcial_addr = page_table->getPhysicalAddress(pid, var_virtual_addr);

    //void *var_ptr = malloc();


    std::vector<Process *> processVec = mmu->getProcessVector();

    //checks for exisiting process in the process vector
    for(int i = 0; i < processVec.size(); i++){
        
        if((uint32_t)processVec[i]->pid != pid){

            printf(" error: process not found \n");
        }
    }

    //checks for non-exisiting variable in the process vector
    for(int i = 0; i < processVec.size(); i++){
        
        for(int j = 0; i < processVec[i]->variables.size(); j++ ){

            if(processVec[i]->variables[j]->name != var_name){

                printf(" error: variable not found \n");
            }
        }
    }


    for(int i = 0; i < processVec.size(); i++){
        
        for(int j = 0; i < processVec[i]->variables.size(); j++ ){

            if(processVec[i]->variables[j]->name == var_name){

                processVec[i]->variables.erase(processVec[i]->variables.begin()+j);
            }
        }
    }

    //delete &mmu[pid];
}

void terminateProcess(uint32_t pid, Mmu *mmu, PageTable *page_table)
{
    // TODO: implement this!
    //   - remove process from MMU
    //   - free all pages associated with given process


    std::vector<Process *> processVec = mmu->getProcessVector();

    //checks for exisiting process in the process vector
    for(int i = 0; i < processVec.size(); i++){
        
        if((uint32_t)processVec[i]->pid != pid){

            printf(" error: process not found \n");
        }
    }

    /*
    for(int i = 0; i < processVec.size(); i++){
        
        if((uint32_t)processVec[i]->pid == pid){

            processVec[i]->
            
        }
    }*/



    delete &mmu[pid];
    free(&page_table[pid]);
    
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
