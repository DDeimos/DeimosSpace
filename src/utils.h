#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <filesystem>

using namespace std;
using namespace std::tr2::sys;

class Directory
{
public:

	static vector<string> GetFiles(string directory, string ext = "", bool onlyName = false)
	{
		vector<string> files;
		recursive_directory_iterator start(directory), end;

		for (; start != end; ++start)
		{
			path p = start->path();

			if (!is_directory(p))
			{
				if (ext == "" || ext == extension(p))
				{
					string file = onlyName ? p.filename() : p.string();
					files.push_back(file);
				}
			}
		}

		return files;
	}

	static vector<string> GetDirectories(string directory, bool onlyName = false)
	{
		vector<string> dirs;
		recursive_directory_iterator start(directory), end;

		for (; start != end; ++start)
		{
			path p = start->path();

			if (is_directory(p))
			{
				string dir = onlyName ? p.filename() : p.string();
				dirs.push_back(dir);
			}
		}

		return dirs;
	}
};

#endif