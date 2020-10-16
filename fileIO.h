/**
 * fileIO.cpp
 * Header with basic file-related functions
 * By radj307(tjradj)
 * 
 * Dependencies:
 * vector
 * stringstream
 * filestream
 */
#pragma once
#include <vector>
#include <fstream>
#include <sstream>

 /**
  * fileRead(string, char)
  * Opens a file, copies it's contents to a buffer, closes the file, copies the buffer into a vector to format it, then returns the vector.
  *
  * @param filepath			- The full location of the target file, including filename.
  * @param delim			- Each occurrence of this character will be placed at a separate index in the vector. (defaults to newline)
  * @returns vector<string>	- File contents, indexed by delim param.
  */
inline std::vector<std::string> fileRead(std::string filepath, char delim = '\n')
{
	// create a vector of strings to return
	std::vector<std::string> fileContents;

	// create a read-buffer stringstream
	std::stringstream readbuffer;
	// create input filestream
	std::ifstream file;

	// attempt to open the file
	file.open(filepath);

	// check if file is open
	if (file.is_open())
	{
		// copy the file contents to the read buffer
		readbuffer << file.rdbuf();
		// close the file
		file.close();

		// copy the read buffer into the vector. (the whole process is completed within the for loop declaration)
		for (std::string line = ""; std::getline(readbuffer, line, delim); fileContents.push_back(line)) {}
	}
	// return the vector
	return fileContents;
}

/**
 * fileWrite(string, stringstream, char)
 * Writes a stringstream to target file. Endlines must be included in the stringstream to appear in the file.
 *
 * @param filepath	- The full filepath including file name & extension
 * @param data		- sstream ref containing data to write to file
 * @param delim		- Each occurrence of this char in the stream will be put on a new line.
 * @param fixedNum	- Setting this to true will force all numbers to standard notation
 * @returns boolean	- ( true = successful ) ( false = failed to write )
 */
inline bool fileWrite(std::string filepath, std::stringstream& data, char delim = '\n')
{
	// create the output filestream
	std::ofstream file;
	// attempt to open new file for writing
	file.open(filepath);
	// check if file is open
	if (file.is_open())	{
		// write data stream to file
		file << data.rdbuf() << "\t\\ \\ \\  END  / / /";

		// close the file
		file.close();

		// success, return true
		return true;
	}
	// error, return false
	return false;

}

/**
 * splitString(string)
 * Returns a vector of strings with each index representing one word from the input string. Does not perform error correction.
 *
 * @param line				- The string to be split
 * @param delim				- The delimiter to split the string by.
 * @returns vector<string>	- String contents, indexed by delim param.
 */
inline std::vector<std::string> splitString(std::string line, char delim = ' ')
{
	// create a vector to return words from the line
	std::vector<std::string> words = {};
	// create a stringstream for getline
	std::stringstream ss;
	// copy the line param to stringstream
	ss << line;
	// iterate through sstream, copy each word to vector
	for (std::string word = ""; std::getline(ss, word, ' '); words.push_back(word)) {}
	// return vector
	return words;
}

/**
 * fileGetExtension(string, char)
 * Returns the name of a file without the extension
 *
 * @param path			- The full filepath including file name & extension
 * @param delim = '.'	- The last occurrence of this will be treated as the extension mark
 * @returns string		- The file extension with no name
 */
inline std::string fileGetExtension(std::string path, char delim = '.')
{
	// find the last occurrence of the delim parameter
	std::string::size_type delim_index = path.rfind(delim);

	// if rfind was successful (returns -1, or string::npos, on fail)
	if (delim_index != std::string::npos) {
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
inline std::string fileGetName(std::string path, char delim = '.')
{
	// find the last occurrence of the delim parameter
	std::string::size_type delim_index = path.rfind(delim);

	// if rfind was successful (returns -1, or string::npos, on fail)
	if (delim_index != std::string::npos) {
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
inline std::string fileExtendName(std::string path, std::string text, char delim = '.')
{
	// reverse search for delim
	std::string::size_type delim_index = path.rfind(delim);

	// if rfind didn't throw an error
	if (delim_index != std::string::npos) {
		// insert text into path
		path.insert(delim_index, text);
		// return processed string
		return path;
	}
	// return default if delim wasn't found
	return path;
}