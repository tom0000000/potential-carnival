#pragma once

#include "ofMain.h"

class Raindrops
{
public:
	Raindrops();
	void setRainAmount(int _rainAmount);
	void setVelocity(float _velocity);
	void setWidth(float _width);
	void draw(int x);
	int getWidth();
	~Raindrops();	

private:
	int width;
	int rainAmount;
	int rainColumns;
	int velocity;
	int rainX;
	std::vector<int> length;
	std::vector<int> rainY;
	ofVec2f rainPos;
};

