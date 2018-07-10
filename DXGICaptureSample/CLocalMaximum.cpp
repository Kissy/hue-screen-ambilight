#include "stdafx.h"
#include <cmath>
#include <cpprest/json.h>
#include "CLocalMaximum.h"


LocalMaximum::LocalMaximum() : hitCount(0), cellIndex(0), red(0), green(0), blue(0), weigth(0) {
}

LocalMaximum::LocalMaximum(int index, CubeCell* cubeCell) : hitCount(cubeCell->hitCount), cellIndex(index) {
	red = (int)(cubeCell->redAcc / cubeCell->hitCount);
	green = (int)(cubeCell->greenAcc / cubeCell->hitCount);
	blue = (int)(cubeCell->blueAcc / cubeCell->hitCount);
	updateWeigth();
}

LocalMaximum::~LocalMaximum() {
}


void LocalMaximum::updateWeigth() {
	/*int rg = std::abs(red - green);
	int rb = std::abs(red - blue);
	int gb = std::abs(green - blue);
	weigth = ((rg * rg + rb * rb + gb * gb) / 65535.0f * 50.0f + 1) * hitCount;*/
	/*if (red > 245 && green > 245 && blue > 245) {
	weigth = 0;
	} else {
	weigth = hitCount * ((red * red + green * green + blue * blue) / 65535.0f * 20.0f + 1);
	}*/
	if (red > 245 && green > 245 && blue > 245) {
		weigth = 0;
	}
	else if (red < 10 && green < 10 && blue < 10) {
		weigth = 0;
	}
	else {
		int rg = std::abs(red - green);
		int rb = std::abs(red - blue);
		int gb = std::abs(green - blue);
		weigth = ((rg * rg + rb * rb + gb * gb) / 65535.0f * 25.0f + 1.0f) * hitCount;
		//weigth = hitCount;
	}
}

float LocalMaximum::brightness() {
	return ((red * 299) + (green * 587) + (blue * 114)) / 1000.0f;
}

web::json::value LocalMaximum::getHueState() {
	float red = this->red / 255.0f;
	float green = this->green / 255.0f;
	float blue = this->blue / 255.0f;
	float min = fminf(fminf(red, green), blue);
	float max = fmaxf(fmaxf(red, green), blue);
	float delta = max - min;

	float hue = 0;
	float brightness = max;
	float saturation = max == 0 ? 0 : delta / max;

	if (max != 0) {
		if (red == max) {
			hue = (green - blue) / delta;
		}
		else {
			if (green == max) {
				hue = 2 + (blue - red) / delta;
			}
			else {
				hue = 4 + (red - green) / delta;
			}
		}

		hue *= 60;

		if (hue < 0) {
			hue += 360;
		}
	}

	int roundBri = (int)std::round(brightness * 255.0f);
	int roundSat = (int)std::round(saturation * 255.0f);
	int roundHue = (int)std::round(hue * 182.04f);
	printf("setting color to bri=%d, sat=%d, hue=%d\n", roundBri, roundSat, roundHue);
	web::json::value object;
	object[U("bri")] = roundBri;
	object[U("sat")] = roundSat;
	object[U("hue")] = roundHue;
	return object;
}

web::json::value LocalMaximum::getHueXYState() {
	float normalizedToOne[] = { this->red / 255.0f, this->green / 255.0f, this->blue / 255.0f };
	float red, green, blue;

	// Make red more vivid
	if (normalizedToOne[0] > 0.04045f) {
		red = pow((normalizedToOne[0] + 0.055f) / (1.0f + 0.055f), 2.4f);
	} else {
		red = normalizedToOne[0] / 12.92f;
	}

	// Make green more vivid
	if (normalizedToOne[1] > 0.04045f) {
		green = pow((normalizedToOne[1] + 0.055f) / (1.0f + 0.055f), 2.4f);
	} else {
		green = (float)(normalizedToOne[1] / 12.92f);
	}

	// Make blue more vivid
	if (normalizedToOne[2] > 0.04045) {
		blue = pow((normalizedToOne[2] + 0.055f) / (1.0f + 0.055f), 2.4f); 
	} else {
		blue = (float)(normalizedToOne[2] / 12.92f);
	}

	float X = (float)(red * 0.649926f + green * 0.103455f + blue * 0.197109f);
	float Y = (float)(red * 0.234327f + green * 0.743075f + blue * 0.022598f);
	float Z = (float)(red * 0.000000f + green * 0.053077f + blue * 1.035763f);

	float x = X / (X + Y + Z);
	float y = Y / (X + Y + Z);

	web::json::value xyList;
	xyList[0] = web::json::value::number(x);
	xyList[1] = web::json::value::number(y);
	web::json::value object;
	object[U("xy")] = xyList;
	return object;
}

bool LocalMaximum::operator < (const LocalMaximum& other) const {
	return weigth > other.weigth;
}