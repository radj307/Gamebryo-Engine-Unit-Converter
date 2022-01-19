# Bethesda Engine Unit Converter
  This is a commandline utility that converts between common length measurement units & Bethesda's arbitrary length measurement known as the _Unit_.  
  
  Supports all units from the following measurement systems:
  - Metric
  - Imperial
  - Gamebryo / CreationKit Units

  If you're new to using CLI programs, see the [guide](https://github.com/radj307/Gamebryo-Engine-Unit-Converter/wiki#detailed-usage-guide-for-less-experienced-terminal-users).

## Installation
  No formal installation is required, simply place the executable somewhere and call it from your preferred shell.
  
  If you want to be able to use the program no matter your shell's current working directory, you'll need to place the executable in a location found in your `PATH` environment variable.  
  
  The program is small enough to embed within another executable, for an example see [this synthesis patcher](https://github.com/radj307/Metric-Units-Patcher).
  
  ### Linux
  Place the executable in `/usr/local/bin` to add to your path.
  
  ### Windows
  [See Here](https://stackoverflow.com/a/9546345/8705305)
  
## Usage  
  Use `ckconv -h` or `ckconv --help` to see a usage guide.  
  Both the standard symbol and full names are accepted for length unit types.  
  
  ### Options
  This is more-or-less the same information displayed when using the `ckconv -h` or `ckconv --help` command.
  | Option                           | Description                                                                                          |  
  |----------------------------------|------------------------------------------------------------------------------------------------------|  
  | `-h` `--help`                    | Show the inline usage help display, and exit.                                                        |  
  | `-v` `--version`                 | Show the current version number & exit.                                                              |  
  | `--standard`  `--fixed`          | Always use standard notation when printing numbers.                                                  |  
  | `--scientific`  `--sci`          | Always use scientific notation when printing numbers.                                                |  
  | `-p <INT>`  `--precision <INT>`  | Override the number of digits to show after the decimal point. By default, precision is "as-needed". |  
  | `-a <INT>`  `--align-to <INT>`   | Set the number of characters _(from the left side of the screen)_ before printing output values & units. |
  | `-q` `--quiet`                   | Print only the resulting values of conversions, hide input values, units, and output units.          |  
  | `-n` `--no-color`                | Disable the usage of ANSI color escape sequences, effectively disabling the usage of all colors.     |  
  | `--reset-ini`                    | Create or overwrite the config file with the current configuration. This is sensitive to other options, allowing you to control the values written to the INI by including options that have a corresponding INI key. |  
  
  ### Examples
  Simple conversions, and pipe operators:  
  ![](https://i.imgur.com/eoeCV8t.png)
  ![](https://i.imgur.com/djxJO0t.png)
  ![](https://i.imgur.com/AH01PU6.png)
  ![](https://i.imgur.com/djxJO0t.png)
  ![](https://i.imgur.com/02IlBID.png)  
  _Note: Only the Linux version allows piped input, as Windows refuses to process STDIN using `select()`. Blame Microsoft._
  
  Multiple conversions at once:  
  ![](https://i.imgur.com/TPmpR1W.png)
  ![](https://i.imgur.com/djxJO0t.png)
  ![](https://i.imgur.com/WvhAz51.png)
  
  Converting 5 picometers to units in standard notation, and response-only mode:  
  ![](https://i.imgur.com/HoykBM7.png)
  ![](https://i.imgur.com/fLj1yvU.png)
  ![](https://i.imgur.com/9LTzbIz.png)
