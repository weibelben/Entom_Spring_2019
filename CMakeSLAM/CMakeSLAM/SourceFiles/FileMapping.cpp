#include "../HeaderFiles/FileMapping.h"
#include "../HeaderFiles/CrashAndBurn.h"

using namespace std;

/*
 * creates an empty list of tuples
 */
FileMapping::FileMapping()
{
    mapping = list<tuple<string, InputFile>>();
    num_mappings = 0;  
}

/*
 * adds a file to the list of tuples
 */
void FileMapping::add_file(string identifier, InputFile file)
{
    if (check_for_dups(file))
    {
        num_mappings++;
        const auto fileTuple = tuple<string, InputFile>(identifier, file);
        mapping.push_back(fileTuple);
    }
}

/*
 * returns the InputFile mapped to an identifier
 */
InputFile FileMapping::get_file(const string &identifier)
{
    for (auto fileTuple : mapping)
    {
        if (get<0>(fileTuple) == identifier)
        {
            return get<1>(fileTuple);
        }
    }

    // mapping does not exist
    CrashAndBurn("The file identifier " + identifier + " does not exist.\n");
    return InputFile(FileMapping(), "error", "error"); // return an empty file_mapping
}

/*
 * returns true if there are no mappings
 */
bool FileMapping::empty() const
{
    return num_mappings <= 0;
}

/*
 * given an inputFile, this method exits the program if there is a repeat 
 * mapping id to path
 */
bool FileMapping::check_for_dups(InputFile file)
{
    for (auto fileTuple : mapping)
    {
        if (get<0>(fileTuple) == file.fileName)
        {
            if (get<1>(fileTuple).filePath != file.filePath)
            {
                CrashAndBurn("The file identifier [" + file.fileName 
                    + "] is ambiguous.\n");
            }
            return false;
        }
    }

    return true;
}