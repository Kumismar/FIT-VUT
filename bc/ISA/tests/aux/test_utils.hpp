#pragma once

#include <string>
#include <vector>
#include <cstdint>

char** stringVecToCharArr(std::vector<std::string>& strings);
void deleteCharArr(char** strings, uint8_t numOfStrings);