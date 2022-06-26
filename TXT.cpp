#include "TXT.h"

TXT::TXT() : File()
{
	lenght = 0;
}

void TXT::operator=(const TXT& other)
{
	path = other.path;
	name = other.name;
	format = other.format;
	lenght = other.lenght;
}

TXT::TXT(string pathparam, struct dirent* nameparam, string formatparam) : File(pathparam, nameparam, formatparam)
{
	ifstream read;
	read.open(pathparam, ios::in);
	lenght = 0;
	char currentChar;
	if (read.is_open())
	{
		while (read.get(currentChar))
		{
			if (currentChar == EOF)
			{
				break;
			}
			lenght++;
		}
	}
	else
	{

	}
	read.close();
}

bool TXT::operator==(const File& other) const
{
	ifstream first, second;
	first.open(path, ios::in);
	second.open(other.getPath(), ios::in);
	string otherformat = other.getFormat();
	char currentCharFirst, currentCharSecond;
	if (strcmp(format.c_str(), otherformat.c_str()) != 0)
	{
		return false;
	}
	if (first.is_open() && second.is_open())
	{
		while (1)
		{
			currentCharFirst = first.get();
			currentCharSecond = second.get();
			if (currentCharFirst == EOF && currentCharSecond == EOF)
			{
				first.close();
				second.close();
				return true;
			}
			if (currentCharFirst == EOF || currentCharSecond == EOF)
			{
				first.close();
				second.close();
				return false;
			}
			if (currentCharFirst != currentCharSecond)
			{
				first.close();
				second.close();
				return false;
			}
		}
	}
	else
	{

	}
}

long TXT::getLenght()
{
	return lenght;
}
