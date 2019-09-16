#pragma once

#include "ofMain.h"
#include "Constants.hpp"
#include "ofxSimpleSerial.h"
#include "Line.hpp"
#include "DashedLine.hpp"
#include "DottedLine.hpp"
#include "HandMarker.hpp"
#include "ImageAnimation.hpp"
#include "ImageMovement.hpp"
#include "TextTyping.hpp"

class ofApp : public ofBaseApp{

	public:
		void setup();
        void setupMedia();
        void setupAnimations();
		void update();
		void updateSerials();
		void draw();
		void drawBackground();
		void drawHandMarkers();
		void drawAnimations();
		void onNewMessage(string & message);
		void keyPressed(int key);
        void checkShouldRunAnimations(int index);
		void runAnimation(int animationNum);

		ofxSimpleSerial	mySerial;
		int baud = 57600;
        ofColor red = ofColor(220, 20, 30);
        ofColor white = ofColor(255, 255, 255);
    	int serial1 = 0;
    	int serial2 = 0;

    	float width;
    	float height;

        float fixedWidth = 1920.0;
        float fixedHeight = 1080.0;

        float smSqSize;

    	float universalCounter = 0.0;

    	string message;

    	vector<int> serialInput;

    	std::array<int, 8> touchThreshold = {{0, 0, 0, 0, 0, 0, 0, 0}};

    	std::array<bool, 8> shouldRunAnimation = {{false, false, false, false, false, false, false, false}};
    	std::array<int, 8> animationCounter = {{0, 0, 0, 0, 0, 0, 0, 0}};
        int universalAnimationCounter = 0;
        int universalAnimationCounterMax = 5000;

    	// All animations declared here

        // All arbitrary animation variables which we'll need to create animations
        vector<std::array<glm::vec2, 4>> _pts;

    	// All the hand markers declared here
    	std::array<HandMarker, 8> handMarkers;

        // All lines declared here
        DashedLine companiesLine,
            leftHandMarkerLine;

        // All images declared here
        ofImage background;
        ofImage companies;
        ofImage plane, ship, buildingLeft, buildingRight;

        // All Videos declared here
        ofVideoPlayer waterVideo;

        // All image animations
        ImageAnimation companiesAnimation,
            buildingLeftAnimation,
            buildingRightAnimation;

        // All image movements
        ImageMovement shipMovement,
            planeMovement;

        // All Text Typing animations
        TextTyping milleniumFalconText,
            companiesCenterText,
            companiesRightText,
            centerLeftText,
            buildingLeftText,
            buildingRightText,
            shipText,
            planeText;

        float projectorOffsetY = 75.0;
        float projectorOffsetX = -40.0;
        float companiesOffsetY = 14.0;
		
};
