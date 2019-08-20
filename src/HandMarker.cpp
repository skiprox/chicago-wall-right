//
//  HandMarker.cpp
//  HandMarkers
//
//
//

#include "HandMarker.hpp"

HandMarker::HandMarker()
{
	// If we're doing a default one, place it off screen
	center = glm::vec2(-200, -200);
	color = ofColor(240, 60, 40);
    setup();
}

HandMarker::HandMarker(glm::vec2 _center, ofColor _color) {
	center = _center;
	color = _color;
    setup();
}

void HandMarker::setup() {
	ofSetCircleResolution(100);
}

void HandMarker::update(float _inc) {
	inc = _inc;
}

void HandMarker::draw() {
	ofPushStyle();
	ofSetColor(color);
	float radius = 20 + 8 * sin(inc);
	ofFill();		// draw "filled shapes"
	ofDrawCircle(center.x, center.y, radius);
	ofNoFill();
	ofSetColor(color);
	ofDrawCircle(center.x, center.y, 30);
	ofPopStyle();
}
