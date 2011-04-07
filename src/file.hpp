#ifndef DUPFINDER_FILE_HPP
#define DUPFINDER_FILE_HPP

#include <string>

class file
{
private:
	std::string name;
	FILE *pfile;
	long offset;
	bool reached_eof;

public:
	file(const std::string &filename);
	std::string filename() const;
	bool open();
	bool close();
	bool eof() const;
	size_t read(void *buf, size_t count);
};

#endif // DUPFINDER_FILE_HPP

