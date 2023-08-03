#include <iostream>
#include "D:/FPTA/developing code/text normailze/text normailze/text-normalize.h"


int main()
{
    unsigned char* result = new unsigned char[75];
    TextNormalize* obj = new TextNormalize(result, 75);
    obj->normalizeBuffer((const unsigned char*)"sample      text \t\t\t  dm,fsm, \n DFD/GDT‡‡‡‡df \v  fcx[ ] [  \f   ]xgfbxcv \r",(const size_t) 73);
}
