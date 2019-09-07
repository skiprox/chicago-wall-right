#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofSetCircleResolution(100);
	width = ofGetWidth();
	height = ofGetHeight();
	cout << "WIDTH: " << width << " HEIGHT: " << height << endl;
	// Small square size
	smSqSize = fixedHeight/5.0;
	mySerial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = mySerial.getDeviceList();
	mySerial.setup(0, baud); //open the first device
	mySerial.startContinuousRead(false);
	ofAddListener(mySerial.NEW_MESSAGE,this,&ofApp::onNewMessage);
	message = "";
	setupMedia();
	setupAnimations();
}

//--------------------------------------------------------------
void ofApp::setupMedia(){
	// IMAGES
	background.load("images/background.png");
	companies.load("images/companies.png");
	ship.load("images/ship.png");
	plane.load("images/airplane.png");
	buildingLeft.load("images/building-left.png");
	buildingRight.load("images/building-right.png");
	// VIDEOS
	waterVideo.load("video/ripple.mp4");
	waterVideo.play();
}

//--------------------------------------------------------------
void ofApp::setupAnimations(){
	/**
	 * ALL THE ANIMATIONS GO HERE
	 */
	/**
	 * ALL THE LINE ANIMATIONS GO HERE
	 */
	vector<std::array<glm::vec2, 2>> pts;
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(fixedWidth/2.0, 860), glm::vec2(fixedWidth/2.0, 750)}});
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(fixedWidth/2.0, 750), glm::vec2(60, 750)}});
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(60, 750), glm::vec2(60, 625)}});
	leftHandMarkerLine = DashedLine(pts, 5.0, red, 250, false);
	/**
	 * ALL THE IMAGE ANIMATIONS GO HERE
	 */
	companiesAnimation = ImageAnimation(
		companies,
		glm::vec2(fixedWidth/2.0, fixedHeight - 120),
		glm::vec2(800, 184),
		250
	);
	buildingLeftAnimation = ImageAnimation(
		buildingLeft,
		glm::vec2(515, 540),
		glm::vec2(316, 296),
		250
	);
	buildingRightAnimation = ImageAnimation(
		buildingRight,
		glm::vec2(1085, 552),
		glm::vec2(342, 262),
		250
	);
	/**
	 * ALL THE IMAGE MOVEMENTS GO HERE
	 */
	planeMovement = ImageMovement(
		plane,
		glm::vec2(300, 95),
		glm::vec2(-300, -100),
		glm::vec2(250, 370),
		250
	);
	shipMovement = ImageMovement(
		ship,
		glm::vec2(300, 113),
		glm::vec2(2000, 800),
		glm::vec2(1725, 700),
		250
	);
	/**
	 * ALL THE TEXT TYPING GO HERE
	 */
	centerLeftText = TextTyping(
		"Lorem ipsum dolor sit amet, consectetur adipisicing\nelit, sed do eiusmod tempor incididunt ut labore et\ndolore magna aliqua. Ut enim ad minim veniam, quis\nnostrud exercitation ullamco laboris nisi ut aliquip\nex ea commodo consequat. Duis aute irure dolor in\nreprehenderit in voluptate velit esse cillum dolore\neu fugiat nulla pariatur. Excepteur sint occaecat\ncupidatat non proident, sunt in culpa qui officia\ndeserunt mollit anim id est laborum. Lorem ipsum dolor\nsit amet, consectetur adipisicing elit, sed do eiusmod\ntempor incididunt ut labore et dolore magna aliqua.\nUt enim ad minim veniam, quis nostrud exercitation\nullamco laboris nisi ut aliquip ex ea commodo consequat.\nDuis aute irure dolor in reprehenderit in voluptate\nvelit esse cillum dolore eu fugiat nulla pariatur.\nExcepteur sint occaecat cupidatat non proident, sunt in\nculpa qui officia deserunt mollit anim id est laborum.",
		glm::vec2(150, 780),
		ofColor(255),
		10,
		150
	);
	buildingLeftText = TextTyping(
		"Lorem ipsum dolor sit amet, consectetur adipisicing\nelit, sed do eiusmod tempor incididunt ut labore et\ndolore magna aliqua. Ut enim ad minim veniam, quis\nnostrud exercitation ullamco laboris nisi ut aliquip\nex ea commodo consequat. Duis aute irure dolor in\nreprehenderit in voluptate velit esse cillum dolore\neu fugiat nulla pariatur. Excepteur sint occaecat\ncupidatat non proident, sunt in culpa qui officia\ndeserunt mollit anim id est laborum. Lorem ipsum dolor\nsit amet, consectetur adipisicing elit, sed do eiusmod\ntempor incididunt ut labore et dolore magna aliqua.\nUt enim ad minim veniam, quis nostrud exercitation\nullamco laboris nisi ut aliquip ex ea commodo consequat.\nDuis aute irure dolor in reprehenderit in voluptate\nvelit esse cillum dolore eu fugiat nulla pariatur.\nExcepteur sint occaecat cupidatat non proident, sunt in\nculpa qui officia deserunt mollit anim id est laborum.",
		glm::vec2(690, 650),
		ofColor(255),
		10,
		150
	);
	buildingRightText = TextTyping(
		"Lorem ipsum dolor sit amet, consectetur adipisicing\nelit, sed do eiusmod tempor incididunt ut labore et\ndolore magna aliqua. Ut enim ad minim veniam, quis\nnostrud exercitation ullamco laboris nisi ut aliquip\nex ea commodo consequat. Duis aute irure dolor in\nreprehenderit in voluptate velit esse cillum dolore\neu fugiat nulla pariatur. Excepteur sint occaecat\ncupidatat non proident, sunt in culpa qui officia\ndeserunt mollit anim id est laborum. Lorem ipsum dolor\nsit amet, consectetur adipisicing elit, sed do eiusmod\ntempor incididunt ut labore et dolore magna aliqua.\nUt enim ad minim veniam, quis nostrud exercitation\nullamco laboris nisi ut aliquip ex ea commodo consequat.\nDuis aute irure dolor in reprehenderit in voluptate\nvelit esse cillum dolore eu fugiat nulla pariatur.\nExcepteur sint occaecat cupidatat non proident, sunt in\nculpa qui officia deserunt mollit anim id est laborum.",
		glm::vec2(1300, 700),
		ofColor(255),
		10,
		150
	);
	planeText = TextTyping(
		"Lorem ipsum dolor sit amet, consectetur adipisicing\nelit, sed do eiusmod tempor incididunt ut labore et\ndolore magna aliqua. Ut enim ad minim veniam, quis\nnostrud exercitation ullamco laboris nisi ut aliquip\nex ea commodo consequat. Duis aute irure dolor in\nreprehenderit in voluptate velit esse cillum dolore\neu fugiat nulla pariatur. Excepteur sint occaecat\ncupidatat non proident, sunt in culpa qui officia\ndeserunt mollit anim id est laborum. Lorem ipsum dolor\nsit amet, consectetur adipisicing elit, sed do eiusmod\ntempor incididunt ut labore et dolore magna aliqua.\nUt enim ad minim veniam, quis nostrud exercitation\nullamco laboris nisi ut aliquip ex ea commodo consequat.\nDuis aute irure dolor in reprehenderit in voluptate\nvelit esse cillum dolore eu fugiat nulla pariatur.\nExcepteur sint occaecat cupidatat non proident, sunt in\nculpa qui officia deserunt mollit anim id est laborum.",
		glm::vec2(250, 530),
		ofColor(255),
		10,
		150
	);
	shipText = TextTyping(
		"Lorem ipsum dolor sit amet, consectetur adipisicing\nelit, sed do eiusmod tempor incididunt ut labore et\ndolore magna aliqua. Ut enim ad minim veniam, quis\nnostrud exercitation ullamco laboris nisi ut aliquip\nex ea commodo consequat. Duis aute irure dolor in\nreprehenderit in voluptate velit esse cillum dolore\neu fugiat nulla pariatur. Excepteur sint occaecat\ncupidatat non proident, sunt in culpa qui officia\ndeserunt mollit anim id est laborum. Lorem ipsum dolor\nsit amet, consectetur adipisicing elit, sed do eiusmod\ntempor incididunt ut labore et dolore magna aliqua.\nUt enim ad minim veniam, quis nostrud exercitation\nullamco laboris nisi ut aliquip ex ea commodo consequat.\nDuis aute irure dolor in reprehenderit in voluptate\nvelit esse cillum dolore eu fugiat nulla pariatur.\nExcepteur sint occaecat cupidatat non proident, sunt in\nculpa qui officia deserunt mollit anim id est laborum.",
		glm::vec2(1590, 840),
		ofColor(255),
		10,
		150
	);
	/**
	 * ALL THE HAND MARKERS GO HERE
	 */
	// MILLENIUM FALCON
	handMarkers[0] = HandMarker(glm::vec2(60, fixedHeight - 100), red, true);
	// CENTER OF THE COMPANIES
	handMarkers[1] = HandMarker(glm::vec2(fixedWidth/2.0 + 40, fixedHeight - 115), red, false);
	// RIGHT OF THE COMPANIES
	handMarkers[2] = HandMarker(glm::vec2(fixedWidth/3.0 * 2.0 + 60, fixedHeight - 185), red, false);
	// LEFT OF THE SCREEN
	handMarkers[3] = HandMarker(glm::vec2(60, fixedHeight/2.0 + 80), red, true);
	// THE LEFT BUILDING
	handMarkers[4] = HandMarker(glm::vec2(510, 550), red, true);
	// THE RIGHT BUILDING
	handMarkers[5] = HandMarker(glm::vec2(1100, 530), red, true);
	// THE SHIP
	handMarkers[6] = HandMarker(glm::vec2(1705, 720), red, true);
	// THE AIRPLANE
	handMarkers[7] = HandMarker(glm::vec2(250, 390), red, true);
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
		// Check if it's our button first:
		if (i == serialInput.size() - 1) {
			// Reset stuff if we have our button press
			if (serialInput[i] == 0) {
				for (int i = 0; i < shouldRunAnimation.size(); i++) {
					shouldRunAnimation[i] = false;
					animationCounter[i] = 0;
				}
			}
		} else {
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
}

