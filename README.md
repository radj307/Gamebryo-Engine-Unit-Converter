# Bethesda Engine Unit Converter
<p align="center"><img alt="GitHub tag (latest by date)" src="https://img.shields.io/github/v/tag/radj307/Gamebryo-Engine-Unit-Converter?color=ffffff&label=Current%20Version&logo=github&style=for-the-badge">&nbsp&nbsp<img alt="GitHub all releases" src="https://img.shields.io/github/downloads/radj307/Gamebryo-Engine-Unit-Converter/total?color=ffffff&logo=github&style=for-the-badge"></p>
  This is a commandline utility that converts between common length measurement units & Bethesda's arbitrary length measurement known as the _Unit_.  

- Units Supported
  - Metric
    - Includes all metric prefixes from `pico-` to `tera-`
  - Imperial
    - Includes all Imperial length units listed on [wikipedia](https://en.wikipedia.org/wiki/Imperial_units#Length).
  - Gamebryo / CreationKit Units
    - Includes all metric prefixes from `pico-` to `tera-` because who doesn't want to use kilounits
- Cross-Platform
  - Windows
  - Linux
  - macOS
  

If you're new to using CLI programs, see the [beginners guide](https://github.com/radj307/Gamebryo-Engine-Unit-Converter/wiki#detailed-usage-guide-for-less-experienced-terminal-users).

## Installation
  No formal installation is required, simply place the executable somewhere and call it from your preferred shell.
  
  If you want to be able to use the program no matter your shell's current working directory, you'll need to place the executable in a location found in your `PATH` environment variable.  
  
  The program is small enough to embed within another executable, for an example see [this synthesis patcher](https://github.com/radj307/Metric-Units-Patcher).

### Adding to PATH
  #### Linux
  Place the executable in `/usr/local/bin` to add to your path, or add `export PATH='$PATH;<PATH_TO_DIRECTORY>'` to the end of `~/.bashrc`
  
  #### Windows
  [See Here](https://stackoverflow.com/a/9546345/8705305)
  
  #### macOS
  I genuinely have no idea as I have never owned or used a mac, but I assume it's similar to Linux.
  
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
  | `-f`  `--full-name`              | Use full unit names instead of their official symbols. _Ex: `Kilometers` instead of `km`_
  | `-p <INT>`  `--precision <INT>`  | Override the number of digits to show after the decimal point. By default, precision is "as-needed". |  
  | `-a <INT>`  `--align-to <INT>`   | Set the number of characters _(from the left side of the screen)_ before printing output values & units. Useful when converting many values at once. |
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
  
  Multiple conversions at once:  
  ![](https://i.imgur.com/TPmpR1W.png)
  ![](https://i.imgur.com/djxJO0t.png)
  ![](https://i.imgur.com/WvhAz51.png)
  
  Converting 5 picometers to units in standard notation, and response-only mode:  
  ![](https://i.imgur.com/HoykBM7.png)
  ![](https://i.imgur.com/fLj1yvU.png)
