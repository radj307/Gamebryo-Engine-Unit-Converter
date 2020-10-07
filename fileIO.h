/* fileIO.cpp - Header with basic file-related functions
 * By radj307(tjradj)
 */
#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
//#define DEBUG

 /**
  * fileRead(string, char)
  * Opens a file, copies it's contents to a buffer, closes the file, copies the buffer into a vector to format it, then returns the vector.
  *
  * @param filepath	- The full location of the target file, including filename.
  * @param delim		- Each occurance of this character will be placed at a seperate index in the vector
  * @returns vector<string> with file contents.
  */
inline std::vector<std::string> fileRead(std::string filepath, char delim = '\n')
{
	// create a vector of strings to return
	std::vector<std::string> dataReturn = {};
	// create a readbuffer stringstream
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
 * @param delim		- Each occurance of this char in the stream will be put on a new line.
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
 * fileIO_Debug(string) - DEBUG FUNCTION
 * Prompts terminal for input, then writes it to a file & closes it, then reads the file and outputs the contents to the terminal.
 * (Uses writeToFile & readFromFile)
 *
 * @param filepath	- The full filepath including file name & extension
 * @returns bool (true = success | false = failure)
 */#ifdef DEBUG
inline bool fileIO_Debug(std::string filepath)
{
	// create stringstream
	std::stringstream ss;

	// prompt user, input is only passed to stream if it is not "quit"
	std::cout << "Type some text, type 'quit' when done." << std::endl;
	// loop until "quit" is detected as input.
	for (std::string input = ""; input != "quit"; std::getline(std::cin, input)) {
		ss << input + '\n';
	}
	if (fileWrite(filepath, ss)) {
		std::vector<std::string> list;
		list = fileRead(filepath);
		for (auto it = list.begin(); it != list.end(); it++) {
			std::cout << *it << std::endl;
		}
		return true; // success
	}
	return false; // failure
}
#endif

/**
 * splitLine(string)
 * Returns a vector of strings with each index representing one word from the input string
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
 * fileRemoveExtension(string)
 * Utility function that returns a filename without the extension.
 *
 * @param filepath		- Target filepath/name
 * @returns	string		- Returns filename without extension, or filename if extension couldn't be found.
 */
inline std::string fileRemoveExtension(std::string filepath)
{
	// find the last occurance of '.' in filename, save its index
	std::string::size_type index = filepath.rfind('.');

	// remove all chars from index to string end
	if (index != std::string::npos) {
		filepath.erase(index, filepath.length());
	}
	// return filename
	return filepath;
}