#pragma once
#include "file.h"
#include "sys.h"
#include "Value.h"

/**
 * class Content
 * Stores the contents of target file in local memory.
 */
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
		_content = file::readToVector(filename);
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
			return file::write(filename, _content);
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

/** 
 * class File : public Content
 * Processes a file, then saves the contents.
 */
class File : public Content {
	std::string _filename;
public:
	std::string _savename;
	bool _success = false;

	File(std::string file) : _filename(file), _savename(file::extendName(_filename, "-converted"))
	{
		// check if file was read to content successfully
		if ( getFrom(_filename) ) {
			// create write buffer
			std::stringstream toWrite;

			toWrite << std::fixed;
			toWrite.precision(cfg.iGet("Precision"));

			// iterate through content
			for ( auto it = _content.begin(); it != _content.end(); it++ ) {
				// delete extra char from windows line endings if present
				(*it).erase(std::remove((*it).begin(), (*it).end(), '\r'), (*it).end());
				// replace all tabs with spaces
				for ( auto it_ = it->begin(); it_ != it->end(); it_++ ) {
					switch ( *it_ ) {
					case '\t':
						*it_ = ' ';
						break;
					default:break;
					}
				}

				// create stream for line buffer
				std::stringstream line;
				line << *it;
				// create a vector of strings to hold arguments
				std::vector<std::string> args;

				// parse line with delim ' ', populate vec
				for ( std::string parse; std::getline(line, parse, ' '); ) {
					// remove spaces
					parse.erase(std::remove(parse.begin(), parse.end(), ' '), parse.end());

					if ( parse.size() > 0 )
						args.push_back(parse);
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
			_success = file::write(_savename, toWrite);
		}
		else sys::msg(sys::error, "Couldn't open '" + _filename + "'");
	}
};