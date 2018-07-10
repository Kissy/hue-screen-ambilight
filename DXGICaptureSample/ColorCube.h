#pragma once

#include <windows.h>
#include <vector>

class CubeCell;
class LocalMaximum;

class ColorCube {
public:
	ColorCube();
	~ColorCube();

	void quantizePixels(BYTE* pBuf, int pixelCount);

	void updateLocalMaxima();
	std::vector<LocalMaximum> getMaxima();
	LocalMaximum getMaximum(int index);

	void filterLocalMaxima();
	std::vector<LocalMaximum> getFilteredMaxima();

private:
	void clear();

	std::vector<CubeCell> cells;
	std::vector<LocalMaximum> localMaxima;
	std::vector<LocalMaximum> filteredMaxima;

};

