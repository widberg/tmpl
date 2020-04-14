# tmpl
CLI template replacement tool

### Usage
command
```
tmpl template.txt output.txt arg0 arg1 arg2
```

template.txt
```
Example: $(0) $(1) $(2) $(TEMPLATE) $(OUTPUT) $(%c %Z) $$
```

output.txt
```
Example: arg0 arg1 arg2 template.txt output.txt 04/13/20 20:36:48 Eastern Daylight Time $
```
### Indexes
Any replacement macro beginning with a numerical digit will be interpreted as an index. Each index is replaced by the argument passed to tmpl in that possition following the output file path. The fist argument following the output path is denoted by index 0.

### Token
Any replacement macro that matches a string bellow will be replaced with the value described to the right of the string. Tokens are case sensitive.

| Token | Description |
| --- | --- |
|TEMPLATE | The argument given as the template file path |
| OUTPUT | The argument given as the output file path |

### Time
All replacement macros that are not an index or token will be interpreted as a time format string. Valid foramt specifiers for this use case can be found [here](http://www.cplusplus.com/reference/ctime/strftime/).

### Escape Sequences
A $ immediately followed by another $ will be replaced by a $.

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
