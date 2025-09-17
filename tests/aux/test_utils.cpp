#include "test_utils.hpp"
#include <vector>
#include <string>
#include <cstring>
#include <cstdint>

char** stringVecToCharArr(std::vector<std::string>& strings)
{
    char** argv = new char*[strings.size() + 1];
    for (size_t i = 0; i < strings.size(); i++)
    {
        argv[i] = new char[strings[i].length() + 1];
        std::strcpy(argv[i], strings[i].c_str());
    } 
    return argv;
}

void deleteCharArr(char** strings, uint8_t numOfStrings)
{
    for (size_t i = 0; i < numOfStrings; i++) 
    {
        delete[] strings[i];
    }
    delete[] strings;
}