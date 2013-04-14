#include "testApp.h"


helix h;

//--------------------------------------------------------------
void testApp::setup() {
    
    lightRadius = 280.f;
    
    h.generate();
    ofSetColor(255);

    gui = new ofxUICanvas(0, 0, 200, ofGetHeight());
    gui->setColorBack(ofColor(255,255,255,100));
    gui->addLabel("Lights");
    gui->addWidgetDown( new ofxUIToggle( "Render FBOs", false, 16, 16) );
    gui->addSlider("Light Radius", 10, 2000, &lightRadius);
    
    gui->loadSettings("settings.xml");
    
    lightShader.load("shaders/lightLine.vert", "shaders/lightLine.frag");
    
    //int width, int height, int radius = 32, float shape = .2, int passes = 1, float downsample = .5
    blur.setup( ofGetWidth(), ofGetHeight(), 16, .2, 2, .3 );
    
}

//--------------------------------------------------------------
void testApp::update() {
    
}

//--------------------------------------------------------------
void testApp::draw() {
    
    
    glEnable(GL_DEPTH_TEST);
    ofEnableAlphaBlending();
    
    // store the white highlights created from the lights in the fbo
    // later it will be blurred and then applied over the render
    lightFbo.begin();
    ofClear(0, 0, 0, 0);
//    ofSetColor(0, 0, 0);
//    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    
    cam.begin();
//    for(int i = 0; i < NUM_LIGHTS; i++ ) {
//        lights[i].draw();
//    }

//    glDepthMask(false);
    
    ofVec3f lightPos = cam.getPosition() + (cam.getLookAtDir() * 50.f);
    ofSetLineWidth(5.f);
    
    ofMatrix4x4 camModelViewMatrix = cam.getModelViewMatrix();
    lightShader.begin();
    for(int i = 0; i < NUM_LIGHTS; i++) {
        
        if(i == 0) {
            lightPos.set( -200, 0, sin(ofGetElapsedTimef()*.5) * 400.f );
        } else if( i == 1) {
            lightPos.set(200, 0, cos(ofGetElapsedTimef() * .7) * 300.f);
        }
        lights[i].setPosition( lightPos );
        
        ofVec3f lightPosInViewSpace = lights[i].getPosition() * camModelViewMatrix;
        lightShader.setUniform1f( "lightRadius", lightRadius );
        lightShader.setUniform3fv( "lightPos", &lightPosInViewSpace.getPtr()[0] );
//        ofSetColor( 50, 255, 255);
        h.drawCentered(true, false);
        h.drawCentered( false, true );
    }
    
//    glDepthMask(true);
    lightShader.end();
    
    cam.end();
    
    lightFbo.end();
    
    glDisable( GL_DEPTH_TEST );
    
    ofSetColor(255);
    ofEnableAlphaBlending();
    blur.begin();
    ofClear(0, 0, 0, 255);
    lightFbo.draw(0, 0, lightFbo.getWidth(), lightFbo.getHeight() );
    blur.end();
    
    
    ofSetLineWidth(2.f);
    cam.begin();
    ofSetColor(84,204,254);
    h.drawCentered(true, false);
    h.drawCentered( false, true );
    cam.end();
    
    ofEnableBlendMode( OF_BLENDMODE_ADD );
    blur.getTextureReference().draw(0,768,1024,-768);
//    blur.getTextureReference().draw(0,768,1024,-768);
    ofSetColor(255, 255, 255, 20);
    lightFbo.draw(0,768,1024,-768 );
    ofDisableBlendMode();
    
    
    bool bRenderFbos = ((ofxUIToggle*)gui->getWidget("Render FBOs"))->getValue();
    if(bRenderFbos) {
        ofSetColor(255,255,255,255);
        lightFbo.draw(220, 20 + ofGetHeight() * .25, ofGetWidth() * .25, -ofGetHeight() * .25);
    }
    

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
void testApp::windowResized(int w, int h) {
//    cout << "windowResized :: w = " << w << " h = " << h << endl;
    lightFbo.allocate( w, h, GL_RGBA );
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}