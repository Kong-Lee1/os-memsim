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

    for(int i = 0; i < page_number; i++){

        if(_table.at(entry)){

            //_table.
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

    int page_offset_holder = log2(_page_size);

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




    // Combination of pid and page number act as the key to look up frame number
    std::string entry = std::to_string(pid) + "|" + std::to_string(real_page_number);
    
    // If entry exists, look up frame number and convert virtual to physical address
    // physical memory = page number and frame
    int address = -1;
    if (_table.count(entry) > 0)
    {
        // TODO: implement this!
        address = _table[entry];

        
    }

    return address;
}

void PageTable::print()
{
    int i;

    std::cout << " PID  | Page Number | Frame Number" << std::endl;
    std::cout << "------+-------------+--------------" << std::endl;

    std::vector<std::string> keys = sortedKeys();

    for (i = 0; i < keys.size(); i++)
    {
        // TODO: print all pages

        std::cout << keys[i];
    
    }
}
