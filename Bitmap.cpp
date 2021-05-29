#include "Bitmap.hpp"

void Bitmap::UpdateColumn(size_t columnNumber, int columnData)
{
	for (int rowNumber = 0; rowNumber < ROWS; rowNumber++)
		Pixels[rowNumber][columnNumber] = (columnData & (1 << rowNumber)) != 0;
}
