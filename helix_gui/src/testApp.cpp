#include "testApp.h"


helix h;


// blur amount scale outside
// blur amount scale inside
// % visible outside
// % visible inside
// DOF params


typedef struct {
    
    float bos;
    float bob;
    float boa;
    
    float bis;
    float bib;
    float bia;
    
    float dof_lw;
    float dof_f;
    float dof_app;
    
    
    float dof_o_lw;
    float dof_o_f;
    float dof_o_app;
    
    
    
} params;

params p;



//--------------------------------------------------------------
void testApp::setup(){


    
    h.generate();
    ofSetColor(255);
    
    blur.setup(1024, 768, 8, 0.2, 1, 0.5);
    blur2.setup(1024, 768, 32, 0.2, 8, 0.4);
    //int radius = 32, float shape = .2, int passes = 1, float downsample = .5);
    
    filter.setup(1024, 768);
    
    
    dofShader.load("shaders/DOFLine");
    
    dust.loadImage("dust.png");
    
    

    gui = new ofxUICanvas(0, 0, 200, ofGetHeight());
    gui->setColorBack(ofColor(255,255,255,100));
	
    gui->addSlider("blurOutScale", 0.0, 10.0, &(p.bos = 2.0));
    gui->addSlider("blurOutBri", 0.0, 255.0,  &(p.bob = 2.0));
    gui->addSlider("blurOutAlpha", 0.0, 255.0, &(p.boa = 100.0));
    gui->addSlider("blurInScale", 0.0, 10.0, &(p.bis = 2.0));
    gui->addSlider("blurInBri", 0.0, 255.0,  &(p.bib = 2.0));
    gui->addSlider("blurInAlpha", 0.0, 255.0, &(p.bia = 100.0));
    gui->addSlider("DOF_lineWidth", 0.0, 200.0, &(p.dof_lw = 4));
    gui->addSlider("DOF_focus", -1000, 1000, &(p.dof_f = 200));
    gui->addSlider("DOF_aperature", 0.0, 1.0, &(p.dof_app = 0.03));
    
    gui->addSlider("DOF_outer_lineWidth", 0.0, 200.0, &(p.dof_o_lw = 4));
    
    
    gui->loadSettings("settings.xml");
    
    
    img.loadImage("city0.png");
    
    fbo.allocate(1024,768);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    
    
    ofEnableLighting();
    light.setDirectional();
    light.setSpotConcentration(2.0);
    light.setPosition( 0, mouseX, 0);
    light.setAmbientColor(ofColor(255,255,255));
    light.setDiffuseColor(ofColor(255,255,255));
    light.setSpecularColor(ofColor(255,255,255));
    light.setDiffuseColor(ofColor(255,255,255));
    light.lookAt(ofPoint(mouseX, mouseY, 0));

    light.enable();
    
    
    fbo.begin();
    
    ofBackground(0,0,0);
    
    
    
    
    
    
    
    ofSetLineWidth(2);
    ofEnableAlphaBlending();
    ofSetColor(255);
    
    
       
    //---------------------------------------------------------  first pass blurry outer
    blur2.begin();
    blur2.setScale( p.bos);
    blur2.setBrightness(p.bob);
    ofClear(0,0,0, 255);
    cam.begin();
    //ofDrawAxis(100);
    ofSetColor(255,255,255);
    h.drawCentered(true, false);
    cam.end();
    //ofSetColor(255,255,255, 200);
    blur2.end();
    ofSetColor(110, 194, 253, p.boa);
    blur2.getTextureReference().draw(0,0);
        
    
    //---------------------------------------------------------  second pass blurry inner
    blur2.begin();
    blur2.setScale( p.bis);
    blur2.setBrightness(p.bib);
    ofClear(0, 0, 0, 0);
    cam.begin();
    //ofDrawAxis(100);
    ofSetColor(255,255,255);
    h.drawCentered(false, true);
    cam.end();
    ofSetColor(255,255,255, 200);
    blur2.end();
    ofSetColor(110 * p.bia/255.0, 194 * p.bia/255.0, 253 * p.bia/255.0, p.bia);
    
    //    ofEnableAlphaBlending();
    glBlendFunc(GL_SRC_COLOR, GL_ONE);
    
    //ofEnableAlphaBlending();
    
    blur2.getTextureReference().draw(0,0);
    
    ofEnableAlphaBlending();
//
//    
//    
//    
//    
    //---------------------------------------------------------
    dofShader.begin();
	dofShader.setUniform1f("aspectRatio", ofGetWidth() / ofGetHeight());
	dofShader.setUniform1f("lineWidth", p.dof_lw);
	dofShader.setUniform1f("focusDistance", p.dof_f);
	dofShader.setUniform1f("aperture", p.dof_app);
    dofShader.setUniform1f("bump", 0.2);
    cam.begin();
    ofDrawAxis(100);
    ofSetColor(110, 194, 253, 170);
    h.drawCenteredForDOF(dofShader, true, false);
    cam.end();
    dofShader.end();
    
    
    
    dofShader.begin();
	dofShader.setUniform1f("aspectRatio", ofGetWidth() / ofGetHeight());
	dofShader.setUniform1f("lineWidth", p.dof_o_lw);
	dofShader.setUniform1f("focusDistance", p.dof_f);
	dofShader.setUniform1f("aperture", p.dof_app);
    dofShader.setUniform1f("bump", 1.8);
    cam.begin();
    ofDrawAxis(100);
    ofSetColor(110, 194, 253, 170);
    h.drawCenteredForDOF(dofShader, false, true);
    cam.end();
    dofShader.end();
    //---------------------------------------------------------
    
    
    fbo.end();
    
    
    img.draw(0,0);
    glBlendFunc(GL_SRC_COLOR, GL_ONE);
    
    fbo.draw(0, 0);
    
//    
//    //---------------------------------------------------------  second pass just helix edges
//    blur.begin();
//    
//    
//    ofClear(26, 37, 113, 255);
//    cam.begin();
//    ofSetLineWidth(3);
//    ofEnableAlphaBlending();
//    ofSetColor(84,204,254);
//    h.drawCentered(false);
//    cam.end();
//    blur.end();
//    ofSetColor(255,255,255);
//    
//    ofEnableAlphaBlending();
//    glBlendFunc(GL_SRC_COLOR, GL_ONE);
//    ofSetColor(255,255,255,255);
//    blur.getTextureReference().draw(0,768,1024,-768);
//    //---------------------------------------------------------  
//    
//   
//    ofEnableAlphaBlending();
//
//    
//    
//    ofEnableAlphaBlending();
//    
//    cam.begin();
//    
//    ofSetColor(255,255,255);
//    dofShader.begin();
//	dofShader.setUniform1f("aspectRatio", ofGetWidth() / ofGetHeight());
//	dofShader.setUniform1f("lineWidth", 1);
//	dofShader.setUniform1f("focusDistance", mouseX * 2);
//	dofShader.setUniform1f("aperture", .03);
//    dofShader.setUniform1f("bump", 0.2);
//    h.drawCenteredForDOF(dofShader, true, false);
//    
//    
//    ofSetColor(255,255,255);
//
//    h.drawCenteredForDOF(dofShader, true, false);
//    
//    dofShader.setUniform1f("bump", 0.8);
//    h.drawCenteredForDOF(dofShader, false, true);
//    
//
//    
//    dofShader.end();
//    
//    
//    
//    
//    
//    ofSetLineWidth(1);
//    //h.drawDebug();
//    
//    
//    cam.end();
//    
//    ofSetColor(255,255,255);
//    
//    ofGetWindowWidth();
//
//    
//    blur.begin();
//    ofClear(26, 37, 50, 255);
//
//    dofShader.begin();
//	dofShader.setUniform1f("aspectRatio", ofGetWidth() / ofGetHeight());
//	dofShader.setUniform1f("lineWidth", 1);
//	dofShader.setUniform1f("focusDistance", mouseX * 2);
//	dofShader.setUniform1f("aperture", .03);
//    dofShader.setUniform1f("bump", 0.8);
//    
//    ofEnableLighting();
//    light.setSpotlight();
//    light.setSpotConcentration(2.0);
//    light.setPosition( 0, mouseX, 0);
//    light.setAmbientColor(ofColor(255,255,255));
//    light.setDiffuseColor(ofColor(255,255,255));
//    light.setSpecularColor(ofColor(255,255,255));
//    light.setDiffuseColor(ofColor(255,255,255));
//    light.lookAt(ofPoint(mouseX, mouseY, 0));
//                 
//    light.enable();
//    
//    
//    ofSetColor(255,255,255);
//    ofSetLineWidth(10);
//    cam.begin();
//    h.drawCenteredNormals(true);
//    cam.end();
//    
//    ofDisableLighting();
//    
//    
//    dofShader.end();
//    
//    blur.end();
//    
//    ofEnableAlphaBlending();
//    glBlendFunc(GL_SRC_COLOR, GL_ONE);
//    ofSetColor(255,255,255,255);
//    blur.getTextureReference().draw(0,768,1024,-768);
//    
////    ofSetColor(150,150,150);
////    dust.draw(mouseX, mouseY, dust.getWidth()*2, dust.getHeight()*2);
//    
//    //blur.end();
    

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    if (key == 's'){
        gui->saveSettings("settings.xml");
    }
    
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

    if (x < 200){
        cam.disableMouseInput();
        
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

    cam.enableMouseInput();
    
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