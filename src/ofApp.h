#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxMaxim.h"
#include "ofxThreadedImageLoader.h"
#include "Raindrops.h"

#define PORT 7400

class ofApp : public ofBaseApp {

public:
	~ofApp();
	void setup();
	void update();
	void draw();

	/*audio*/
	ofSoundStream soundStream;
	void audioIn(float * input, int bufferSize, int nChannels);
	int sampleRate, bufferSize;

	/*drawing buffer for sound*/
	void drawTimeDomain(int y);
	std::vector<float> lAudio;
	std::vector<float> rAudio;

	/* shader */
	ofShader shader;
	ofShader shader2;
	ofShader shader3;
	ofShader shader4;

	/* osc */
	ofxOscReceiver osc;
	float oscKick, oscSnare, oscPerc, oscImgIter, oscDrawImgs, oscBeep1, oscRain, osc_shader4;

	/* images */
	ofxThreadedImageLoader  loader;
	vector<ofImage> images;
	int total, count, iHeight, iWidth;

	/* rain animation */
	vector<Raindrops*> raining;
	int rainColumns;
	
	void drawRandomRects();
};

