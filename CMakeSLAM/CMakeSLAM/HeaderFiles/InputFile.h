#pragma once
#include "FileMapping.h"

#include <string>
#include <list>

using namespace std;

class InputFile; // forward declaration
class FileMapping;

/*
 * list of inputFiles
 */
class FileList
{
public:
    string listName;
    list<InputFile> files;

    FileList();
    FileList(string id, list<InputFile> file_list);
    FileList(FileMapping file_mapping, string identifier, list<string> file_paths);
    void print() const;
    void print_output_files() const;

private:
    void populate_files(FileMapping mapping, list<string> file_paths);
};


/*
 * Singular input file definitions
 */
class InputFile
{
public:
    string fileName;
    string filePath;
    FileList file_list;

    InputFile(FileMapping file_mapping, string identifier, string file_path);
    InputFile(string id, FileList files);

    list<string> inquire_file_paths(const string& file_path);
    void verify_existence(const string file_path);
    bool is_readable() const;
    bool is_list();
    void print() const;
    void print_output_file() const;

private:
    bool readable;
    bool file_set;

    static string remove_whitespace(string str);
    FileList copy_and_convert(FileMapping mapping, string identifier, string path, string ext);
    string convert_file(FileMapping mapping, string path, string ext);
};

