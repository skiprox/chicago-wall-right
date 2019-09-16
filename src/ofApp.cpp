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
	 * ALL THE PRESS POINTS
	 */
	// 0, millenium
	pressPts.push_back(array<glm::vec2, 2> {{glm::vec2(370, 970), glm::vec2(150, 860)}});
	pressLines.push_back(DottedLine(pressPts, 5.0, red, 75, true));
	pressPts.clear();
	// 1, companies center
	pressPts.push_back(array<glm::vec2, 2> {{glm::vec2(745, 760), glm::vec2(970, 835)}});
	pressLines.push_back(DottedLine(pressPts, 5.0, red, 75, true));
	pressPts.clear();
	// 2, companies left
	pressPts.push_back(array<glm::vec2, 2> {{glm::vec2(275, 1000), glm::vec2(490, 970)}});
	pressLines.push_back(DottedLine(pressPts, 5.0, red, 75, true));
	pressPts.clear();
	// 3, left screen side
	pressPts.push_back(array<glm::vec2, 2> {{glm::vec2(250, 475), glm::vec2(93, 556)}});
	pressLines.push_back(DottedLine(pressPts, 5.0, red, 75, true));
	pressPts.clear();
	// 4. building left
	pressPts.push_back(array<glm::vec2, 2> {{glm::vec2(280, 490), glm::vec2(440, 500)}});
	pressLines.push_back(DottedLine(pressPts, 5.0, red, 75, true));
	pressPts.clear();
	// 5. building right
	pressPts.push_back(array<glm::vec2, 2> {{glm::vec2(816, 776), glm::vec2(995, 730)}});
	pressLines.push_back(DottedLine(pressPts, 5.0, red, 75, true));
	pressPts.clear();
	// 6, plane
	pressPts.push_back(array<glm::vec2, 2> {{glm::vec2(75, 575), glm::vec2(145, 440)}});
	pressLines.push_back(DottedLine(pressPts, 5.0, red, 75, true));
	pressPts.clear();
	// 7, ship
	pressPts.push_back(array<glm::vec2, 2> {{glm::vec2(1460, 912), glm::vec2(1548, 796)}});
	pressLines.push_back(DottedLine(pressPts, 5.0, red, 75, true));
	pressPts.clear();
	/**
	 * ALL THE TEXT TYPING GO HERE
	 */
	milleniumFalconText = TextTyping(
		"HISTORY OF THE MONTJUÏC FAIRGROUND\n\nIn 1915, Pich i Pon, the acting Mayor of Barcelona\nand owner of an electric utility company, proposed\nthe creation of the Montjuïc fairground to host an\nInternational Exhibition for showcasing the\nadvances electric power industries. This became\nknown as the 1929 Barcelona International Exhibi-\ntion, where the architect Mies van der Rohe\ndesigned the famous German pavilion, an icon of\nmodernism. The timing of the International Exhibi-\ntion coincided with The Great Depression, provok-\ning the sudden abandonment of many pavilions by\nits exhibitors. The International Exhibition was a\nfinancial disaster for the city, so much that the\nSpanish dictator Primo de Rivera censored any\ndetail regarding its cost. To pay all the debt that it\nhad incurred, the State emitted bonds, but this\nstrategy failed to raise the necessary capital. The\ndebt was so high that a new special city tax had to\nbe implemented for paying 14 million pesetas\nevery year for the subsequent 34 years. Scandals\nover the misappropriation of funds continued to\nplague la Fira de Barcelona. In the 1980s, when the\nmunicipal servants began preparing the 1992\nOlympic Games, they discovered that the City\nCouncil was still paying the debt of the 1929\nInternational Exhibition.",
		glm::vec2(310, 900),
		ofColor(255),
		10,
		250
	);
	companiesCenterText = TextTyping(
		"THE NEW NEIGHBORHOOD AND CULTURE\n\nDespite taking place in municipal land, the\nmega-events organized by Fira de Barcelona at\nthe Montjuïc fairground use vast public resources\nto support the propaganda of global capitalism\nand its corporate landscape. Their constant \nmarketing, their conferences and macro concerts\nare part of the contemporary spectacle that treats\ncitizens as a mass of consumers, indifferent to\nany future challenge to its well-being, such as the\nclimate emergency, the disruption of artificial\nintelligence, the rise of neo-fascism, poverty, the\nwealth gap, labor exploitation or patriarchy. For \ndecades, Fira de Barcelona has taken advantage\nof public assets to enrich the private corporations\n, their shareholders, and the perpetuation of the\nculture of mass consumption that they represent.",
		glm::vec2(fixedWidth/2.0, 800),
		ofColor(255),
		10,
		250
	);
	companiesRightText = TextTyping(
		"AN OPAQUE NETWORK OF PUBLIC-PRIVATE\nENTITIES RULES KEY URBAN COMMONS\n\nThe neoliberal restructuring of the European Union\n project had dramatic consequences on the\n socio-spatial configurations of its cities. In Spain,\n led by a series of privatization friendly govern\nments, a significant effort was made to make \nurban regions attractive to foreign direct invest\nment. Barcelona, as most other European cities,\ncentered its objectives on becoming competitive,\nprompting it to offer to global private investors an\narray of highly favorable fiscal, infrastructural, and\npublic incentives to attract their capital. For the\nmanagement of this new investment, a powerful\nand opaque network of public-private corpora\ntions was weaved. Apart from the port, the airport,\nFira de Barcelona and the Chamber of Com\nmerce, the Consortium of the Free-trade Zone, the\nTourism Consortium of Barcelona and Mercabarna\nare remarkable knots in this network. The role of\nthese public-private partnerships has been to\nplunder municipal land, taxes, infrastructure,\npeople, and goods from the public and give full\ncontrol of them to the global capitalist elites.",
		glm::vec2(1510, 900),
		ofColor(255),
		10,
		250
	);
	centerLeftText = TextTyping(
		"THE NEW NEIGHBORHOOD AND DEMOCRACY\n\nTo extend the concession of the Montjuïc\nfairground to Fira de Barcelona and to allocate 380\nmillion euros of public money to its renovation\nproject “Fira Univers” is a despotic, opaque,\nclientelistic, short-sighted and anti-democratic \nimposition. It is an example of the extent to which\nglobal capitalism infringes upon public infrastruc\nture and public resources to thrive. The decision of\nthe City Council was announced unilaterally and \nwithout making the conditions of the agreement\npublic; it is unknown how many years would the\nconcession be extended. It leaves the future of the\nlarge piece of centrally located municipal land in\nthe hands of very influential lobbies, with private \ninterests and an opaque management structure.\nThe struggle of #LaFiraOLaVida is not only to\nprotest against the blatant abuse of power but to\nco-design and co-develop an alternative and\nviable proposal for achieving the right to the city\nfor all. There is still time! Barcelona's civil society\nhas until 2025 to organize an open, transparent,\nrigorous, and genuinely participatory debate. The\nUrban Front will be there to support its organiza-\ntion.",
		glm::vec2(225, 670),
		ofColor(255),
		10,
		250
	);
	buildingLeftText = TextTyping(
		"REAL-ESTATE SPECULATION, GENTRIFICATION\nAND HOUSING CRISIS IN BARCELONA\n\nFor decades, Spanish housing policies have\nentirely centered on the promotion of private-own-\nership, fueled by mortgages, financialization,\nrent-deregulation, fiscal incentives for speculators,\nand all the characteristic traits of the global real\nestate markets. These neoliberal policies were at\nthe heart of the 2008 burst of the housing bubble,\nwhich provoked 500,000 evictions while the\nSpanish government rescued the banks with more\nthan 60,000 million euros. In Barcelona, the hous-\ning condition has worsened, and it is at a critical\npoint. This is due in part to the city's profile as a\nsignificant tourist attraction as well as for congress\ngoers and organizers. In 2015, thanks to the\nelectoral victory of progressive mayor Ada Colau,\nthe city approved the construction of 18,000 new\naffordable housing units for the next ten years and\npassed legislation to force private developers to\ndestine 30% of newly built units as affordable. Still,\nthese policies have not been sufficient to curve the\nincreasing gentrification and displacements.\nToday, more than 50% pay over 40% of their\nincome to housing, while 30,000 families are\nwaiting for social housing and 80% of youngsters\nare now incapable of leaving their parent's homes.",
		glm::vec2(690, 650),
		ofColor(255),
		10,
		250
	);
	buildingRightText = TextTyping(
		"THE BARCELONA AIRPORT\n\nWith more than 50 million passengers in 2018, the\n Barcelona airport is the sixth most transited in the\n European Union. It is situated at 10 minutes by car\n from the Montjuïc fairground. The airport is\n managed by a publicly-traded state enterprise\n known as AENA, a for-profit property of the\n Spanish state. The President of Fira de Barcelona,\n Pau Relat, has been at work, lobbying for a\n multi-billion euro project to expand Barcelona's\n airport. The projec argues for the urgent construc-\ntion of a 4th landin strip to be built on the sea.",
		glm::vec2(1180, 700),
		ofColor(255),
		10,
		250
	);
	planeText = TextTyping(
		"CLIMATE CRISIS AND ENVIRONMENTAL\nPOLLUTION IN BARCELONA\n\nBarcelona is extremely vulnerable to the environ-\nmental crisis. At the global warming level, it is a\ncoastal city that is susceptive to water shortages\nand cyclical heatwaves. The predicted sea-level\nrise is expected to swallow beach fronts and flood\nentire neighborhoods, like Barceloneta. The\nincrease in temperatures will favor the introduction\nof tropical illnesses and the intensification of heavy\nstorms as well as long term drought. On the local\nscale, Barcelona is one of the most polluted cities\nin Europe. Its low air quality is responsible for\n3,500 premature deaths per year in its metropolitan\narea. Barcelona is also the European city most\npolluted by cruise ship fuel and the second throw-\ning more plastic waste the Mediterranean Sea. If\nthis was not enough, it also has some of the\ndensest motor traffic in Europe, at a ratio of 7,000\nvehicles per square kilometer —London has 1,200\nand Paris 1,500—. Over the years, Fira de Barcelo-\nna has hosted expositions for major polluting\nindustries. Its 2 million annual visitors generate\nmassive toxic emissions, water pollution, plastic\nwaste, and energy consumption.",
		glm::vec2(250, 530),
		ofColor(255),
		10,
		250
	);
	shipText = TextTyping(
		"THE PORT OF BARCELONA\n\nThe port of Barcelona is the most important\ncommodity trade and cruise ship facility of the\nMediterranean. It is managed by one of the most\ninfluential organizations in the region, another\nopaque public-private entity called Port de Barce-\nlona, with links to La Fira de Barcelona. It is rarely\nmentioned in the media, but has not been exempt\nfrom criminal proves related to illegal selling of\npublic space, as well as for corruption, illicit drug\ntrade and its relationship to Russian, Chinese and\nQatar mafias. It occupies a piece of public land\nmore extensive than the whole of Cerdá's Eixam-\nple district, fenced, secured by its port police,\nguarded by cameras and security infrastructure. It\nis a hidden site behind the Montjuïc hill where the\nvast majority of Barcelona's citizens have never\nset afoot.",
		glm::vec2(1490, 960),
		ofColor(255),
		10,
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
	handMarkers[6] = HandMarker(glm::vec2(1650, 610), red, true);
	// THE AIRPLANE
	handMarkers[7] = HandMarker(glm::vec2(165, 375), red, true);
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
				// and once that reaches 2 we can run the animation
				if (touchThreshold[i] >= 2) {
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
		pressLines[0].update(500);
		pressLines[0].draw();
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
				pressLines[1].update(animationCounter[0]);
				pressLines[1].draw();
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
				pressLines[3].update(animationCounter[1]);
				pressLines[3].draw();
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
				pressLines[4].update(animationCounter[2]);
				pressLines[4].draw();
			}
			// Building Right
			buildingRightAnimation.update(animationCounter[2]);
			buildingRightAnimation.draw();
			if (!shouldRunAnimation[5]) {
				handMarkers[5].draw();
				pressLines[5].update(animationCounter[2]);
				pressLines[5].draw();
			}
			ofPopStyle();
			break;
		// Screen center left button pressed,
		// center left button text shows up
		case 3:
			ofPushStyle();
			// Center left text
			if (!shouldRunAnimation[2]) {
				centerLeftText.update(animationCounter[3]);
				centerLeftText.draw();
				pressLines[2].update(animationCounter[3]);
				pressLines[2].draw();
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
				pressLines[6].update(animationCounter[5]);
				pressLines[6].draw();
			}
			// Airplane
			planeMovement.update(animationCounter[5]);
			planeMovement.draw();
			if (!shouldRunAnimation[6]) {
				handMarkers[7].draw();
				pressLines[7].update(animationCounter[5]);
				pressLines[7].draw();
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
