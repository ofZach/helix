#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	depthOfField.setup(1280,720);
	
	for(int i = 0; i < 100; i++){
		boxPositions.push_back(ofVec3f(ofRandom(-300, 300),
									   ofRandom(-300, 300),
									   ofRandom(-300, 300)));
		boxSizes.push_back(ofRandom(30, 100));
		boxColors.push_back(ofColor::fromHsb(ofRandomuf() > .8 ? ofRandom(120, 130) : 128 - ofRandom(110, 128), //complementing random hues
											 ofRandom(255),
											 ofRandom(255)));
	}
    
    
    h.generate();
}

//--------------------------------------------------------------
void testApp::update(){
	
	//swim the depth of field
    
    
    depthOfField.setFocalRange(mouseY);
    
    depthOfField.setFocalDistance(mouseX);
}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofEnableAlphaBlending();
    
	depthOfField.begin();
	
    ofSetLineWidth(3);
    
	camera.begin(depthOfField.getDimensions());
	
    
    ofPushMatrix();
    
    ofRotateZ(ofGetElapsedTimef()*10);
    ofSetColor(255,255,255);
    h.drawCentered(true, true);
    
    ofPopMatrix();
    
//	ofPushStyle();
//	for(int i = 0; i < boxPositions.size(); i ++){
//		ofSetColor(boxColors[i]);
//		ofBox(boxPositions[i], boxSizes[i]);
//	}
//	ofPopStyle();
	
	camera.end();
	
	depthOfField.end();

	if(ofGetKeyPressed('f')){
		depthOfField.drawFocusAssist(0, 0);
	}
	else{
		depthOfField.getFbo().draw(0, 0);
	}
	
	ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate()), 10, 10);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}