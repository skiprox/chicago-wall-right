#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofSetCircleResolution(100);
	width = ofGetWidth();
	height = ofGetHeight();
	// Small square size
	smSqSize = height/5.0;
	mySerial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = mySerial.getDeviceList();
	mySerial.setup(0, baud); //open the first device
	mySerial.startContinuousRead();
	ofAddListener(mySerial.NEW_MESSAGE,this,&ofApp::onNewMessage);
	message = "";
	setupImages();
	setupAnimations();
}

//--------------------------------------------------------------
void ofApp::setupImages(){
	background.load("images/background.png");
	companies.load("images/companies.png");
	ship.load("images/ship.png");
	airplane.load("images/airplane.png");
	buildingLeft.load("images/building-left.png");
	buildingRight.load("images/building-right.png");
}

//--------------------------------------------------------------
void ofApp::setupAnimations(){
	/**
	 * ALL THE ANIMATIONS GO HERE
	 */
	// NOTHING TO SEE HERE SO FAR
	/**
	 * ALL THE HAND MARKERS GO HERE
	 */
	// MILLENIUM FALCON
	handMarkers[0] = HandMarker(glm::vec2(0, height - 100), red);
	// ABOVE THE COMPANIES
	handMarkers[1] = HandMarker(glm::vec2(width/3.0 - 80, height - 200), red);
	// THE LEFT BUILDING
	handMarkers[2] = HandMarker(glm::vec2(380, 380), red);
	// THE RIGHT BUILDING
	handMarkers[3] = HandMarker(glm::vec2(660, 460), red);
	// THE SHIP
	handMarkers[4] = HandMarker(glm::vec2(1050, 125), red);
	// THE AIRPLANE
	handMarkers[5] = HandMarker(glm::vec2(560, 300), red);
}

//--------------------------------------------------------------
void ofApp::update(){
	updateSerials();
	if (fabs(universalCounter - TWO_PI) < 0.0001) {
		universalCounter = 0.0f;
	} else {
		universalCounter = (universalCounter + PI/100.0);
	}
	for (int i = 0; i < handMarkers.size(); i++) {
		handMarkers[i].update(universalCounter);
	}
}

