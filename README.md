# defmt
fmt is used to format arguments into a string, and defmt is used to deformat a string into arguments.
The relationship between them is similar to that of encoding and decoding.
defmt depends on [fast_float](https://github.com/fastfloat/fast_float), and some of the code is borrowed from [fmt](https://github.com/fmtlib/fmt).

The current version has only some basic features and will be improved in the future.
## example
```cpp
#include <iostream>
#include <string>
#include "defmt/defmt.hpp"

using namespace std;

int main() {
    string str{"hello kevin, your score is 99. Good job!"};
    int score = 0;
    auto ret = defmt::deformat("hello kevin, your score is {}. Good job!", str, score);
    if (ret.ec != std::errc())
        cout << "deformat failed!" << endl;
    else
        cout << "score is:" << score << " " << endl;
    return 0;
}


```


## TODO:
- [ ] support more types
- [ ] support custom types