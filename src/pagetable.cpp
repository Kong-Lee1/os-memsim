#include "pagetable.h"
#include <iostream>
#include <string>
#include <vector>
#include <math.h>

PageTable::PageTable(int page_size)
{
    _page_size = page_size;
}

PageTable::~PageTable()
{
}

std::vector<std::string> PageTable::sortedKeys()
{
    std::vector<std::string> keys;

    std::map<std::string, int>::iterator it;
    for (it = _table.begin(); it != _table.end(); it++)
    {
        keys.push_back(it->first);
    }

    std::sort(keys.begin(), keys.end(), PageTableKeyComparator());

    return keys;
}

void PageTable::addEntry(uint32_t pid, int page_number)
{
    // Combination of pid and page number act as the key to look up frame number
    std::string entry = std::to_string(pid) + "|" + std::to_string(page_number);// 1024|0

    //table will be ordered by PID
    //maps are implemented using binary search
    //map contains integer types

    int frame = 0;
    
    // Find free frame
    // TODO: implement this!
    // this is the same as PID 1024|Page# => Frame 0
    // PID 1024 points to Frame 0
    // the number of frames equal the number of pages
    _table[entry] = frame;

    //_table.empty;
    //the key would be passed into operator to access they key's mapped value
    //_table.operator[entry] = 

    // Do we need to consider page numbers?? - 04/23/21

    for(int i = 1; i < page_number; i++){

        if(_table.at(entry) == 0){

            _table[entry] = i;
        }
    }
    
}

int PageTable::getPhysicalAddress(uint32_t pid, uint32_t virtual_address)
{
    // Convert virtual address to page_number and page_offset - done - 04/21/21
    // TODO: implement this!
   
    uint32_t holder = 0;
    //uint32_t tester = 2;
    //bit shifting works for real_page_number & real_page_offset - 04/21/21

    uint32_t n = (uint32_t)log2(_page_size);// n bits for pageoffset
    uint32_t page_number = virtual_address >> n;
    uint32_t page_offset = (_page_size - 1) & virtual_address;

    /*

    for(int i = 0; i < page_offset_holder; i++){
        
        holder = holder + pow(2, i);
    }

    int real_page_offset = holder & virtual_address;

    int page_number_holder = 32 - page_offset_holder;

    holder = 0;

    for(int i = 0; i < page_number_holder; i++){
        
        holder = holder + pow(2, 31 - i);
    }

    int real_page_number = virtual_address & holder;

        //std::cout << "real page offset is " << page_offset_holder << "\n";

        //real_page_number = real_page_number >> page_offset_holder;
        //std::cout << "real page number is " << real_page_number << "\n";
    //for(int j = 0; j < page_offset_holder; j++){

        //std::cout << (real_page_offset >> 1) << "\n";
    //}
    */


    // Combination of pid and page number act as the key to look up frame number
    std::string entry = std::to_string(pid) + "|" + std::to_string(page_number);
    
    // If entry exists, look up frame number and convert virtual to physical address

    // physical memory = page number and frame --
    // virtual memory = page number and offset -- 
    // offset bits are the same but page numbers are different
    // how do we determine the which frame correspond to which page
    // conversion -- convert frame number into binary and place this onto the front of the page number
    // addresses are 32 bits but page sizes can be from 10 - 15 bits (1024 through 32768)
    // - ask about offset and how it affects the translation/conversion between physical and virtual addresses??? --left off here
    int address = -1;
    if (_table.count(entry) > 0)
    {
        // TODO: implement this!
        address = _table[entry];//this is the physical frame number address at the entry (PID|page_number) in physical memory

        //want to join the physical frame number address and page offset together

        address = address << n; 

        address = address & page_offset; //??? physical frame number and pageoffset

    }

    return address;
}

void PageTable::print()
{
    int i;

    std::cout << " PID  | Page Number | Frame Number" << std::endl;
    std::cout << "------+-------------+--------------" << std::endl;

    std::vector<std::string> keys = sortedKeys();

    //grab frame

    //mess with formatting

    for (i = 0; i < keys.size(); i++)
    {
        // TODO: print all pages

        std::string str_keys = keys[i];
        std::string key_pid = " " + str_keys.substr(0, 4) + " ";
        std::string key_page_Num = str_keys.substr(5, str_keys.length());
        std::string key_frame_tostring;
        int key_frame = _table[str_keys]; 


        if((key_page_Num.length() >= 13)){

            key_page_Num = key_page_Num.substr(0, 13);

        }else{

                while(key_page_Num.length() < 13)
                {
                    key_page_Num = " " + key_page_Num;
                }
        }

        key_frame_tostring = std::to_string(key_frame);

        if((key_frame_tostring.length() >= 13)){

                key_frame_tostring = key_frame_tostring.substr(0, 13);

        }else{

                while(key_frame_tostring.length() < 13)
                {
                    key_frame_tostring = " " + key_frame_tostring;
                }
        }

        
        std::cout << key_pid << "|" << key_page_Num << "|" << key_frame_tostring << std::endl;
    
    }
}

int PageTable::getNextPageNumber(uint32_t pid){

    int page_num = 0;

    std::string entry = std::to_string(pid) + "|" + std::to_string(page_num);

    while(_table.count(entry) > 0 ){

        page_num = page_num + 1;

        entry = std::to_string(pid) + "|" + std::to_string(page_num);

    }

    return page_num;

}

uint32_t PageTable::getPageSize(){

    return _page_size;
}


std::map<std::string, int> PageTable:: getTable(){

    return _table;
    
}

std::vector<std::string> PageTable:: getKeys(){

    return sortedKeys();
}
/*
std::map<std::string, int> PageTable:: getFrameNum(std::string pid, std::string pageNumber){


}*/

