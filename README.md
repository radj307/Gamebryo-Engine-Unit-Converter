# Gamebryo Unit Converter

 Commandline tool that can convert between Gamebryo Units, Meters, and Feet.
 It can convert values directly from the terminal, or a text file.



# Arguments
 Examples of correct syntax for each conversion method can be found below, under the Usage headers.
 
	 <value>	The number you want to convert
	 
	 <input unit>	The measurement unit of <value>
			Valid entries are: m, meters, f, ft, feet, u, units
			
	 <output unit>	The measurement unit <value> will be converted to
			Valid entries are the same as <input unit>
			
	 <filename>	The name of the file to convert. If this file is not located in the same directory as conv.exe, 
	 		you must include the full file path including drive, name, and extension.
			A new file with "-conv" appended to the name will be created to store the results.
			
Additionally, you can add 'r' to the end of the argument list to enable integral grouping, which is disabled by default to allow quick copy/paste for use in xEdit, zEdit, CK, etc.
(Grouping Disabled: 100000.0000, Grouping Enabled: 100,000.0000)



# Usage - Direct Conversion
 Open a terminal in the directory where conv.exe is located, then use:
 
	.\conv <input unit> <value> <output unit>
	
Example: (for cmd prompt syntax, remove the ".\\" prefix)
	
	.\conv u 1234.56789 m
	
Outputs:

	1.234568e+03 u          =  17.632378 m
	
The input value is always shown in scientific notation, while the output value is always shown in standard notation with 6 decimal point precision.


	
# Usage - File Conversion
 Open a terminal in the directory where conv.exe is located, then use:
 
 	.\conv <filename>
	
Example: (for cmd prompt syntax, remove the ".\\" prefix)

	.\conv myFile.txt
	
Creates a result file called:

	myFile-conv.txt
 
 Files must be formatted correctly, one conversion per line, each argument seperated by a single space.
 Example:
 
	u 1024 feet
	feet 250 meters
	ft 250 u
	m 500 u
	m 500 ft


# INI Settings
There are a few INI settings available:

	iPrecision		// Controls how many digits are shown after the decimal point.
	fRatioMetricImperial	// The conversion factor used by Metric <-> Imperial conversions
	fRatioMetricGamebryo	// The conversion factor used by Metric <-> Unit conversions
	fRatioGamebryoImperial	// The conversion factor used by Unit <-> Imperial conversions
	
The program does not need the INI file to operate normally, and will not automatically create one. To generate a fresh .ini file, use the command:

	.\conv /ini
	

# Credits & Thanks
As of version 2.3, ikalnytskyi's termcolor library is used for colored console output. ( https://github.com/ikalnytskyi/termcolor )
