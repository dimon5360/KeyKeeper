#ifndef KEY_H
#define KEY_H

#include <string>
using namespace std;


class key
{
    string keyVal, secKey;
    unsigned int timeStamp;


public:
    key(string keyInput, string secKeyInput, unsigned int time);
};

#endif // KEY_H
