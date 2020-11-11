/**
 * FileIO.h
 * Contains class FileIO, and static functions for various file i/o operations
 * by radj307
 */
#pragma once
#include <sstream>  // for stringstream
#include <fstream>  // for file stream
#include <vector>   // for vector

/** STATIC **
 * class File
 * Static functions for file I/O operations.
 */
struct FileIO {
protected:
    /** STATIC **
     * append(string, stringstream&)
     * Appends a stringstream to target file
     *
     * @param filename      - Target file's name
     * @param data          - Stream content to append to file
     * @returns bool        - ( true = wrote to file successfully ) ( false = failed )
     */
    static inline bool append(std::string filename, std::stringstream& data)
    {
        std::ofstream f(filename, std::ios_base::app);
        if ( f.is_open() ) {
            // write to file & return true
            f << std::endl << data.rdbuf();
            return true; // closes ofstream
        }
        // else return false
        return false; // closes ofstream
    }

    /** STATIC **
     * append(string, string&)
     * Appends a string to target file
     *
     * @param filename      - Target file's name
     * @param data          - string content to append to file
     * @returns bool        - ( true = wrote to file successfully ) ( false = failed )
     */
    static inline bool append(std::string filename, std::string& data)
    {
        std::ofstream f(filename, std::ios_base::app);
        if ( f.is_open() ) {
            // write to file & return true
            f << std::endl << data;
            return true; // closes ofstream
        }
        // else return false
        return false; // closes ofstream
    }

    /** STATIC **
     * overwrite(string, stringstream&)
     * Overwrites the contents of target file with a stringstream
     *
     * @param filename      - Target file's name
     * @param data          - Stream content to append to file
     * @returns bool        - ( true = wrote to file successfully ) ( false = failed )
     */
    static inline bool overwrite(std::string filename, std::stringstream& data)
    {
        std::ofstream f(filename);
        if ( f.is_open() ) {
            // write to file & return true
            f << data.rdbuf();
            return true; // closes ofstream
        }
        // else return false
        return false; // closes ofstream
    }

    /** STATIC **
     * overwrite(string, string&)
     * Overwrites the contents of target file with a string
     *
     * @param filename      - Target file's name
     * @param data          - string content to append to file
     * @returns bool        - ( true = wrote to file successfully ) ( false = failed )
     */
    static inline bool overwrite(std::string filename, std::string& data)
    {
        std::ofstream f(filename);
        if ( f.is_open() ) {
            // write to file & return true
            f << data;
            return true; // closes ofstream
        }
        // else return false
        return false; // closes ofstream
    }

public:
    /** STATIC **
     * save_type
     * List of valid save operations
     *
     * @member append       - Used to append to file
     * @member overwrite    - Used to overwrite a file
     */
    enum class save_type {
        append,     // appends data to the end of file
        overwrite   // overwrites the entire file with data
    };

    /** STATIC **
     * write(string, stringstream, save_type)
     * Writes a stringstream to file.
     *
     * @param filename  - Name & Location of save file
     * @param content   - Stringstream ref to be saved & cleared
     * @param saveAs    - Determines whether data will overwrite file, or be appended.
     * @returns bool    - Success state ( true = success ) ( false = fail )
     */
    static inline bool write(std::string filename, std::stringstream& data, save_type saveAs = save_type::append)
    {
        bool toReturn = false;
        // switch save type
        switch ( saveAs ) {
        case save_type::append: // append content to file
            toReturn = append(filename, data);
            data.clear();
            break;
        case save_type::overwrite: // overwrite file with content
            toReturn = overwrite(filename, data);
            data.clear();
            break;
        default:break;
        }
        return toReturn;
    }

    /** STATIC **
     * write(string, vector<string>, save_type)
     * Writes a vector of strings to file.
     *
     * @param filename  - Name & Location of save file
     * @param content   - vector of strings ref to be saved & cleared
     * @param saveAs    - Determines whether data will overwrite file, or be appended.
     * @returns bool    - Success state ( true = success ) ( false = fail )
     */
    static inline bool write(std::string filename, std::vector<std::string>& data, save_type saveAs = save_type::append)
    {
        bool toReturn = false;
        std::stringstream ss;

        for ( auto it = data.begin(); it != data.end(); it++ ) {
            size_t index_of_newline = (*it).rfind('\n');
            if ( index_of_newline != std::string::npos )
                ss << *it;
            else
                ss << *it << std::endl;
        }

        // switch save type
        switch ( saveAs ) {
        case save_type::append: // append content to file
            toReturn = append(filename, ss);
            data.clear();
            break;
        case save_type::overwrite: // overwrite file with content
            toReturn = overwrite(filename, ss);
            data.clear();
            break;
        default:break;
        }
        return toReturn;
    }

