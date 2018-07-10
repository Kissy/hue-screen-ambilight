#include "stdafx.h"
#include <algorithm>
#include "ColorCube.h"
#include "CubeCell.h"
#include "CLocalMaximum.h"

#define COLOR_CUBE_RESOLUTION 30
#define COLOR_CUBE_SIZE COLOR_CUBE_RESOLUTION * COLOR_CUBE_RESOLUTION * COLOR_CUBE_RESOLUTION
#define CELL_INDEX(r,g,b) (r+g*COLOR_CUBE_RESOLUTION+b*COLOR_CUBE_RESOLUTION*COLOR_CUBE_RESOLUTION)

#define BRIGHT_COLOR_THRESHOLD 155
#define DARK_COLOR_THRESHOLD 100
#define DISTINCT_COLOR_THRESHOLD 0.2

static int neighbourIndices[27][3] = {
	{ 0, 0, 0 },
	{ 0, 0, 1 },
	{ 0, 0, -1 },

	{ 0, 1, 0 },
	{ 0, 1, 1 },
	{ 0, 1, -1 },

	{ 0, -1, 0 },
	{ 0, -1, 1 },
	{ 0, -1, -1 },

	{ 1, 0, 0 },
	{ 1, 0, 1 },
	{ 1, 0, -1 },

	{ 1, 1, 0 },
	{ 1, 1, 1 },
	{ 1, 1, -1 },

	{ 1, -1, 0 },
	{ 1, -1, 1 },
	{ 1, -1, -1 },

	{ -1, 0, 0 },
	{ -1, 0, 1 },
	{ -1, 0, -1 },

	{ -1, 1, 0 },
	{ -1, 1, 1 },
	{ -1, 1, -1 },

	{ -1, -1, 0 },
	{ -1, -1, 1 },
	{ -1, -1, -1 }
};

ColorCube::ColorCube() : cells(COLOR_CUBE_SIZE), localMaxima(100), filteredMaxima(0) {
}


ColorCube::~ColorCube() {
}

void ColorCube::quantizePixels(BYTE* pBuf, int pixelCount) {
	clear();

	int red, green, blue;
	int redIndex, greenIndex, blueIndex, cellIndex;
	for (int k = 0; k < pixelCount; k++) {
		blue = pBuf[k * 4 + 0];
		green = pBuf[k * 4 + 1];
		red = pBuf[k * 4 + 2];

		/*if (false) {
			if (red < BRIGHT_COLOR_THRESHOLD && green < BRIGHT_COLOR_THRESHOLD && blue < BRIGHT_COLOR_THRESHOLD) {
				continue;
			}
		}
		else if (false) {
			if (red >= DARK_COLOR_THRESHOLD || green >= DARK_COLOR_THRESHOLD || blue >= DARK_COLOR_THRESHOLD) {
				continue;
			}
		}*/

		redIndex = (int)(red * (COLOR_CUBE_RESOLUTION - 1) / 255.0f);
		greenIndex = (int)(green * (COLOR_CUBE_RESOLUTION - 1) / 255.0f);
		blueIndex = (int)(blue * (COLOR_CUBE_RESOLUTION - 1) / 255.0f);
		cellIndex = CELL_INDEX(redIndex, greenIndex, blueIndex);

		cells[cellIndex].hitCount++;
		cells[cellIndex].redAcc += red;
		cells[cellIndex].greenAcc += green;
		cells[cellIndex].blueAcc += blue;
	}
}

void ColorCube::updateLocalMaxima() {
	localMaxima.clear();

	BOOL isLocalMaximum = true;
	int localHitCount = 0, localCellIndex;
	int redIndex, greenIndex, blueIndex, cellIndex;

	for (int r = 0; r < COLOR_CUBE_RESOLUTION; r++) {
		for (int g = 0; g < COLOR_CUBE_RESOLUTION; g++) {
			for (int b = 0; b < COLOR_CUBE_RESOLUTION; b++) {
				localCellIndex = CELL_INDEX(r, g, b);
				localHitCount = cells[localCellIndex].hitCount;
				if (localHitCount == 0) {
					continue;
				}

				isLocalMaximum = true;
				for (int n = 0; n < 27; n++) {
					redIndex = r + neighbourIndices[n][0];
					greenIndex = g + neighbourIndices[n][1];
					blueIndex = b + neighbourIndices[n][2];
					cellIndex = CELL_INDEX(redIndex, greenIndex, blueIndex);
					if (cellIndex >= 0 && cellIndex < COLOR_CUBE_SIZE && cells[cellIndex].hitCount > localHitCount) {
						isLocalMaximum = false;
						break;
					}
				}

				if (!isLocalMaximum) {
					continue;
				}

				LocalMaximum maximum(localCellIndex, &cells[localCellIndex]);
				localMaxima.push_back(maximum);
			}
		}
	}

	std::sort(localMaxima.begin(), localMaxima.end());
}

std::vector<LocalMaximum> ColorCube::getMaxima() {
	return localMaxima;
}

LocalMaximum ColorCube::getMaximum(int index) {
	if (localMaxima.size() <= index) {
		return localMaxima[0];
	}
	return localMaxima[index];
}

void ColorCube::filterLocalMaxima() {
	filteredMaxima.clear();

	int size = localMaxima.size();
	for (int k = 0; k < size; k++) {
		LocalMaximum max1 = localMaxima[k];
		BOOL isDistinct = true;

		for (int n = 0; n<k; n++) {
			LocalMaximum max2 = localMaxima[n];
			double redDelta = max1.red - max2.red;
			double greenDelta = max1.green - max2.green;
			double blueDelta = max1.blue - max2.blue;
			double delta = sqrt(redDelta*redDelta + greenDelta*greenDelta + blueDelta*blueDelta);

			if (delta < DISTINCT_COLOR_THRESHOLD) {
				isDistinct = false;
				break;
			}
		}

		if (isDistinct) {
			filteredMaxima.push_back(max1);
		}
	}

	std::sort(filteredMaxima.begin(), filteredMaxima.end());
}

std::vector<LocalMaximum> ColorCube::getFilteredMaxima() {
	return filteredMaxima;
}

void ColorCube::clear() {
	for (int i = 0; i < COLOR_CUBE_SIZE; i++) {
		cells[i].clear();
	}
}