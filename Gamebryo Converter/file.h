#pragma once
#include "fileIO.h"
#include "factor.h"
#include "Value.h"
#include "info.h"

class Content {
protected: // accessible in children
	std::vector<std::string> _content;

	/**
	 * getFrom(string)
	 * Retrieves the contents of a file and saves it to a member vector
	 *
	 * @param filename	- The name/location/extension of target file
	 * @returns bool success state
	 */
	inline bool getFrom(std::string filename)
	{
		_content = fileRead(filename);
		return valid();
	}

	/**
	 * saveTo(string)
	 * Saves member vector to a file
	 *
	 * @param filename	- The name/location/extension of target file
	 * @returns bool success state
	 */
	inline bool saveTo(std::string filename)
	{
		if ( valid() )
			return fileWrite(filename, _content);
		return false;
	}

public:	// accessible anywhere

	/**
	 * valid()
	 * Checks if the content vector has been successfully populated.
	 *
	 * @returns bool success state
	 */
	inline bool valid()
	{
		if ( !_content.empty() )
			return true;
		return false;
	}
};

class File : public Content {
	std::string _filename;
public:
	std::string _savename;
	bool _success = false;

	File(std::string file) : _filename(file), _savename(fileExtendName(_filename, "-converted"))
	{
		// check if file was read to content successfully
		if ( getFrom(_filename) ) {
			// create write buffer
			std::stringstream toWrite;

			// iterate through content
			for ( auto it = _content.begin(); it != _content.end(); it++ ) {
				// create stream for line buffer
				std::stringstream line;
				line << *it;
				// create a vector of strings to hold arguments
				std::vector<std::string> args;

				// parse line with delim ' ', populate vec
				for ( std::string parse; std::getline(line, parse, ' '); args.push_back(parse) ) {
					// remove spaces
					parse.erase(std::remove(parse.begin(), parse.end(), ' '), parse.end());
					// remove tabs
					parse.erase(std::remove(parse.begin(), parse.end(), '\t'), parse.end());
				}

				// check if line has 3 arguments
				if ( args.size() == 3 && std::all_of(args[0].begin(), args[0].end(), ::isalpha) && std::all_of(args[2].begin(), args[2].end(), ::isalpha) ) {
					// convert parsed args to Values
					Value in(args[0], args[1]);
					Value out = in.convert_to(args[2]);

					// check if Values are valid
					if ( in.valid() && out.valid() ) {
						std::string displayIn = in.asString(), displayOut = out.asString(true), buffer = "\t=  ";
						if ( displayIn.size() < 12 )
							buffer = '\t' + buffer;
						toWrite << displayIn << buffer << displayOut << std::endl;
					}
					else // add newline to stream
						toWrite << std::endl;
				} // else add a newline
				else toWrite << std::endl;
			}
			_success = fileWrite(_savename, toWrite);
		}
		else info::msg(info::error, "Couldn't open '" + _filename + "'");
	}

	~File() {}
};