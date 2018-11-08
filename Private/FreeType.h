#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

struct FreeType
{
	FT_Library lib;

	static FreeType& get();

	bool init();
	void destroy();

private:
	FreeType() = default;
	FreeType(const FreeType&) = delete;
	FreeType& operator=(const FreeType&) = delete;

};

