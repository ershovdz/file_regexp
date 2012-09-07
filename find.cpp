// find.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <iterator>
#include <algorithm>
#include <string> 

#include <boost/filesystem.hpp>
#include <boost/regex.hpp> 
#include <boost/algorithm/string.hpp> 

using namespace boost; 
using namespace std;
using namespace boost::filesystem;

inline void printFile(const string& path, const boost::regex& pattern)
{
	string norm_path = path;
	boost::replace_all(norm_path, "\\", "/");
	if (regex_search(norm_path, pattern))
	{
		std::cout << norm_path << endl;
	}
}

int main(int argc, char* argv[])
{
	string sCurDir;
	string sExpression;
	
	if(argc == 5)
	{
		sCurDir = argv[2];
		sExpression =  argv[4];
	}
	else if(argc == 3 && !strcmp(argv[1], "-R"))
	{
		sCurDir = "./";
		sExpression = argv[2];
	}
	else if(argc == 4 
		&& !strcmp(argv[1], "-F") 
		&& strcmp(argv[3], "-R"))
	{
		sCurDir = argv[2];
		sExpression = argv[3];
	}
	else if(argc == 2 
		&& strcmp(argv[1], "-F") 
		&& strcmp(argv[1], "-R"))
	{
		sCurDir = "./";
		sExpression = argv[1];
	}
	else
	{
		cout << "Usage: find.exe -F \"C:\\path\" -R \"*\\Debug\\*\"";
		return 0;
	}

	path current_dir(sCurDir); 
	if (!exists(current_dir))
	{
		cout << current_dir << " does not exist\n";
		return 0;
	}
	
	//prepare expression for searching
	{
		if(sExpression.substr(0, 1) == "*" 
			|| sExpression.substr(0, 1) == "?")
		{
			sExpression = sExpression.substr(1, -1);
		}
		boost::replace_all(sExpression, "\\", "/");
		boost::replace_all(sExpression, "\"", "");
		boost::replace_all(sExpression, ".", "\\.");
		boost::replace_all(sExpression, "*", "(\\w+)");
		boost::replace_all(sExpression, "?", "(\\w)");
		sExpression += "\$";
	}

	boost::regex pattern(sExpression/*, boost::regex::grep*/);

	if(argc == 4 || argc == 2) // non-recursive 
	{
		for (directory_iterator iter(current_dir), last; iter != last; ++iter)
		{
			printFile(iter->path().string(), pattern);
		}
	}
	else //recursive
	{
		for (recursive_directory_iterator iter(current_dir), last; iter != last; ++iter)
		{
			printFile(iter->path().string(), pattern);
		}
	}
	return 0;
}


