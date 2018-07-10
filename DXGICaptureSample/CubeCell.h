#pragma once

class CubeCell {

public:
	CubeCell();
	~CubeCell();

	void clear();

public:
	int hitCount;
	long redAcc;
	long greenAcc;
	long blueAcc;

};
