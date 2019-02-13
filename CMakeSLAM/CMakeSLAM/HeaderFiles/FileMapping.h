#pragma once

#include <tuple>
#include <list>

#include "InputFile.h"

using namespace std;
class InputFile;

class FileMapping
{
public:
    size_t num_mappings;

    FileMapping();
    void add_file(string identifier, InputFile file);
    InputFile get_file(const string& identifier);
    bool empty() const;

private:
    list<tuple<string, InputFile>> mapping;
    bool check_for_dups(InputFile file);
};