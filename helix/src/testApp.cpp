#include "testApp.h"


helix h;

//--------------------------------------------------------------
void testApp::setup(){

    h.generate();
    ofSetColor(255);
    
    blur.setup(1024, 768);
    blur2.setup(1024, 768, 8, 0.2, 1, 0.5 );
    //int radius = 32, float shape = .2, int passes = 1, float downsample = .5);
    
    dofShader.load("shaders/DOFLine");
    
    dust.loadImage("dust.png");
    
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofBackground(0,0,0);
    ofSetLineWidth(2);
    ofEnableAlphaBlending();
    ofSetColor(255);
    
    
       
    //---------------------------------------------------------  first pass blurry
    blur.begin();
    
    ofEnableLighting();

    
    ofClear(26, 37, 50, 255);
    cam.begin();
    ofDrawAxis(100);
    ofSetColor(84,204,254);
    h.drawCentered();
    cam.end();
    blur.end();
    ofSetColor(255,255,255, 200);
    
    ofDisableLighting();

    
    blur.getTextureReference().draw(0,768,1024,-768);
    
    
    //---------------------------------------------------------  second pass just helix edges
    blur.begin();
    
    
    ofClear(26, 37, 113, 255);
    cam.begin();
    ofSetLineWidth(3);
    ofEnableAlphaBlending();
    ofSetColor(84,204,254);
    h.drawCentered(false);
    cam.end();
    blur.end();
    ofSetColor(255,255,255);
    
    ofEnableAlphaBlending();
    glBlendFunc(GL_SRC_COLOR, GL_ONE);
    ofSetColor(255,255,255,255);
    blur.getTextureReference().draw(0,768,1024,-768);
    //---------------------------------------------------------  
    
   
    ofEnableAlphaBlending();

    
    
    ofEnableAlphaBlending();
    
    cam.begin();
    
    ofSetColor(255,255,255);
    dofShader.begin();
	dofShader.setUniform1f("aspectRatio", ofGetWidth() / ofGetHeight());
	dofShader.setUniform1f("lineWidth", 1);
	dofShader.setUniform1f("focusDistance", mouseX * 2);
	dofShader.setUniform1f("aperture", .03);
    dofShader.setUniform1f("bump", 0.2);
    h.drawCenteredForDOF(dofShader, true, false);
    
    
    ofSetColor(255,255,255);

    h.drawCenteredForDOF(dofShader, true, false);
    
    dofShader.setUniform1f("bump", 0.8);
    h.drawCenteredForDOF(dofShader, false, true);
    

    
    dofShader.end();
    
    
    
    
    
    ofSetLineWidth(1);
    //h.drawDebug();
    
    
    cam.end();
    
    ofSetColor(255,255,255);
    
    ofGetWindowWidth();

    
    blur.begin();
    ofClear(26, 37, 50, 255);

    dofShader.begin();
	dofShader.setUniform1f("aspectRatio", ofGetWidth() / ofGetHeight());
	dofShader.setUniform1f("lineWidth", 1);
	dofShader.setUniform1f("focusDistance", mouseX * 2);
	dofShader.setUniform1f("aperture", .03);
    dofShader.setUniform1f("bump", 0.8);
    
    ofEnableLighting();
    light.setSpotlight();
    light.setSpotConcentration(2.0);
    light.setPosition( 0, mouseX, 0);
    light.setAmbientColor(ofColor(255,255,255));
    light.setDiffuseColor(ofColor(255,255,255));
    light.setSpecularColor(ofColor(255,255,255));
    light.setDiffuseColor(ofColor(255,255,255));
    light.lookAt(ofPoint(mouseX, mouseY, 0));
                 
    light.enable();
    
    
    ofSetColor(255,255,255);
    ofSetLineWidth(10);
    cam.begin();
    h.drawCenteredNormals(true);
    cam.end();
    
    ofDisableLighting();
    
    
    dofShader.end();
    
    blur.end();
    
    ofEnableAlphaBlending();
    glBlendFunc(GL_SRC_COLOR, GL_ONE);
    ofSetColor(255,255,255,255);
    blur.getTextureReference().draw(0,768,1024,-768);
    
//    ofSetColor(150,150,150);
//    dust.draw(mouseX, mouseY, dust.getWidth()*2, dust.getHeight()*2);
    
    //blur.end();
    

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

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