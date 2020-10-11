/* fileIO.cpp - Header with basic file-related functions
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
  * @param filepath	- The full location of the target file, including filename.
  * @param delim		- Each occurrence of this character will be placed at a separate index in the vector
  * @returns vector<string> with file contents.
  */
inline std::vector<std::string> fileRead(std::string filepath, char delim = '\n')
{
	// create a vector of strings to return
	std::vector<std::string> dataReturn = {};
	// create a read-buffer stringstream
	std::stringstream rb;
	// create input filestream
	std::ifstream file;

	// attempt to open the file
	file.open(filepath);

	// check if file is open
	if (file.is_open())
	{
		// copy the file contents to the read buffer
		rb << file.rdbuf();
		// close the file
		file.close();

		// copy the read buffer into the vector. (the whole process is completed within the for loop declaration)
		for (std::string line = ""; std::getline(rb, line, delim); dataReturn.push_back(line)) {}
	}
	// return the vector
	return dataReturn;
}

/**
 * fileWrite(string, stringstream, char)
 * Creates a new file at target location
 *
 * @param filepath	- The full filepath including file name & extension
 * @param data		- sstream ref containing data to write to file
 * @param delim		- Each occurrence of this char in the stream will be put on a new line.
 * @param fixedNum	- Setting this to true will force all numbers to standard notation
 * @returns bool (true=success | false=fail)
 */
inline bool fileWrite(std::string filepath, std::stringstream& data, char delim = '\n')
{
	// create the output filestream
	std::ofstream file;
	// attempt to open new file for writing
	file.open(filepath);
	// check if file is open
	if (file.is_open())
	{
		// iterate through stringstream, insert line with \n appended
		for (std::string line = ""; std::getline(data, line, delim); file << line + '\n') {}

		// close the file
		file.close();
	}
	else return false; // error, return false

	// success, return true
	return true; 
}

/**
 * splitLine(string)
 * Returns a vector of strings with each index representing one word from the input string. Does not perform error correction.
 *
 * @param line				- The string to be split
 * @returns vector<string>	- Each word gets an index
 */
inline std::vector<std::string> splitLine(std::string line)
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
	return path;
}