    /** STATIC **
     * write(string, string, save_type)
     * Writes a string to file.
     *
     * @param filename  - Name & Location of save file
     * @param content   - vector of strings ref to be saved & cleared
     * @param saveAs    - Determines whether data will overwrite file, or be appended.
     * @returns bool    - Success state ( true = success ) ( false = fail )
     */
    static inline bool write(std::string filename, std::string& data, save_type saveAs = save_type::append)
    {
        bool toReturn = false;
        switch ( saveAs ) {
        case save_type::append: // append content to file
            toReturn = append(filename, data);
            data.clear();
            break;
        case save_type::overwrite: // overwrite file with content
            toReturn = overwrite(filename, data);
            data.clear();
            break;
        default:break;
        }
        return toReturn;
    }

    /** STATIC **
     * readToStream(string)
     * Returns the contents of target file as a stringstream
     *
     * @param filename  - The name/location of the target file.
     * @returns stringstream
     */
    static inline std::stringstream readToStream(std::string filename)
    {
        std::stringstream ss;
        // open file
        std::ifstream f(filename);
        // check if file opened successfully
        if ( f.is_open() ) {
            // copy file content to stream
            f >> ss.rdbuf();
            return ss;
        }
        else return ss;
    }

    /** STATIC **
     * readToString(string)
     * Returns File::readToStream() as a string
     *
     * @param filename  - The name/location of the target file.
     * @returns string
     */
    static inline std::string readToString(std::string filename)
    {
        return readToStream(filename).str();
    }

    /** STATIC **
     * readToVector(string)
     * Returns File::readToStream() as a vector of strings
     *
     * @param filename  - The name/location of the target file.
     * @returns string
     */
    static inline std::vector<std::string> readToVector(std::string filename)
    {
        std::stringstream ss = readToStream(filename);
        std::vector<std::string> vec;
        for ( std::string parse; std::getline(ss, parse); vec.push_back(parse) ) {}
        return vec;
    }

    /** STATIC **
     * readToCStr(string)
     * Returns File::readToStream() as a const char*
     *
     * @param filename  - The name/location of the target file.
     * @returns const char*
     */
    static inline const char* readToCStr(std::string filename)
    {
        return readToStream(filename).str().c_str();
    }

    /** STATIC **
     * exists(string)
     * Checks if a file exists at the specified location.
     * 
     * @param filename  - The name/location of the target file.
     * @returns bool    - ( true = file exists ) ( false = file does not exist )
     */
    static inline bool exists(std::string filename)
    {
        std::ifstream f(filename);
        if ( f.is_open() )
            return true;
        return false;
    }

    /**
     * fileGetExtension(string, char)
     * Returns the name of a file without the extension
     *
     * @param path			- The full filepath including file name & extension
     * @param delim = '.'	- The last occurrence of this will be treated as the extension mark
     * @returns string		- The file extension with no name
     */
    static inline std::string getExtension(std::string path, char delim = '.')
    {
        // find the last occurrence of the delim parameter
        std::string::size_type delim_index = path.rfind(delim);

        // if rfind was successful (returns -1, or string::npos, on fail)
        if ( delim_index != std::string::npos ) {
            // erase the name from copied parameter, and return the extension
            path.erase(0, delim_index);
        }
        // return the file's extension only
        return path;
    }

    /**
     * fileGetName(string, char)
     * Returns the name of a file without the extension
     *
     * @param path			- The full filepath including file name & extension
     * @param delim = '.'	- The last occurrence of this will be treated as the extension mark
     * @returns string		- The filename with no extension
     */
    static inline std::string getName(std::string path, char delim = '.')
    {
        // find the last occurrence of the delim parameter
        std::string::size_type delim_index = path.rfind(delim);

        // if rfind was successful (returns -1, or string::npos, on fail)
        if ( delim_index != std::string::npos ) {
            // erase the extension from copied parameter, and return the name
            path.erase(delim_index, path.length());
        }
        // return the file's name without the extension
        return path;
    }

    /**
     * fileExtendName(string, string, char)
     * Returns the full filename with a string inserted between the extension and the filename. Used for making an alternate name to prevent overwriting the source file.
     * Ex: "file.txt" & "name" = "filename.txt"
     *
     * @param path		- The string to add to
     * @param text		- The string to insert into path
     * @param delim		- Optional delimiter, reverse searches the path param for this character, and inserts the text param at its position.
     * @returns string	- Returns the path param if delim character wasn't found. Otherwise, returns the path param with the text param inserted.
     */
    static inline std::string extendName(std::string path, std::string text, char delim = '.')
    {
        // reverse search for delim
        std::string::size_type delim_index = path.rfind(delim);

        // if rfind didn't throw an error
        if ( delim_index != std::string::npos ) {
            // insert text into path
            path.insert(delim_index, text);
            // return processed string
            return path;
        }
        // return default if delim wasn't found
        return path;
    }
};