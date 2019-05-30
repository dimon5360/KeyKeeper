#include "inc/key.h"

key::key(string keyInput, string secKeyInput, unsigned int time)
{
    keyVal = keyInput;
    secKey = secKeyInput;
    timeStamp = time;
}
