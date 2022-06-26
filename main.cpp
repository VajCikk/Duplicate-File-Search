#include "Scan.h"

int main()
{
	char answer;
	while (1)
	{
		string directory;
		cout << "Type in the directory for scanning (format: C:/Example/ExampleSubdir):" << endl;
		cin >> directory;
		try {
			Scan read(directory);
			read.markFiles();
			read.printMarkedFiles(directory);
		}
		catch (DIR) {
			cout << "Could not open directory. Would you like to try again?" << endl << "Type 'Y' if yes, type anything else to quit." << endl << endl;
			answer = getchar();
			answer = getchar();
			if (answer == 'Y')
			{
				cout << endl;
				continue;
			}
		}
		catch (std::bad_alloc) {
			cout << "Could not allocate memory for storing file data." << endl;
		}
		catch (std::out_of_range) {
			cout << "Invalid index was given to one of the arrays." << endl;
		}
		catch (std::ifstream::failure)
		{
			cout << "Could not open/close one of the file streams.";
		}
		return 0;
	}
}