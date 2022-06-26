#include "Scan.h"

Scan::Scan(string baseDir)
{
	allFiles = nullptr;
	allFilesNum = 0;
	markedFiles = nullptr;
	markedFilesRibs = nullptr;
	markedFilesSpine = 0;
	DIR* folder;
	struct dirent* folderData;
	queue<string> directories;			//fomappat es almappakat FIFO stackbe helyezi, mindig amikor talal mappat pushol, ha vegigjart mappat popol
	directories.push(baseDir);
	while (!directories.empty())
	{
		string parent = directories.front();
		folder = opendir(parent.c_str());
		if (folder == NULL)
		{
			throw DIR();
		}
		while ((folderData = readdir(folder)) != NULL) {
			if (strcmp(folderData->d_name, ".") != 0 && strcmp(folderData->d_name, "..") != 0)
			{
				string temp = parent + "/";
				temp += folderData->d_name;
				if (folderData->d_type == DT_DIR)
				{
					directories.push(temp);
				}
				else
				{
					addFile(temp, folderData);
				}
			}
		}
		closedir(folder);
		directories.pop();
	}
}



void Scan::markFiles()
{
	for (unsigned int i = 0; i < allFilesNum; i++)
	{
		for (unsigned int j = 0; j < i; j++)
		{
			if (*allFiles[j] == *allFiles[i])			//ha talalt ket fajl kozt egyezest, tobbit felesleges vegignezni
			{
				addMarkedFile(i, j);
				break;
			}
		}
	}
}

void Scan::addMarkedFile(unsigned int indexadd, unsigned int indexwhere)
{
	unsigned int i = 0;
	for (i; i < markedFilesSpine; i++)
	{
		if (markedFiles[i][0] == indexwhere)
			break;
	}
	if (i == markedFilesSpine)				//ha a tomb gerincen nem szerepel meg a fajl, amihez hozza akarjuk tenni az indexadd fajlt, akkor hozzon letre uj elemet a gerincen mindket indexu fajlnak
	{
		unsigned int** temp = new unsigned int* [i + 1];
		for (unsigned int j = 0; j < markedFilesSpine; j++)
			temp[j] = markedFiles[j];
		temp[i] = new unsigned int[2];
		delete[] markedFiles;
		markedFiles = temp;
		markedFiles[i][0] = indexwhere;
		markedFiles[i][1] = indexadd;
		markedFilesSpine++;
		unsigned int* tempRib = new unsigned int[markedFilesSpine];
		for (unsigned int j = 0; j < (markedFilesSpine - 1); j++)
			tempRib[j] = markedFilesRibs[j];
		tempRib[markedFilesSpine - 1] = 2;
		delete[] markedFilesRibs;
		markedFilesRibs = tempRib;
		return;
	}
	else								//ha letezik mar a gerincen, akkor ahhoz a bordahoz adja hozza
	{
		unsigned int* temp = new unsigned int[markedFilesRibs[i] + 1];
		for (unsigned int j = 0; j < markedFilesRibs[i]; j++)
			temp[j] = markedFiles[i][j];
		temp[markedFilesRibs[i]] = indexadd;
		delete[] markedFiles[i];
		markedFiles[i] = temp;
		markedFilesRibs[i]++;
		return;
	}
}

const File& Scan::getFile(unsigned int index) const
{
	return *allFiles[index];
}

const File& Scan::getMarkedFile(unsigned int indexmain, unsigned int indexsecondary ) const
{
	return *allFiles[markedFiles[indexmain][indexsecondary]];
}

void Scan::addFile(string locationparam, struct dirent* nameparam)
{
	File** temp;
	string formatlocal = getFormat(nameparam);
	temp = new File*[allFilesNum + 1];
	if (allFilesNum == 0)
	{
		if (strcmp(formatlocal.c_str(), ".txt") == 0)
		{
			temp[allFilesNum] = new TXT(locationparam, nameparam, formatlocal);
		}
		else
		{
			temp[allFilesNum] = new File(locationparam, nameparam, formatlocal);
		}
	}
	else
	{
		for (unsigned int i = 0; i < allFilesNum; i++)
		{
			temp[i] = allFiles[i];
		}
		if (strcmp(formatlocal.c_str(), ".txt") == 0)
		{
			temp[allFilesNum] = new TXT(locationparam, nameparam, formatlocal);
		}
		else
		{
			temp[allFilesNum] = new File(locationparam, nameparam, formatlocal);
		}
		delete[] allFiles;
	}
	allFilesNum++;
	allFiles = temp;
}

string Scan::getFormat(struct dirent* in) const
{
	char out[10];
	int i = 0;
	int j = 0;
	for (i = 0; in->d_name[i] != '\0'; i++);
	for (i; in->d_name[i] != '.'; i--);
	for (i, j; in->d_name[i] != '\0'; i++, j++)
	{
		out[j] = in->d_name[i];
	}
	out[j] = '\0';
	return out;
}

Scan::~Scan()
{
	if (allFilesNum == 0)
		return;
	for (unsigned int i = 0; i < allFilesNum; i++)
	{
		delete allFiles[i];
	}
	delete[] allFiles;
	for (unsigned int i = 0; i < markedFilesSpine; i++)
	{
		delete[] markedFiles[i];
	}
	delete[] markedFiles;
	delete[] markedFilesRibs;
}

void Scan::printMarkedFiles(string location)
{
	if (markedFilesSpine == 0)
	{
		cout << "There are no duplicate files in the given directory." << endl;
		return;
	}
	location += "/DeleteDupes.bat";
	fstream bat;
	bat.open(location, ios::out | ios::trunc);
	bat << "@echo off" << endl << "title Duplicate file deleting script" << endl;
	for (unsigned int i = 0; i < markedFilesSpine; i++)
	{
		cout << allFiles[markedFiles[i][0]]->getname() << " (at " << allFiles[markedFiles[i][0]]->getPath() << ") has duplicates, the following files will be deleted:" << endl << endl;
		for (unsigned int j = 1; j < markedFilesRibs[i]; j++)
		{
			cout << allFiles[markedFiles[i][j]]->getname() << " (at " << allFiles[markedFiles[i][j]]->getPath() << ")" << endl;
			string temp = allFiles[markedFiles[i][j]]->getPath();
			std::replace(temp.begin(), temp.end(), '/', '\\');
			bat << "del \"" << temp << "\"" << endl;
		}
		cout << endl << endl << endl;
	}
	cout << "The script for deleting the listed files has been created in the scanned base directory as \"DeleteDupes.bat\".";
	bat << "pause";
	bat.close();
}