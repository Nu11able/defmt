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
