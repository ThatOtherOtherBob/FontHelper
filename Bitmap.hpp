#pragma once
#include "32blit.hpp"
class Bitmap
{
public:
	static const size_t ROWS = 8, COLUMNS = 6;
	bool Pixels[Bitmap::ROWS][Bitmap::COLUMNS];
	void UpdateColumn(size_t columnNumber, int columnData);
};
