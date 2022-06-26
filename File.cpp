#include "File.h"


File::File()
{
	path = nullptr;
	name = nullptr;
	format = nullptr;
}

File::File(string param, struct dirent* nameparam, string formatparam)
{
	path = param;
	name = nameparam->d_name;
	format = formatparam;
}

File::File( const File& other)
{
	path = other.path;
	name = other.name;
	format = other.format;
}

bool File::operator==(const File& other) const
{
	if (strcmp(format.c_str(), other.format.c_str()) != 0)
	{
		return false;
	}
	ifstream file, theOther;
	file.open(path, ios::binary);
	theOther.open(other.path, ios::binary);
	unsigned char buffer1[1024], buffer2[1024];
	while (!file.eof() || !theOther.eof())
	{
		file.read((char*)buffer1, sizeof(buffer1));
		theOther.read((char*)buffer2, sizeof(buffer2));
		if (file.gcount() != theOther.gcount())
		{
			file.close();
			theOther.close();
			return false;
		}

		for (int i = 0; i < file.gcount(); i++)
		{
			if (buffer1[i] != buffer2[i])
			{
				file.close();
				theOther.close();
				return false;
			}
		}
	}
	if (file.eof() == theOther.eof())
	{
		file.close();
		theOther.close();
		return true;
	}
	else
	{
		file.close();
		theOther.close();
		return false;
	}
}


void File::setname(string param)
{
	name = param;
}

void File::setPath(string param)
{
	path = param;
}

string File::getname() const
{
	return name;
}

string File::getPath() const
{
	return path;
}

void File::operator=(const File& other)
{
	this->name = other.name;
	this->path = other.path;
}

string File::getFormat() const
{
	return format;
}