//--------------------------------------------------------------
void ofApp::updateSerials(){
	// Loop through the serial inputs
	for (int i = 0; i < serialInput.size(); i++) {
		// If we have something going on with the serial input...
		if (serialInput[i] == 1) {
			// Increase the threshold number,
			touchThreshold[i]++;
			// and once that reaches 10 we can run the animation
			if (touchThreshold[i] >= 50) {
				// Reset the threshold
				touchThreshold[i] = 0;
				checkShouldRunAnimations(i);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::onNewMessage(string & message)
{
	vector<string> input = ofSplitString(message, ",");
	serialInput.clear();
	for (int i = 0; i < input.size() - 1; i++) {
		serialInput.push_back(ofToInt(input[i]));
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	drawBackground();
	drawAnimations();
	drawHandMarkers();
}

//--------------------------------------------------------------
void ofApp::drawBackground(){
	ofPushStyle();
	ofBackground(255);
	ofSetColor(0);
	// Draw up and down lines
	for (int i = 0; i < 2; i++) {
		ofDrawRectangle(width/3.0 * (i + 1) - 1, 0, 2, height);
		float divConst = 5.0;
		if (i == 0) {
			ofDrawRectangle(0, height/divConst, width, 2);
		} else {
			ofDrawRectangle(0, height - height/divConst, width, 2);
		}
	}
	ofPopStyle();
	background.draw(0, 0, width, height);
}

//--------------------------------------------------------------
void ofApp::drawHandMarkers(){
	handMarkers[0].draw();
	// for (int i = 0; i < handMarkers.size(); i++) {
	// 	handMarkers[i].draw();
	// }
}

//--------------------------------------------------------------
void ofApp::drawAnimations(){
	// Run through the `shouldRunAnimation` array, and run any of the animations that we should
	for (int i = 0; i < shouldRunAnimation.size(); i++) {
		if (shouldRunAnimation[i]) {
			// Increase the animation counter,
			// which is what we're using to run the animation for
			// a set amount of time
			animationCounter[i]++;
			// If we haven't hit the threshold for how long to
			// run the animation, fucking run it
			if (animationCounter[i] <= animationCounterMax[i]) {
				runAnimation(i + 1);
			} else { // Otherwise stop running the animation
				shouldRunAnimation[i] = false;
				animationCounter[i] = 0;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	// Keys 1 through 9 are 49 through 57
	// so use this to determine the index of the animation we should run
	if (key >= 49 && key <= 57) {
		int keyIndex = key - 49;
		// Simulate increasing the threshold number,
		touchThreshold[keyIndex] = 50;
		// and once that reaches 50 we can run the animation
		if (touchThreshold[keyIndex] >= 50) {
			// Reset the threshold
			touchThreshold[keyIndex] = 0;
			checkShouldRunAnimations(keyIndex);
		}
	}
}

//--------------------------------------------------------------
void ofApp::checkShouldRunAnimations(int index){
	// If it's the first button, we should run the animation
	if (index == 0) {
		cout << "WE FUCKING DID IT!!!!" << endl;
		shouldRunAnimation[index] = true;
	} else if (index == 1 || index == 2) {
		cout << "WHY ARENT WE DOING THIS???" << index << endl;
		if (shouldRunAnimation[0]) {
			shouldRunAnimation[index] = true;
		}
	} else if (index == 3 || index == 4) {
		if (shouldRunAnimation[1] || shouldRunAnimation[2]) {
			shouldRunAnimation[index] = true;
		}
	}
}

//--------------------------------------------------------------
void ofApp::runAnimation(int animationNum){
	switch(animationNum) {
		case 1:
			ofPushStyle();
			handMarkers[1].draw();
			companies.draw(width/3.0 - 100, height - 240, 800, 200);
			ofPopStyle();
			break;
		case 2:
			ofPushStyle();
			// Building Left
			buildingLeft.draw(280, 350, 300, 280);
			handMarkers[2].draw();
			// Building Right
			buildingRight.draw(600, 400, 300, 230);
			handMarkers[3].draw();
			// Ship
			ship.draw(width - 380, 100, 300, 113);
			handMarkers[4].draw();
			// Airplane
			airplane.draw(width/2.0 - 140, 300, 140, 45);
			handMarkers[5].draw();
			ofPopStyle();
			break;
		case 3:
			ofPushStyle();
			ofSetColor(red);
			ofDrawRectangle(width - width/3.0, 0, width/3.0, height/5.0);
			ofPopStyle();
			break;
		case 4:
			ofPushStyle();
			ofSetColor(red);
			ofDrawRectangle(0, height/5.0, width/3.0, height - 2.0 * (height/5.0));
			ofPopStyle();
			break;
		case 5:
			ofPushStyle();
			ofSetColor(red);
			ofDrawRectangle(width/3.0, height/5.0, width/3.0, height - 2.0 * (height/5.0));
			ofPopStyle();
			break;
		case 6:
			ofPushStyle();
			ofSetColor(red);
			ofDrawRectangle(width - width/3.0, height/5.0, width/3.0, height - 2.0 * (height/5.0));
			ofPopStyle();
			break;
		case 7:
			ofPushStyle();
			ofSetColor(red);
			ofDrawRectangle(0, height - height/5.0, width/3.0, height/3.0);
			ofPopStyle();
			break;
		case 8:
			ofPushStyle();
			ofSetColor(red);
			ofDrawRectangle(width/3.0, height - height/5.0, width/3.0, height/3.0);
			ofPopStyle();
			break;
		case 9:
			ofPushStyle();
			ofSetColor(red);
			ofDrawRectangle(width/3.0 * 2, height - height/5.0, width/3.0, height/3.0);
			ofPopStyle();
			break;
		default:
			cout << "WHAT FUCKING KEY IS THIS?? " << animationNum << endl;
			break;
	}
}
