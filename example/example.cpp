#include <iostream>
#include <string>
#include "defmt/defmt.hpp"

using namespace std;

int main() {
    string str{"hello kevin, your score is 99. Good job!"};
    std::string name;
    int score = 0;
    auto ret = defmt::deformat("hello {}, your score is {}. Good job!", str, name, score);
    if (ret.ec != std::errc())
        cout << "deformat failed!" << endl;
    else
        cout << name << ":" << score << endl;
    return 0;
}
