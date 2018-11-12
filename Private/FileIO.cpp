#include "FileIO.h"
#include <fstream>


std::string readFile(const char * path)
{
	FILE* f = fopen(path, "rb");
	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	fseek(f, 0, SEEK_SET);

	std::string result;
	result.resize(size + 1);
	fread(&result[0], size, 1, f);
	fclose(f);

	return result;
}
