#include "stdafx.h"
#include "PixelData.h"


PixelData::PixelData() : color(RgbColor()), brightness(0), count(0), weight(0) {
}


PixelData::~PixelData() {
}

bool PixelData::operator < (const PixelData& other) const {
	return other.count * other.weight < count * weight;
}