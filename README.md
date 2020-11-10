# Gamebryo Unit Converter (conv.exe)

 Commandline tool that can convert between Gamebryo Units, Meters, and Feet.
 It can convert values directly from the terminal, or a text file.
 Linux beta version is available as of (v3.1.5)


# Arguments
 Examples of correct syntax for each conversion method can be found below, under the Usage headers.
 
 In-Terminal Conversion:
 
	 <input unit>	The measurement unit of <value>
	 		Valid entries:
			Units:	( u, unit, units, beth, bethesda, gamebryo, game )
			Meters:	( m, meter, meters, metric, met 		 )
			Feet:	( f, ft, feet, foot, imperial, imp, i 		 )
			
	 <value>	Any integer or real number under 
			
	 <output unit>	The measurement unit <value> will be converted to
			Valid entries are the same as <input unit>
			
 File Conversion:
			
	 <filename>	The name of a text file located in the same directory.
			A new file will be created to store the results.


# Installation & Usage
Extract .zip file to a location of your choice, then open a terminal in that directory.

You can chain command arguments by seperating them with commas (,). 
Currently the only commands available are /ini and /f=<filename>
Example:
	conv /ini,f=file.txt


# Inline Conversions

	conv <input unit> <value> <output unit>
	
Example:
	
	conv u 1234.56789 m
	
Converts 1234.56789 gamebryo units to meters, this will output:

	1234.567890 u   =  17.638927 m

As of (v3.1.5) you can perform multiple conversions on the same line:

	conv <input unit> <value> <output unit> <input unit> <value> <output unit>
	

# File Conversions

Create a text file in the directory where conv.exe is located, or include the path when using command:
(PRE-v3.1.5)

 	conv /<filename>
	
(v3.1.5+)

	conv /f=<filename>
	
 Files must be formatted correctly, one conversion per line, each argument seperated by a single space.
 Example:
 
	units	1024	feet
	unit	1	meter
	foot	250	meters
	ft	357	unit
	m	0.001	u
	m	5000	ft


# INI Configuration

To generate a new INI file with the default values, use this command:

	conv /ini

It will create a file called "conv.ini" in the directory where conv.exe is located.

INI Settings: (v3.1.3)

	iPrecision		// Controls how many digits are shown after the decimal point.
	fRatioMetricImperial	// The conversion factor used by Metric <-> Imperial conversions
	fRatioMetricGamebryo	// The conversion factor used by Metric <-> Unit conversions
	fRatioGamebryoImperial	// The conversion factor used by Unit <-> Imperial conversions

As of (v3.1.4+) the setting names have changed:

	iPrecision	// Controls how many digits are shown after the decimal point.
	fFactor_MI	// Metric <-> Imperial conversion factor (m / ft)
	fFactor_UM	// Metric <-> Unit conversion factor
	fFactor_UI	// Unit <-> Imperial conversion factor
	
	
# Credits & Thanks
As of (v2.3), ikalnytskyi's termcolor library is used for colored console output. ( https://github.com/ikalnytskyi/termcolor )
