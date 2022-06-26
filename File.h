#pragma once
#include "dirent.h"
#include <string>
#include <fstream>
using namespace std;

class File
{
protected:
	string name;
	string path;
	string format;
public:
	File();
	File(string, struct dirent*, string);
	File(const File&);
	virtual bool operator==(const File& other) const;
	void setname(string param);
	void setPath(string param);
	string getname() const;
	string getPath() const;
	string getFormat() const;
	virtual void operator=(const File& other);
};

