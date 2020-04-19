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
Example: argument zero arg1 arg2 template.txt D:\programming\widberg\tmpl\build\outputarg1.txt 04/15/20 13:36:14 Eastern Daylight Time $
```

## Replacement Macros
A replacement macro is denoted by a $ followed by a series of one or more digits, an individual alphabetical character, or a string of one or more alphanumeric characters enclosed in a pair of parentheses. Replacement macros in the template file will be replaced with a corresponding value in the output file. File names and directory names will be evaluated for replacement macros as well. The two types of replacement macro are listed bellow.

### Indexes
Any replacement macro beginning with a numerical digit will be interpreted as an index. Each index is replaced by the argument passed to tmpl in that position following the output path. The first argument following the output path is denoted by index 0. Following argument indexes sequentially increment by 1.

### Token
Any replacement macro that matches a string bellow will be replaced with the described value. Tokens are case sensitive.

| Token | Description | Example |
| --- | --- | --- |
| TEMPLATE_ARGUMENT | The argument given as the template path | template.txt |
| OUTPUT_ARGUMENT | The argument given as the output path | output$1.txt |
| TEMPLATE_ABSOLUTE<sup>‡</sup> | The absolute path to the current template file after replacement | D:\programming\widberg\tmpl\build\template.txt |
| OUTPUT_ABSOLUTE<sup>‡</sup> | The absolute path to the current output file after replacement | D:\programming\widberg\tmpl\build\outputarg1.txt |
| TEMPLATE_RELATIVE<sup>‡</sup> | The relative path to the current template file after replacement | template.txt |
| OUTPUT_RELATIVE<sup>‡</sup> | The relative path to the current output file after replacement | outputarg1.txt |
| TEMPLATE_DIRECTORY_ABSOLUTE<sup>‡</sup> | The absolute path to the directory of the current template file after replacement | D:\programming\widberg\tmpl\build |
| OUTPUT_DIRECTORY_ABSOLUTE<sup>‡</sup> | The absolute path to the directory of the current output file after replacement | D:\programming\widberg\tmpl\build |
| TEMPLATE_DIRECTORY_RELATIVE<sup>‡</sup> | The relative path to the directory of the current template file after replacement |  |
| OUTPUT_DIRECTORY_RELATIVE<sup>‡</sup> | The relative path to the directory of the current output file after replacement. |  |
| WORKING_DIRECTORY | The absolute path to the directory tmpl was executed from | D:\programming\widberg\tmpl\build |
| a<sup>†</sup> | Abbreviated weekday name | Wed |
| A<sup>†</sup> | Full weekday name | Wednesday |
| b<sup>†</sup> | Abbreviated month name | Apr |
| B<sup>†</sup> | Full month name | April |
| c<sup>†</sup> | Date and time representation | 04/15/20 13:36:14 |
| C<sup>†</sup> | Year divided by 100 and truncated to integer | 20 |
| d<sup>†</sup> | Day of the month, zero-padded (01-31)	| 15 |
| D<sup>†</sup> | Short MM/DD/YY date, equivalent to $m/$d/$y | 04/15/20 |
| e<sup>†</sup> | Day of the month, space-padded ( 1-31) | 15 |
| E | Seconds since UNIX Epoch (00:00:00, 1 Jan 1970 UTC) | 1586968574 |
| F<sup>†</sup> | Short YYYY-MM-DD date, equivalent to $Y-$m-$d	| 2020-04-15 |
| g<sup>†</sup> | Week-based year, last two digits (00-99) | 20 |
| G<sup>†</sup> | Week-based year | 2020 |
| h<sup>†</sup> | Abbreviated month name (same as $b) | Apr |
| H<sup>†</sup> | Hour in 24h format (00-23) | 13 |
| I<sup>†</sup> | Hour in 12h format (01-12) | 01 |
| j<sup>†</sup> | Day of the year (001-366)	| 106 |
| m<sup>†</sup> | Month as a decimal number (01-12) | 04 |
| M<sup>†</sup> | Minute (00-59) | 36 |
| p<sup>†</sup> | AM or PM designation | PM |
| r<sup>†</sup> | 12-hour clock time | 01:36:14 |
| R<sup>†</sup> | 24-hour HH:MM time, equivalent to $H:$M | 13:36 |
| S<sup>†</sup> | Second (00-61) | 14 |
| T<sup>†</sup> | ISO 8601 time format (HH:MM:SS), equivalent to $H:$M:$S | 13:36:14 |
| u<sup>†</sup> | ISO 8601 weekday as number with Monday as 1 (1-7) | 3 |
| U<sup>†</sup> | Week number with the first Sunday as the first day of week one (00-53) | 15 |
| V<sup>†</sup> | ISO 8601 week number (01-53) | 16 |
| w<sup>†</sup> | Weekday as a decimal number with Sunday as 0 (0-6) | 3 |
| W<sup>†</sup> | Week number with the first Monday as the first day of week one (00-53) | 15 |
| x<sup>†</sup> | Date representation | 04/15/20 |
| X<sup>†</sup> | Time representation | 13:36:14 |
| y<sup>†</sup> | Year, last two digits (00-99) | 20 |
| Y<sup>†</sup> | Year | 2020 |
| z<sup>†</sup> | ISO 8601 offset from UTC in timezone (1 minute=1, 1 hour=100). If timezone cannot be determined, no characters | -0400 |
| Z<sup>†</sup> | Timezone name or abbreviation. If timezone cannot be determined, no characters | Eastern Daylight Time |
| $ | Escape sequence. A $ immediately followed by another $ will be replaced with a $. | $ |

‡<sub>Macro unavailable during filename replacement.</sub>
†<sub>Description provided by [cplusplus.com](http://www.cplusplus.com/reference/ctime/strftime/).</sub>

## Directory Mode
If the template path is a directory, tmpl will recursively replace the names and contents of files and subdirectories at the output path.

## Interactive Mode
If the template path is `--stdin`, tmpl will use the input stream as the template file. If the output path is `--stdout`, tmpl will use the output stream as the output file. If either the template path or output path are stream arguments, the other must be a stream argument or file path. Directory mode is incompatible with interactive mode.

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
