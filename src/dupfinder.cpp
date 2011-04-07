/*-------------------------------------------------------------------------
 *
 * dupfinder.cpp
 *     Command-line tool for finding duplicate files
 *
 * Copyright (c) 2011, Olexandr Melnyk <me@omelnyk.net>
 *
 *------------------------------------------------------------------------- 
 */

#include <map>
#include <vector>
#include <string>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/integer.hpp>
#include <boost/crc.hpp>

#include "vectormap.hpp"
#include "file.hpp"
#include "config.hpp"

using namespace std;
using namespace boost;
using namespace boost::filesystem;
 
inline uint32_t crc32(void const *buf, size_t bytes)
{
	crc_32_type result;
   	result.process_bytes(buf, bytes);
	return result.checksum();
}

void group_files_by_size(const path &dir, vector<vector<file> > &groups)
{
}

void find_duplicates(const path &dir, ostream &of, size_t block_size)
{
	vector<vector<file> > groups;
	char *buf = new char[HASHED_BLOCK_SIZE];

	// Perform initial grouping of files by their size

	vectormap<uintmax_t, file> files_by_size;
	for (recursive_directory_iterator iter(dir), end; iter != end; ++iter)
	{
		string name = iter->path().directory_string();
		if (!is_directory(name))
		{
			uintmax_t size = file_size(name);
			vectormap<uintmax_t, file>::iterator p = files_by_size.find(size);
			files_by_size.insert(size, file(name));
		}
	}

	for (vectormap<uintmax_t, file>::iterator it = files_by_size.begin(); it != files_by_size.end(); ++it)
		if (it->second.size() > 1) // ignore one-file groups	
			groups.push_back(it->second);

	files_by_size.clear(); // no longer needed

	// Analyze formed groups as a stack, by splitting them further when hashes of files chunks don't match

	while (!groups.empty())
	{
		vector<file> group = groups.back();
		groups.pop_back();

		if (!group.begin()->eof())
		{
			// Read a block of data from every file in the group, and compare their CRC32 hashes
			vectormap<uint32_t, file> files_by_hash; 
			for (vector<file>::iterator file = group.begin(); file != group.end(); ++file)
			{
				if (file->open())
				{
					int bytes_read = file->read(buf, block_size);
					files_by_hash.insert(crc32(buf, bytes_read), *file);
				}			
			}

			// Split group into smaller ones, according to the hashes of the last read blocks 
			for (vectormap<uint32_t, file>::iterator it = files_by_hash.begin(); it != files_by_hash.end(); ++it)
				if (it->second.size() > 1) // ignore one-file groups
					groups.push_back(it->second);
				else
					it->second.begin()->close();
		}
		else // EOF was reached and all hashes matched, print out file names
		{
			for (vector<file>::iterator file = group.begin(); file != group.end(); ++file)
				of << file->filename() << endl;				
			of << endl;
		}
	}

	delete []buf; 
}

int main(int argc, char *argv[])
{
	if (argc > 2)
	{
		cout << "Too many command-line arguments" << endl;
		return 1;
	}

	path dir(".");
	if (argc == 2)
	{
		dir = path(argv[1]);
		if (!exists(dir))
		{
			cout << "No such directory" << endl;
			return 1;
		}
		if (!is_directory(dir))
		{
			cout << "The specified path is not a directory" << endl;
			return 1;
		}
	} 

	find_duplicates(dir, cout, HASHED_BLOCK_SIZE);

	return 0;
}

