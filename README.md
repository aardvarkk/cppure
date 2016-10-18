# cppure
## A basic profanity filter written in C++

### Include the header file and compile `cppure.cpp`

```
#include "cppure.h"
```

### Load a wordlist

```
cppure::load_wordlist("wordlist.txt");
```

### Cleanse a string

```
auto cleansed = cppure::cleanse("badword");
```