#ifndef UTILS_H
#define UTILS_H

#include <vector>

class Directory
{
public:

	static std::vector<std::string> GetFiles(std::string directory, std::string searchPattern)
	{
		WIN32_FIND_DATAA data;
		std::vector<std::string> files;

		std::string path = directory + searchPattern;
		HANDLE hFind = FindFirstFileA(path.c_str(), &data);

		if (hFind != INVALID_HANDLE_VALUE)
		{
			do 
			{
				files.push_back(std::string(data.cFileName));
			} while (FindNextFileA(hFind, &data));

			FindClose(hFind);
		}

		return files;
	}
};

#endif