//--------------------------------------------------------------
void ofApp::onNewMessage(string & message)
{
	vector<string> input = ofSplitString(message, ",");
	serialInput.clear();
	cout << "THE MESSAGE " << message << endl;
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
	ofBackground(0);
	ofSetColor(255);
	// Draw the video
	waterVideo.update();
	waterVideo.draw(0, 0, fixedWidth, fixedHeight);
	// Draw up and down lines
	for (int i = 0; i < 2; i++) {
		ofDrawRectangle(fixedWidth/3.0 * (i + 1) - 1, 0, 2, fixedHeight);
		float divConst = 5.0;
		if (i == 0) {
			ofDrawRectangle(0, fixedHeight/divConst, fixedWidth, 2);
		} else {
			ofDrawRectangle(0, fixedHeight - fixedHeight/divConst, fixedWidth, 2);
		}
	}
	ofPopStyle();
	background.draw(0, 0, fixedWidth, fixedHeight);
}

//--------------------------------------------------------------
void ofApp::drawHandMarkers(){
	handMarkers[0].draw();
}

//--------------------------------------------------------------
void ofApp::drawAnimations(){
	bool runningAnyAnimations = false;
	// Run through the `shouldRunAnimation` array, and run any of the animations that we should
	for (int i = 0; i < shouldRunAnimation.size(); i++) {
		if (shouldRunAnimation[i]) {
			// We're running an animation! so this is true
			runningAnyAnimations = true;
			// Increase the animation counter,
			// which is what we're using to time things within the animation
			animationCounter[i]++;
			// Run the fucking animation
			runAnimation(i);
		}
	}
	// If we are running any animations, increment
	// the universal animation counter
	// and check if we should stop running all animations
	if (runningAnyAnimations) {
		universalAnimationCounter++;
		if (universalAnimationCounter >= universalAnimationCounterMax) {
			universalAnimationCounter = 0;
			for (int i = 0; i < shouldRunAnimation.size(); i++) {
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
	} else if (key == 32) { // Reset all animations
		for (int i = 0; i < shouldRunAnimation.size(); i++) {
			shouldRunAnimation[i] = false;
			animationCounter[i] = 0;
		}
	}
}

//--------------------------------------------------------------
void ofApp::checkShouldRunAnimations(int index){
	bool shouldRunOne = false;
	// If it's the first button, we should run the animation
	if (index == 0) { // The millenium falcon button
		// Run the companies animation
		shouldRunOne = true;
	} else if (index == 1) { // The bad companies center sensor
		if (shouldRunAnimation[0]) {
			// Run the left center button animation
			shouldRunOne = true;
		}
	} else if (index == 2) { // The bad companies right sensor
		if (shouldRunAnimation[1]) {
			// Run the buildings animation
			shouldRunOne = true;
		}
	} else if (index == 3) { // The center left button
		if (shouldRunAnimation[1]) {
			// Run the text associated with that button
			shouldRunOne = true;
		}
	} else if (index == 4) { // The left building button
		if (shouldRunAnimation[2]) {
			// Run the left building text
			shouldRunOne = true;
		}
	} else if (index == 5) { // The right building button
		if (shouldRunAnimation[2]) {
			// Run the right building text
			// and ship and plane
			shouldRunOne = true;
		}
	} else if (index == 6) { // The plane button
		if (shouldRunAnimation[5]) {
			// Run the plane text
			shouldRunOne = true;
		}
	} else if (index == 7) { // The ship button
		if (shouldRunAnimation[5]) {
			// Run the ship text
			shouldRunOne = true;
		}
	}
	if (shouldRunOne) {
		shouldRunAnimation[index] = true;
		universalAnimationCounter = 0;
	}
}

//--------------------------------------------------------------
void ofApp::runAnimation(int animationNum){
	switch(animationNum) {
		// Center button pressed,
		// companies show up
		case 0:
			ofPushStyle();
			companiesAnimation.update(animationCounter[0]);
			companiesAnimation.draw();
			handMarkers[1].draw();
			handMarkers[2].draw();
			ofPopStyle();
			break;
		// Company center button pressed,
		// left screen button shows up
		case 1:
			leftHandMarkerLine.update(animationCounter[1]);
			leftHandMarkerLine.draw();
			handMarkers[3].draw();
			break;
		// Company right button pressed,
		// buildings show up
		case 2:
			ofPushStyle();
			// Building Left
			buildingLeftAnimation.update(animationCounter[2]);
			buildingLeftAnimation.draw();
			handMarkers[4].draw();
			// Building Right
			buildingRightAnimation.update(animationCounter[2]);
			buildingRightAnimation.draw();
			handMarkers[5].draw();
			ofPopStyle();
			break;
		// Screen center left button pressed,
		// center left button text shows up
		case 3:
			ofPushStyle();
			centerLeftText.update(animationCounter[3]);
			centerLeftText.draw();
			ofPopStyle();
			break;
		// Left building pressed,
		// left building text shows up
		case 4: // The Left Building Text
			ofPushStyle();
			buildingLeftText.update(animationCounter[4]);
			buildingLeftText.draw();
			ofPopStyle();
			break;
		// Right building pressed,
		// ship and airplane show up, right building text shows up
		case 5:
			// Ship
			shipMovement.update(animationCounter[5]);
			shipMovement.draw();
			handMarkers[6].draw();
			// Airplane
			planeMovement.update(animationCounter[5]);
			planeMovement.draw();
			handMarkers[7].draw();
			// Building right text
			buildingRightText.update(animationCounter[5]);
			buildingRightText.draw();
			break;
		// Airplane button pressed
		// Airplane text shows up
		case 6:
			ofPushStyle();
			planeText.update(animationCounter[6]);
			planeText.draw();
			ofPopStyle();
			break;
		// Ship button pressed
		// ship text shows up
		case 7:
			ofPushStyle();
			shipText.update(animationCounter[7]);
			shipText.draw();
			ofPopStyle();
			break;
		default:
			// Do nothing, this is meaningless
			break;
	}
}
