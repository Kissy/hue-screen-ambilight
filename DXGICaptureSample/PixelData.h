#pragma once

#include "RgbColor.h"

class PixelData {
public:
	PixelData();
	~PixelData();

	bool operator < (const PixelData& other) const;

private:
	RgbColor color;
	int brightness;
	int count;
	double weight;

};

