# Bethesda Engine Unit Converter
  This is a commandline utility that converts between common length measurement units & Bethesda's arbitrary length measurement known as the _Unit_.  
  
  Supports all units from the following measurement systems:
  - Metric
  - Imperial
  - Gamebryo / CreationKit Units

## Installation
  No formal installation is required, simply place the executable somewhere and call it from your preferred shell.
  
  If you want to be able to use the program no matter your shell's current working directory, you'll need to place the executable in a location found in your `PATH` environment variable  
  
  ### Linux
  Place the executable in `/usr/local/bin` to add to your path.
  
  ### Windows
  [See Here](https://stackoverflow.com/a/9546345/8705305)
  
 ## Usage
  Use `ckconv -h` or `ckconv --help` to see a usage guide.  
  Both the standard symbol and full names are accepted for length unit types.  
  
  ### Examples
  To convert _5 units_ to _centimeters_: `ckconv u 5 cm` or `ckconv 5 u cm`  
  _-- to-do_
  
  You can chain any number of conversions, the program will read 3 arguments at a time until there are no more conversions remaining.
 
