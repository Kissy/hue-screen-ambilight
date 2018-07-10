#include "stdafx.h"
#include "RgbColor.h"


RgbColor::RgbColor() : red(0), green(0), blue(0), degradation(0) {
}


RgbColor::~RgbColor() {
}



bool RgbColor::operator < (const RgbColor& other) const {
	return red + green + blue < other.red + other.green + other.blue;
}

bool RgbColor::match(const RgbColor& other) {
	int r = red >> other.degradation;
	int g = green >> other.degradation;
	int b = blue >> other.degradation;
	return red == r && green == g && blue == b;
}