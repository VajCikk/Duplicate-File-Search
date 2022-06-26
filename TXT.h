#pragma once
#include "File.h"
class TXT :
    public File
{
    long lenght;
public:
    TXT();
    TXT(string pathparam, struct dirent* nameparam, string formatparam);
    bool operator==(const File& other) const;
    long getLenght();
    void operator=(const TXT& other);

};

