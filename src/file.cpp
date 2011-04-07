/*-------------------------------------------------------------------------
 *
 * file.cpp
 *     Thin wrapper around standard C buffered input routines 
 *
 * Copyright (c) 2011, Olexandr Melnyk <me@omelnyk.net>
 *
 *------------------------------------------------------------------------- 
 */

#include "file.hpp"

#include <cstdio>
#include <string>

file::file(const std::string &filename) : name(filename), pfile(NULL), offset(0), reached_eof(false) {};

std::string file::filename() const
{
	return name;
}

bool file::open()
{
	if (pfile)
		return true;
	if (!(pfile = fopen(name.c_str(), "r")))
		return false;
	fseek(pfile, offset, SEEK_SET);
	return true;
}

bool file::close()
{
	if (!pfile)
		return true;
	int success = !fclose(pfile);
	if (success)
		pfile = NULL;		
	return success;
}

bool file::eof() const
{
	return reached_eof;
}

size_t file::read(void *buf, size_t count)
{
	count = fread(buf, 1, count, pfile);
	offset += count;
	reached_eof = feof(pfile);
	return count;
}

