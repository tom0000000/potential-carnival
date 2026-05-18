#include "Raindrops.h"



Raindrops::Raindrops()
{
	width = 1;
}
void Raindrops::setRainAmount(int _rainAmount) {
	rainAmount = _rainAmount;
	for (int i = 0; i < rainAmount; i++) {
		length.push_back(ofRandom(40 + 20));
		rainY.push_back(ofRandom(ofGetHeight()));
	}
	
}

void Raindrops::setVelocity(float _velocity) {
	velocity = _velocity;
}
void Raindrops::setWidth(float _width) {
	width = _width;
}
int Raindrops::getWidth() {
	return width;
}
void Raindrops::draw(int _x) {
	ofPushStyle();
	int rainX = _x;
	ofNoFill();
	for (int i = 0; i < rainAmount; i++) {
		if (i % 3 == 0) {
			ofFill();
		}
		else {
			ofNoFill();
		}
		ofDrawRectangle(rainX, rainY[i], width, length[i]);
		if (rainY[i] > ofGetHeight()) {
			rainY[i] = 0;
		}
		else {
			rainY[i] += velocity;
		}
	}
	ofPopStyle();
}

Raindrops::~Raindrops()
{
}
