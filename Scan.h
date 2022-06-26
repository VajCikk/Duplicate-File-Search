#pragma once
#include "File.h"
#include "TXT.h"
#include <string>
#include <queue>
#include <iostream>
#include <algorithm>

class Scan
{
	File** allFiles;
	unsigned allFilesNum;
	unsigned **markedFiles;
	unsigned *markedFilesRibs;
	unsigned markedFilesSpine;
public:
	Scan(string baseDir);		//konstruktor, beolvassa a fomappa osszes fajljat, letrehozza a heterogen kollekciot. csak parameteresen hozhato letre objektum
	void markFiles();			//kigyujtott fajlok megjelolese egyezes alapjan
	void addMarkedFile(unsigned int indexadd, unsigned int indexwhere);		//hozzaad a ket dimenzios tombhoz egy uj jelolt fajl indexet
	const File& getFile(unsigned int index) const;							
	const File& getMarkedFile(unsigned int indexmain, unsigned int indexsecondary) const;
	void addFile( string, struct dirent*);			//hozzaad egy fajlt a heterogen kollekcio vegehez
	string getFormat(struct dirent* in) const;		//megadja egy fajl formatumat
	void printMarkedFiles(string);					//teljes output megformazasa es kiadasa 
	~Scan();
};

