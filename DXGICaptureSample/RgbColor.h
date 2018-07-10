#pragma once



class RgbColor {
public:
	RgbColor();
	~RgbColor();

	bool match(const RgbColor& other);

	bool operator < (const RgbColor& other) const;

private:
	int red;
	int green;
	int blue;
	int degradation;

};

