#pragma once

#include <complex>
#include "CubeCell.h"
#include <cpprest/json.h>

class LocalMaximum {
public:
	LocalMaximum();
	LocalMaximum(int index, CubeCell* cubeCell);
	~LocalMaximum();

	float brightness();
	web::json::value getHueState();
	web::json::value getHueXYState();

	bool operator < (const LocalMaximum& other) const;

public:
	int hitCount;
	int cellIndex;

	int red;
	int green;
	int blue;

	float weigth;

private:
	void updateWeigth();

};

