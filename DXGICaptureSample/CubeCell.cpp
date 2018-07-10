#include "stdafx.h"
#include "CubeCell.h"


CubeCell::CubeCell() : hitCount(0), redAcc(0), greenAcc(0), blueAcc(0) {
}


CubeCell::~CubeCell() {
}

void CubeCell::clear() {
	hitCount = 0;
	redAcc = 0;
	greenAcc = 0;
	blueAcc = 0;
}
