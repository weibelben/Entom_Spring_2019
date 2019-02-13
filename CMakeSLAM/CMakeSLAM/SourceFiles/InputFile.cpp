#include "../HeaderFiles/InputFile.h"
#include "../HeaderFiles/CrashAndBurn.h"

#include <fstream>
#include <iostream>
#include <list>

using namespace std;

/*****************************************************************************/
// File List Functions:

/*
 * gets compiler to stop complaining
 */
FileList::FileList()
{
    listName = "UNINITIALIZED";
}

FileList::FileList(string id, list<InputFile> file_list)
{
    listName = id;
    files = file_list;
}

/*
 * Initializes the FileList and populates files
 */
FileList::FileList(FileMapping file_mapping, string identifier, list<string> file_paths)
{
    listName = identifier;
    populate_files(file_mapping, file_paths);
}

/*
 * prints each InputFile
 */
void FileList::print() const
{
    cout << "\t" << listName << " (list):\n";
    for (const auto file : files)
    {
        cout << "\t";
        file.print();
    }
}

/*
 * prints the name and path of all files in list
 */
void FileList::print_output_files() const
{
    cout << "\t" << listName << " (list):\n";
    for (const auto file : files)
    {
        cout << "\t";
        file.print_output_file();
    }
}

/*
 * populates the list of InputFiles by creating an InputFile for every path
 */
void FileList::populate_files(FileMapping mapping, list<string> file_paths)
{
    if (file_paths.empty())
    {
        CrashAndBurn("Attempt to create a FileList with no files.\n");
    }

    size_t list_num = 0;
    files = list<InputFile>();

    for (const auto file : file_paths)
    {
        // concatenate number to the end of the id, ex. fasta0, fasta1, etc.
        string id = listName + to_string(list_num);
        files.emplace_back(InputFile(mapping, id, file));
        list_num++;
    }
}

/*****************************************************************************/
// Singular input files:

/*
 * initializes a single input file containing an id and a filepath
 */
InputFile::InputFile(FileMapping file_mapping, const string identifier, const string file_path)
{ 
    fileName = identifier;

    if (file_path[0] == '[')
    {
        if (file_path.find(']') == string::npos)
        {
            CrashAndBurn("Syntax error, assignment of " + identifier
                    + "\n\tpath began with list token \'[\' but did not contain \']\'\n");
        }
        
        if (file_path[1] == '>')
        {
            auto new_id = remove_whitespace(file_path.substr(2, file_path.size() - 2));
            const size_t file_ext_start = file_path.find_last_of('.');

            if (file_ext_start == string::npos || file_ext_start != file_path.find(']') + 1)
            {
                CrashAndBurn("Invalid File Extension  " + file_path 
                    + "\n\tMust be in the following format: [>file_id].ext");
            }

            const auto file_ext = file_path.substr(file_path.find('.'));
            new_id = remove_whitespace(new_id.substr(0, new_id.find_last_of(']')));
            const auto file = file_mapping.get_file(new_id); // DNE case handled in FileMapping
            
            if (file.file_set)
            {
                // copying file list from input_files
                file_set = true;
                file_list = copy_and_convert(file_mapping, identifier, new_id, file_ext);
            }
            else
            {
                // copy path to input file, change ext
                file_set = false;
                filePath = convert_file(file_mapping, new_id, file_ext);
            }
        }
        else
        {            
            // create a new file list
            file_set = true;
            file_list = FileList(file_mapping, identifier, inquire_file_paths(file_path));
        }
    }
    else
    {
        file_set = false;
        filePath = file_path;
        verify_existence(file_path);
    }
}

InputFile::InputFile(string id, FileList files)
{
    file_set = true;
    file_list = files;
    fileName = id;
}

/*
 * Accesses the file system to return the names of the files that were requested
 */
list<string> InputFile::inquire_file_paths(const string& file_path)
{
    if (file_path[file_path.size() - 1] != ']')
    {
        CrashAndBurn("Attempt to create a list of files with contents after the closing \']\'\n");
    }

    const string path = remove_whitespace(file_path.substr(1, file_path.size() - 2));

    const string inquiry_file = "File_Path_Inquiry.txt";

    // redirect stdout to inquiry file, list files.
    freopen(inquiry_file.c_str(), "w", stdout);
    auto command = "ls " + path;
    system(command.c_str());

    // redirect stdout back to the console
    freopen("/dev/tty", "a", stdout);

    list<string> file_paths;
    try
    {
        ifstream stream;
        stream.open(inquiry_file);

        for (string file; getline(stream, file); )
        {
            file_paths.push_back(file);
        }
        
        stream.close();
    }
    catch (exception& e)
    {
        CrashAndBurn("Issues occured opening " + inquiry_file + " for reading.\n");
    }
    
    return file_paths;
}

/*
 * return whether this file exists/can be opened
 */
bool InputFile::is_readable() const
{
    return readable;
}

bool InputFile::is_list()
{
    return file_set;
}

/*
 * prints the file name and path and whether it was validated
 */
void InputFile::print() const
{
    if (!file_set)
    {
        cout << "\t" << fileName << " = " << filePath << "\t\t[";

        if (readable)
            cout << "valid]\n";
        else
            cout << "invalid]\n";
    }
    else
    { 
        file_list.print();
    }
}

/*
 * prints the file name and path
 */
void InputFile::print_output_file() const
{
    if (!file_set)
    {
        cout << "\t" << fileName << " = " << filePath << "\n";
    }
    else
    {
        file_list.print_output_files();
    }
}

/*
 * verifies the existence of a file by opening it, then closing it
 */
void InputFile::verify_existence(const string file_path)
{
    if (!file_set)
    {
        try
        {
            ifstream stream;
            stream.open(file_path);
            if (stream.is_open())
            {
                readable = true;
            }
        }
        catch (exception& e)
        {
            readable = false;
        }
    }
}

/*
 * This method removes whitespace (space or tab chars) from the beginning and end of str
 */
string InputFile::remove_whitespace(string str)
{
    int start = 0;
    size_t end = str.length() - 1;

    // ascii for space and tab char
    while (str.at(start) == 32 || str.at(start) == 9) { start++; }
    while (str.at(end) == 32 || str.at(end) == 9) { end--; }
    return str.substr(start, end - start + 1);
}

/*
 * given the id of another fileList
 */
FileList InputFile::copy_and_convert(FileMapping mapping, string identifier, const string path, const string ext)
{
    auto file_list = list<InputFile>();
    size_t count = 0;

    for (auto file : mapping.get_file(path).file_list.files)
    {
        auto file_path = file.filePath;
        auto start_ext = file_path.find_last_of('.');

        if (start_ext == string::npos)
        {
            CrashAndBurn(file_path + " does not contain a file extension.\n");
        }

        file_list.emplace_back(mapping, path + to_string(count), file_path.substr(0, start_ext) + ext);
        count++;
    }

    return FileList(identifier, file_list);
}

/*
 * given an input file, converts it to an output file and adds new file ext
 */
string InputFile::convert_file(FileMapping mapping, string path, string ext)
{
    auto file_path = mapping.get_file(path).filePath;
    const size_t start_ext = file_path.find_last_of('.');

    if (start_ext == string::npos)
    {
        CrashAndBurn(file_path + " does not contain a file extension.\n");
    }

    return file_path.substr(0, start_ext) + ext;
}
