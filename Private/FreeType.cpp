#include "FreeType.h"
#include <iostream>

FreeType& FreeType::get()
{
	static FreeType ft{};
	return ft;
}

bool FreeType::init()
{
	if (FT_Init_FreeType(&lib))
	{
		std::cerr << "Could not init FreeType" << std::endl;
		return false;
	}

	return true;
}

void FreeType::destroy()
{
	FT_Done_FreeType(lib);
}
