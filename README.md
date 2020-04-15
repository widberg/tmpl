# tmpl
CLI template replacement tool

### Usage
```sh
tmpl TEMPLATE OUTPUT [ARG]...
```

### Example
command
```sh
tmpl template.txt output$1.txt "argument zero" arg1 arg2
```

template.txt
```plain_text
Example: $0 $(1) $2 $(TEMPLATE_ARGUMENT) $(OUTPUT_ABSOLUTE) $c $Z $$
```

outputarg1.txt
```plain_text
Example: argument zero arg1 arg2 template.txt D:\programming\widberg\tmpl\build\outputarg1.txt Thu Aug 23 14:55:02 2001 CDT $
```

## Replacement Macros
A replacement macro is denoted by a $ followed by a series of one or more digits, an individual alphabetical character, or a string of one or more alphanumeric characters enclosed in a pair of parentheses. Replacement macros in the template file will be replaced with a coresponding value in the output file. File names and directory names will be evaluated for replacement macros as well. The two types of replacement macro are listed bellow.

### Indexes
Any replacement macro beginning with a numerical digit will be interpreted as an index. Each index is replaced by the argument passed to tmpl in that possition following the output path. The fist argument following the output path is denoted by index 0. Following argument indexes sequentially increment by 1.

### Token
Any replacement macro that matches a string bellow will be replaced with the described value. Tokens are case sensitive.

| Token | Description | Example |
| --- | --- | --- |
| TEMPLATE_ARGUMENT | The argument given as the template path |
| OUTPUT_ARGUMENT | The argument given as the output path |
| TEMPLATE_ABSOLUTE | The absolute path to the current template file after replacement |
| OUTPUT_ABSOLUTE | The absolute path to the current output file after replacement |
| TEMPLATE_RELATIVE | The relative path to the current template file after replacement |
| OUTPUT_RELATIVE | The relative path to the current output file after replacement |
| TEMPLATE_DIRECTORY_ABSOLUTE | The absolute path to the directory of the current template file after replacement |
| OUTPUT_DIRECTORY_ABSOLUTE | The absolute path to the directory of the current output file after replacement |
| TEMPLATE_DIRECTORY_RELATIVE | The relative path to the directory of the current template file after replacement |
| OUTPUT_DIRECTORY_RELATIVE | The relative path to the directory of the current output file after replacement. |
| WORKING_DIRECTORY | The absolute path to the directory tmpl was executed from |
| a<sup>†</sup> | Abbreviated weekday name | Thu |
| A<sup>†</sup> | Full weekday name | Thursday |
| b<sup>†</sup> | Abbreviated month name | Aug |
| B<sup>†</sup> | Full month name | August |
| c<sup>†</sup> | Date and time representation | Thu Aug 23 14:55:02 2001 |
| C<sup>†</sup> | Year divided by 100 and truncated to integer | 20 |
| d<sup>†</sup> | Day of the month, zero-padded (01-31)	| 23 |
| D<sup>†</sup> | Short MM/DD/YY date, equivalent to $m/$d/$y | 08/23/01 |
| e<sup>†</sup> | Day of the month, space-padded ( 1-31) | 23 |
| E | Seconds since UNIX Epoch (00:00:00, 1 Jan 1970 UTC) | 998596502 |
| F<sup>†</sup> | Short YYYY-MM-DD date, equivalent to $Y-$m-$d	| 2001-08-23 |
| g<sup>†</sup> | Week-based year, last two digits (00-99) | 01 |
| G<sup>†</sup> | Week-based year | 2001 |
| h<sup>†</sup> | Abbreviated month name (same as $b) | Aug |
| H<sup>†</sup> | Hour in 24h format (00-23) | 14 |
| I<sup>†</sup> | Hour in 12h format (01-12) | 02 |
| j<sup>†</sup> | Day of the year (001-366)	| 235 |
| m<sup>†</sup> | Month as a decimal number (01-12) | 08 |
| M<sup>†</sup> | Minute (00-59) | 55 |
| p<sup>†</sup> | AM or PM designation | PM |
| r<sup>†</sup> | 12-hour clock time | 02:55:02 pm |
| R<sup>†</sup> | 24-hour HH:MM time, equivalent to $H:$M | 14:55 |
| S<sup>†</sup> | Second (00-61) | 02 |
| T<sup>†</sup> | ISO 8601 time format (HH:MM:SS), equivalent to $H:$M:$S | 14:55:02 |
| u<sup>†</sup> | ISO 8601 weekday as number with Monday as 1 (1-7) | 4 |
| U<sup>†</sup> | Week number with the first Sunday as the first day of week one (00-53) | 33 |
| V<sup>†</sup> | ISO 8601 week number (01-53) | 34 |
| w<sup>†</sup> | Weekday as a decimal number with Sunday as 0 (0-6) | 4 |
| W<sup>†</sup> | Week number with the first Monday as the first day of week one (00-53) | 34 |
| x<sup>†</sup> | Date representation | 08/23/01 |
| X<sup>†</sup> | Time representation | 14:55:02 |
| y<sup>†</sup> | Year, last two digits (00-99) | 01 |
| Y<sup>†</sup> | Year | 2001 |
| z<sup>†</sup> | ISO 8601 offset from UTC in timezone (1 minute=1, 1 hour=100). If timezone cannot be determined, no characters | +100 |
| Z<sup>†</sup> | Timezone name or abbreviation. If timezone cannot be determined, no characters | CDT |

†<sub>Descriptions and examples for time replacement macros provided by [cplusplus.com](http://www.cplusplus.com/reference/ctime/strftime/)</sub>

### Escape Sequence
A $ immediately followed by another $ will be replaced with a $.

## Directory Mode
If the template path is a directory, tmpl will recursively replace the names and contents of files and subdirectories at the output path.

## Build Instructions

### Checkout

```sh
git clone https://github.com/widberg/tmpl.git
cd tmpl
```

### Build
```sh
mkdir build
cd build
cmake ..
cmake --build .
```

### Install
```sh
cmake --build . --config Release --target install
```
