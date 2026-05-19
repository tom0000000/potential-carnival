#include "ofApp.h"

//--------------------------------------------------------------
ofApp::~ofApp() {
	soundStream.close();
	for (Raindrops* r : raining) {
		delete r; // give the memory back
	}
}
void ofApp::setup() {
	ofBackground(0);
	ofSetVerticalSync(true);

	/* shader */
	shader.load("shader");
	shader2.load("shader2");
	shader3.load("shader3");
	shader4.load("shader4");

	if (shader.isLoaded()) {
		std::cout << "yay" << endl;
	}

	/* osc */
	oscKick = 0;
	osc.setup(PORT);

	/* sound */
	sampleRate = 44100;
	bufferSize = 512;

	/* drawing */
	lAudio.assign(bufferSize, 0.0);
	rAudio.assign(bufferSize, 0.0);

	/* rain-ish animation*/
	ofPushStyle();
	ofNoFill();
	rainColumns = 200;
	for (int i = 0; i < rainColumns; i++) {
		raining.push_back(new Raindrops());
		raining[i]->setVelocity(ofRandom(10) + 5);
		raining[i]->setWidth(ofRandom(20));
		raining[i]->setRainAmount(ofRandom(10));
	}
	ofPopStyle();

	/* images */
	oscImgIter = 0;
	total = 200;
	images.resize(total);
	for (int i = 0; i < total; i++) {
		loader.loadFromDisk(images[i], "faces/face" + ofToString(i) + ".png");
	}

	/* turn on DAC */
	ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
	ofSoundStreamListDevices();
	soundStream.setDeviceID(0);
	soundStream.setup(this, 2, 2, sampleRate, bufferSize, 4); 
}

//--------------------------------------------------------------
void ofApp::update() {	
	/* parse OSC messages */
	while (osc.hasWaitingMessages()) {
		ofxOscMessage m;

		osc.getNextMessage(&m);
		if (m.getAddress() == "/drums") {
			oscKick = m.getArgAsFloat(0);
			oscSnare = m.getArgAsFloat(1);
			oscPerc = m.getArgAsFloat(2);
		}
		if (m.getAddress() == "/imgIter") {
			oscImgIter = m.getArgAsFloat(0);
			oscDrawImgs = m.getArgAsFloat(1);
		}
		if (m.getAddress() == "/voice2") {
			oscBeep1 = m.getArgAsFloat(0);
		}
		if (m.getAddress() == "/rainOn") {
			oscRain = m.getArgAsFloat(0);
		}
		if (m.getAddress() == "/shader4") {
			osc_shader4 = m.getArgAsFloat(0);
		}
	}
}
//--------------------------------------------------------------
void ofApp::draw() {
	
	//if (loader.bDoneLoading) 
	//{	/* draw shaders according to osc messages */

	
		shader.begin();
		shader.setUniform2f("u_resolution", ofGetWindowSize());
		shader.setUniform1f("u_time", ofGetElapsedTimef());
		shader.setUniform1f("u_p1", oscKick);
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
		shader.end();

		if (oscBeep1 > 0) {
			drawRandomRects();
		}

		shader2.begin();
		shader2.setUniform2f("u_resolution", ofGetWindowSize());
		shader2.setUniform1f("u_time", ofGetElapsedTimef());
		shader2.setUniform1f("u_p1", oscSnare);
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
		shader2.end();

		/* draw audio waveform */
		
		//drawTimeDomain(0);
		//drawTimeDomain(75); 
		
		shader3.begin();
		shader3.setUniform2f("u_resolution", ofGetWindowSize());
		shader3.setUniform1f("u_time", ofGetElapsedTimef());
		shader3.setUniform1f("u_p1", oscPerc);
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
		shader3.end();

		/* draw logos */
		if (oscDrawImgs == 1) {
			if (oscImgIter == 1) {
				count = (count + 1) % total;
			}
			
			iHeight = images[count].getHeight() * 0.7;
			iWidth = images[count].getWidth() * 0.7;
			images[count].draw(ofGetWidth() / 2 - (iWidth / 2), ofGetHeight() / 2 - (iHeight / 2), iWidth, iHeight);
		}
		if (oscRain) {
			ofSetColor(255);
			ofSetLineWidth(1);
			for (int r = 0; r < raining.size(); r++) {
				raining[r]->draw(r * (ofGetWidth() / rainColumns));
			}
		}

		if (osc_shader4) {
			std::cout << "osc_shader4: " << osc_shader4 << std::endl;
			shader4.begin();
			shader4.setUniform2f("u_resolution", ofGetWindowSize());
			shader4.setUniform1f("u_time", ofGetElapsedTimef());
			ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
			shader4.end();
		}
	//}
	//else {
	//	ofDrawBitmapString("loading...", ofGetWidth() / 2 - 50, ofGetHeight() / 2);
	//}
	ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate()), 10, 20);
}
//--------------------------------------------------------------
void ofApp::drawTimeDomain(int y) {
	ofPushMatrix();
	ofPushStyle();
	ofSetColor(255);
	
	ofNoFill();
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	ofBeginShape();
	for (int i = 0; i < lAudio.size(); i++) {
		for (int j = 0; j < 2; j++) {

			float lx = ofMap(i, 0, lAudio.size(), -ofGetWidth() / 2,
				ofGetWidth() / 2, true);
			ofVertex(lx, (lAudio[i] * 800.0f) - 100 + y);
		}
	}
	ofEndShape(false);
	ofBeginShape();
	for (int i = 0; i < rAudio.size(); i++) {
		for (int j = 0; j < 2; j++) {
			float rx = ofMap(i, 0, rAudio.size(), -ofGetWidth() / 2,
				ofGetWidth() / 2, true);
			ofVertex(rx, (rAudio[i] * 800.0f) + 100 - y);
		}
	}
	ofEndShape(false);
	ofPopMatrix();
	ofPopStyle();
}
//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels) {

	//for (int i = 0; i < bufferSize; i++) {
	//	lAudio[i] = input[i * nChannels + 2];
	//	rAudio[i] = input[i * nChannels + 3];
	//}
	for (int i = 0; i < bufferSize; i++) {
		lAudio[i] = input[i * nChannels];
		rAudio[i] = input[i * nChannels + 1];
	}
}
//--------------------------------------------------------------
void ofApp::drawRandomRects() {
	ofPushStyle();
	ofFill();
	if (oscBeep1 == 1) {
		ofSetColor(255, 0, 0);
		ofDrawRectangle(0, 0, ofGetWidth() / 3, ofGetHeight());
	}
	if (oscBeep1 == 2) {
		ofSetColor(0, 255, 0);
		ofDrawRectangle(ofGetWidth() / 3, 0, ofGetWidth() / 3, ofGetHeight());
	}
	if (oscBeep1 == 3) {
		
		ofSetColor(0, 0, 255);
		ofDrawRectangle((ofGetWidth() / 3) * 2, 0, ofGetWidth() / 3, ofGetHeight());
	}
	ofPopStyle();
}
//--------------------------------------------------------------
