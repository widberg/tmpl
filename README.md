# tmpl
CLI template replacement tool

Usage:
```
tmpl template.txt output.txt arg0 arg1 arg2
```

template.txt
```
Example: $(0) $(1) $(2) $(TEMPLATE) $(OUTPUT) $(%c %Z)
```

output.txt
```
Example: arg0 arg1 arg2 template.txt output.txt 04/13/20 20:36:48 Eastern Daylight Time
```

Note: All replacement macros that are not an index or token will be interpreted as a time format string. Valid foramt specifiers for this use case can be found [here](http://www.cplusplus.com/reference/ctime/strftime/).