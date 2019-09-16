#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofHideCursor();
	ofBackground(0);
	ofSetCircleResolution(100);
	width = ofGetWidth();
	height = ofGetHeight();
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
	waterVideo.load("video/ripple-looped.mp4");
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
	// The dashed line around the companies
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(fixedWidth/2.0, 860), glm::vec2(540, 860)}});
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(540, 860), glm::vec2(540, 1060)}});
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(540, 1060), glm::vec2(1370, 1060)}});
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1370, 1060), glm::vec2(1370, 860)}});
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1370, 860), glm::vec2(fixedWidth/2.0, 860)}});
	companiesLine = DashedLine(pts, 5.0, red, 250, false);
	pts.clear();
	// the dashed line to the left hand marker
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(fixedWidth/2.0, 860), glm::vec2(fixedWidth/2.0, 750)}});
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(fixedWidth/2.0, 750), glm::vec2(35, 750)}});
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(35, 750), glm::vec2(35, fixedHeight/2.0 + 25)}});
	leftHandMarkerLine = DashedLine(pts, 5.0, red, 250, false);
	pts.clear();
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
		glm::vec2(500, 578 + 40),
		glm::vec2(306, 287),
		250
	);
	buildingRightAnimation = ImageAnimation(
		buildingRight,
		glm::vec2(1050, 595 + 40),
		glm::vec2(338, 259),
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
	milleniumFalconText = TextTyping(
		"In 1915, Pich i Pon, the acting Mayor of Barcelona and owner of an electric utility company,\nproposed the creation of the Montjuïc fairground to host an International Exhibition for\nshowcasing the advances electric power industries. This became known as the 1929\nBarcelona International Exhibition, where the architect Mies van der Rohe designed the\nfamous German pavilion, an icon of modernism. The timing of the International Exhibition\ncoincided with The Great Depression, provoking the sudden abandonment of many\npavilions by its exhibitors. The International Exhibition was a financial disaster for the city,\nso much that the Spanish dictator Primo de Rivera censored any detail regarding its cost.\nTo pay all the debt that it had incurred, the State emitted bonds, but this strategy failed to\nraise the necessary capital. The debt was so high that a new special city tax had to be\nimplemented for paying 14 million pesetas every year for the subsequent 34 years.\nScandals over the misappropriation of funds continued to plague la Fira de Barcelona. In\nthe 1980s, when the municipal servants began preparing the 1992 Olympic Games, they\ndiscovered that the City Council was still paying the debt of the 1929 International\nExhibition.",
		glm::vec2(310, 950),
		ofColor(255),
		9,
		250
	);
	companiesCenterText = TextTyping(
		"Despite taking place in municipal land, the mega-events organized by Fira de Barcelona at the Montjuïc fairground use vast\npublic resources to support the propaganda of global capitalism and its corporate landscape. Their constant marketing, their\nconferences and macro concerts are part of the contemporary spectacle that treats citizens as a mass of consumers, indifferent\nto any future challenge to its well-being, such as the climate emergency, the disruption of artificial intelligence, the rise of neo-\nfascism, poverty, the wealth gap, labor exploitation or patriarchy. For decades, Fira de Barcelona has taken advantage of public\nassets to enrich the private corporations, their shareholders, and the perpetuation of the culture of mass consumption that they\nrepresent.",
		glm::vec2(fixedWidth/2.0, 800),
		ofColor(255),
		9,
		250
	);
	companiesRightText = TextTyping(
		"The neoliberal restructuring of the European Union\nproject had dramatic consequences on the socio-\nspatial configurations of its cities. In Spain, led by a\nseries of privatization friendly governments, a\nsignificant effort was made to make urban regions\nattractive to foreign direct investment. Barcelona, as\nmost other European cities, centered its objectives on\nbecoming competitive, prompting it to offer to global\nprivate investors an array of highly favorable fiscal,\ninfrastructural, and public incentives to attract their\ncapital. For the management of this new investment,\na powerful and opaque network of public-private\ncorporations was weaved. Apart from the port, the\nairport, Fira de Barcelona and the Chamber of\nCommerce, the Consortium of the Free-trade Zone,\nthe Tourism Consortium of Barcelona and\nMercabarna are remarkable knots in this network. The\nrole of these public-private partnerships has been to\nplunder municipal land, taxes, infrastructure, people,\nand goods from the public and give full control of\nthem to the global capitalist elites.",
		glm::vec2(1510, 900),
		ofColor(255),
		9,
		250
	);
	centerLeftText = TextTyping(
		"To extend the concession of the Montjuïc fairground to Fira de\nBarcelona and to allocate 380 million euros of public money to its\nrenovation project “Fira Univers” is a despotic, opaque, clientelistic,\nshort-sighted and anti-democratic imposition. It is an example of the\nextent to which global capitalism infringes upon public infrastructure\nand public resources to thrive. The decision of the City Council was\nannounced unilaterally and without making the conditions of the\nagreement public; it is unknown how many years would the\nconcession be extended. It leaves the future of the large piece of\ncentrally located municipal land in the hands of very influential lobbies,\nwith private interests and an opaque management structure. The\nstruggle of #LaFiraOLaVida is not only to protest against the blatant\nabuse of power but to co-design and co-develop an alternative and\nviable proposal for achieving the right to the city for all. There is still\ntime! Barcelona's civil society has until 2025 to organize an open,\ntransparent, rigorous, and genuinely participatory debate. The Urban\nFront will be there to support its organization.",
		glm::vec2(225, 670),
		ofColor(255),
		9,
		250
	);
	buildingLeftText = TextTyping(
		"For decades, Spanish housing policies have entirely centered on the\npromotion of private-ownership, fueled by mortgages, financialization,\nrent-deregulation, fiscal incentives for speculators, and all the\ncharacteristic traits of the global real estate markets. These neoliberal\npolicies were at the heart of the 2008 burst of the housing bubble,\nwhich provoked 500,000 evictions while the Spanish government\nrescued the banks with more than 60,000 million euros. In Barcelona,\nthe housing condition has worsened, and it is at a critical point. This is\ndue in part to the city's profile as a significant tourist attraction as well\nas for congress goers and organizers. In 2015, thanks to the electoral\nvictory of progressive mayor Ada Colau, the city approved the\nconstruction of 18,000 new affordable housing units for the next ten\nyears and passed legislation to force private developers to destine\n30\% of newly built units as affordable. Still, these policies have not\nbeen sufficient to curve the increasing gentrification and\ndisplacements. Today, more than 50\% pay over 40\% of their income\nto housing, while 30,000 families are waiting for social housing and\n80\% of youngsters are now incapable of leaving their parent's homes.",
		glm::vec2(690, 650),
		ofColor(255),
		9,
		250
	);
	buildingRightText = TextTyping(
		"With more than 50 million passengers in 2018, the Barcelona airport\nis the sixth most transited in the European Union. It is situated at 10\nminutes by car from the Montjuïc fairground. The airport is managed\nby a publicly-traded state enterprise known as AENA, a for-profit\nproperty of the Spanish state. The President of Fira de Barcelona,\nPau Relat, has been at work, lobbying for a multi-billion euro project\nto expand Barcelona's airport. The project argues for the urgent\nconstruction of a 4th landing strip to be built on the sea. ",
		glm::vec2(1180, 700),
		ofColor(255),
		9,
		250
	);
	planeText = TextTyping(
		"Barcelona is extremely vulnerable to the environmental crisis. At the global\nwarming level, t is a coastal city that is susceptive to water shortages and\ncyclical heatwaves. The predicted sea-level rise is expected to swallow\nbeach fronts and flood entire neighborhoods, like Barceloneta. The increase\nin temperatures will favor the introduction of tropical illnesses and the\nintensification of heavy storms as well as long term drought. On the local\nscale, Barcelona is one of the most polluted cities in Europe. Its low air\nquality is responsible for 3,500 premature deaths per year in its metropolitan\narea. Barcelona is also the European city most polluted by cruise ship fuel\nand the second throwing more plastic waste the Mediterranean Sea. If this\nwas not enough, it also has some of the densest motor traffic in Europe, at a\nratio of 7,000 vehicles per square kilometer —London has 1,200 and Paris\n1,500—. Over the years, Fira de Barcelona has hosted expositions for major\npolluting industries. Its 2 million annual visitors generate massive toxic\nemissions, water pollution, plastic waste, and energy consumption.",
		glm::vec2(250, 530),
		ofColor(255),
		9,
		250
	);
	shipText = TextTyping(
		"The port of Barcelona is the most important commodity\ntrade and cruise ship facility of the Mediterranean. It is\nmanaged by one of the most influential organizations in\nthe region, another opaque public-private entity called\nPort de Barcelona, with links to La Fira de Barcelona. It\nis rarely mentioned in the media, but has not been\nexempt from criminal proves related to illegal selling of\npublic space, as well as for corruption, illicit drug trade\nand its relationship to Russian, Chinese and Qatar\nmafias. It occupies a piece of public land more extensive\nthan the whole of Cerdá's Eixample district, fenced,\nsecured by its port police, guarded by cameras and\nsecurity infrastructure. It is a hidden site behind the\nMontjuïc hill where the vast majority of Barcelona’s\ncitizens have never set afoot.",
		glm::vec2(1490, 960),
		ofColor(255),
		9,
		250
	);
	/**
	 * ALL THE HAND MARKERS GO HERE
	 */
	// MILLENIUM FALCON
	handMarkers[0] = HandMarker(glm::vec2(50, fixedHeight - 260), red, true);
	// CENTER OF THE COMPANIES
	handMarkers[1] = HandMarker(glm::vec2(fixedWidth/2.0 + 85, fixedHeight - 108), red, false);
	// LEFT OF THE COMPANIES
	handMarkers[2] = HandMarker(glm::vec2(fixedWidth/3.0  + 30, fixedHeight - 108), red, false);
	// LEFT OF THE SCREEN
	handMarkers[3] = HandMarker(glm::vec2(35, fixedHeight/2.0 + 25), red, true);
	// THE LEFT BUILDING
	handMarkers[4] = HandMarker(glm::vec2(590, 520), red, true);
	// THE RIGHT BUILDING
	handMarkers[5] = HandMarker(glm::vec2(1085, 690), red, true);
	// THE SHIP
	handMarkers[6] = HandMarker(glm::vec2(1660, 600), red, true);
	// THE AIRPLANE
	handMarkers[7] = HandMarker(glm::vec2(175, 385), red, true);
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
		// Check if it's our reset button first:
		if (i == serialInput.size() - 1) {
			// Reset stuff if we have our button press
			if (serialInput[i] == 1) {
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
				if (touchThreshold[i] >= 10) {
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
	for (int i = 0; i < input.size() - 1; i++) {
		serialInput.push_back(ofToInt(input[i]));
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	drawBackground();
	drawHandMarkers();
	drawAnimations();
}

//--------------------------------------------------------------
void ofApp::drawBackground(){
	ofPushStyle();
	ofBackground(0);
	ofSetColor(255);
	// Draw the video
	waterVideo.update();
	waterVideo.draw(0, 0, fixedWidth, fixedHeight);
	ofSetColor(0);
	ofDrawRectangle(0, fixedHeight - 400, 400, 400);
	ofPopStyle();
	background.draw(projectorOffsetX, projectorOffsetY, fixedWidth, fixedHeight);
}

//--------------------------------------------------------------
void ofApp::drawHandMarkers(){
	if (!shouldRunAnimation[0]) {
		handMarkers[0].draw();
	}
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
		if (shouldRunAnimation[3]) {
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
			if (!shouldRunAnimation[1]) {
				// Companies center hand marker
				handMarkers[1].draw();
			}
			if (shouldRunAnimation[3] &&
				!shouldRunAnimation[2]) {
				// comapnies right hand marker
				handMarkers[2].draw();
			}
			// The companies image
			companiesAnimation.update(animationCounter[0]);
			companiesAnimation.draw();
			// companies line
			companiesLine.update(animationCounter[0]);
			companiesLine.draw();
			// Millenium falcon text
			if (!shouldRunAnimation[1] &&
				!shouldRunAnimation[2]) {
				milleniumFalconText.update(animationCounter[0]);
				milleniumFalconText.draw();
			}
			ofPopStyle();
			break;
		// Company center button pressed,
		// left screen button shows up
		case 1:
			// companies center stuff,
			// only if the left screen text isn't activated
			if (!shouldRunAnimation[3]) {
				leftHandMarkerLine.update(animationCounter[1]);
				leftHandMarkerLine.draw();
				companiesCenterText.update(animationCounter[1]);
				companiesCenterText.draw();
				// Left screen hand marker
				handMarkers[3].draw();
			}
			break;
		// Company right button pressed,
		// buildings show up
		case 2:
			ofPushStyle();
			// companies right text
			// only if the building text isn't active
			if (!shouldRunAnimation[4] &&
				!shouldRunAnimation[5]) {
				companiesRightText.update(animationCounter[2]);
				companiesRightText.draw();
			}
			// Building Left
			buildingLeftAnimation.update(animationCounter[2]);
			buildingLeftAnimation.draw();
			if (!shouldRunAnimation[4]) {
				handMarkers[4].draw();
			}
			// Building Right
			buildingRightAnimation.update(animationCounter[2]);
			buildingRightAnimation.draw();
			if (!shouldRunAnimation[5]) {
				handMarkers[5].draw();
			}
			ofPopStyle();
			break;
		// Screen center left button pressed,
		// center left button text shows up
		case 3:
			ofPushStyle();
			// Center left text
			// only if the plane text isn't activated
			if (!shouldRunAnimation[2]) {
				centerLeftText.update(animationCounter[3]);
				centerLeftText.draw();
			}
			ofPopStyle();
			break;
		// Left building pressed,
		// left building text shows up
		case 4: // The Left Building Text
			ofPushStyle();
			if (!shouldRunAnimation[6]) {
				buildingLeftText.update(animationCounter[4]);
				buildingLeftText.draw();
			}
			ofPopStyle();
			break;
		// Right building pressed,
		// ship and airplane show up, right building text shows up
		case 5:
			// Ship
			shipMovement.update(animationCounter[5]);
			shipMovement.draw();
			if (!shouldRunAnimation[7]) {
				handMarkers[6].draw();
			}
			// Airplane
			planeMovement.update(animationCounter[5]);
			planeMovement.draw();
			if (!shouldRunAnimation[6]) {
				handMarkers[7].draw();
			}
			// Building right text
			if (!shouldRunAnimation[7]) {
				buildingRightText.update(animationCounter[5]);
				buildingRightText.draw();
			}
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
