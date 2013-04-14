#include "testApp.h"


helix h;

//--------------------------------------------------------------
void testApp::setup() {
    
    // allocation of fbos in windowResized function //
    
    ofBackground(10);
    
    lightRadius = 280.f;
    
    h.generate();
    ofSetColor(255);

    gui = new ofxUICanvas(0, 0, 200, ofGetHeight());
    gui->setColorBack(ofColor(255,255,255,100));
    gui->addLabel("Lights");
    gui->addWidgetDown( new ofxUIToggle( "Render FBOs", false, 16, 16) );
    gui->addWidgetDown( new ofxUIToggle( "Debug Lights", false, 16, 16) );
    gui->addSlider("Light Radius", 10, 2000, &lightRadius);
    
    gui->loadSettings("settings.xml");
    
    lightShader.load("shaders/lightLine.vert", "shaders/lightLine.frag");
    
    //int width, int height, int radius = 32, float shape = .2, int passes = 1, float downsample = .5
    blur.setup( ofGetWidth(), ofGetHeight(), 4, .3, 3, .5 );
    
    dust.loadImage("glow.png");
    bgImage.loadImage("city0.png");
    
    ofSetSphereResolution( 12 );
    
}

//--------------------------------------------------------------
void testApp::update() {
    ofMatrix4x4 camModelViewMatrix = cam.getModelViewMatrix();
    // set the position of the lights //
    ofVec3f lightPos = cam.getPosition() + (cam.getLookAtDir() * 50.f);
    for(int i = 0; i < NUM_LIGHTS; i++) {
        if(i == 0) {
            lightPos.set( -200, 0, sin(ofGetElapsedTimef()*.5) * 750.f );
        } else if( i == 1) {
            lightPos.set(200, 0, cos(ofGetElapsedTimef() * .7) * 800.f);
        } else if (i == 2) {
            lightPos.set( cos(ofGetElapsedTimef()*1.5f)*100.f, sin(ofGetElapsedTimef()*1.5f)*200.f, -sin(ofGetElapsedTimef()*.2f* 600.f));
        } else if ( i == 3) {
            lightPos.set( -cos(ofGetElapsedTimef()*1.75f)*300.f, sin(ofGetElapsedTimef()*1.25f)*200.f, sin(ofGetElapsedTimef()*.15f* 700.f));
        } else if ( i == 4) {
            lightPos.set( sin(ofGetElapsedTimef()*.75f)*100.f, -cos(ofGetElapsedTimef()*.55f)*100.f, sin(ofGetElapsedTimef()*.15f* 800.f));
        } else if ( i == 5) {
            lightPos.set( sin(ofGetElapsedTimef()*.75f)*100.f, cos(ofGetElapsedTimef()*1.55f)*300.f, sin(ofGetElapsedTimef()*.45f* 900.f));
        }
        lights[i].setPosition( lightPos );
        
        lightRadiuss[i] = lightRadius * (cos(ofGetElapsedTimef() * (((float)i/4.f)+.3f)) *.5f + .7f);
        
        
        ofVec3f lightPosInViewSpace = lights[i].getPosition() * camModelViewMatrix;
        lightPoss[i] = ( lightPosInViewSpace );
    }
    
    //swim the depth of field
//	depthOfField.setFocalDistance(ofMap(sin(ofGetElapsedTimef()/2),-1,1, 20, 250));
    
    depthOfField.setFocalDistance( ofMap((float)ofGetMouseX() / (float)ofGetWidth(), 0, 1, 100, 300.f, true) );
    depthOfField.setFocalRange( ofMap( sin(ofGetElapsedTimef() * .25), -1, 1.f, 50, 500, true ) );
}

//--------------------------------------------------------------
void testApp::draw() {
    
//    ofBackground(10);
    ofSetColor(255, 255, 255, 255);
    bgImage.draw( 0, 0);
    
    glEnable(GL_DEPTH_TEST);
    ofEnableAlphaBlending();
    
    // store the white highlights created from the lights in the fbo
    // later it will be blurred and then applied over the render
    lightFbo.begin();
    ofClear(255, 255, 255, 0);
//    ofClear(0, 0, 0, 0);
    
    cam.begin();
    
    ofSetColor(255, 255, 255);
    ofEnableLighting();

//    glDepthMask(false);
    
    ofSetLineWidth(4.f);
    
    
    lightShader.begin();
    lightShader.setUniform1f( "lightRadius", lightRadius );
    lightShader.setUniform1i( "numLights", NUM_LIGHTS );
    
    ofVec3f helixMidPt = h.getMidPoint();
    lightShader.setUniform3fv("lightPoss", (float*)lightPoss, NUM_LIGHTS );
    lightShader.setUniform1fv("lightRadiuss", (float*)lightRadiuss, NUM_LIGHTS );
    
    glDepthMask(false);
//    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetColor(255);
    h.drawCentered(true, false);
    h.drawCentered( false, true );
    glDepthMask(true);
//    ofDisableBlendMode();
    
    
    ofDisableLighting();
    lightShader.end();
    
//    glDepthMask(true);
    
    
    // calculate distance and draw a particle image into the glow fbo //
    glDepthMask(false);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    dust.getTextureReference().bind();
    ofSetColor(255, 255, 255, 255);
    
    float distance = 0;
    for(int i = 0; i < h.lines.size(); i++ ) {
        float closestDistance = 10000.f;
        float closestDistanceb = 10000.f;
        ofVec3f pos     = h.lines[i].a - helixMidPt;
        ofVec3f posb    = h.lines[i].b - helixMidPt;
        for(int j = 0; j < NUM_LIGHTS; j++ ) {
            ofVec3f lightPos = lights[j].getPosition();
            distance = (lightPos - pos).length();
            if(distance < closestDistance) closestDistance = distance;
            
            distance = (lightPos - posb).length();
            if(distance < closestDistanceb) closestDistanceb = distance;
        }
        
        if(closestDistance < lightRadius) {
            float pct = 1.f - (closestDistance / lightRadius);
            if(pct >= .2f) {
                pct = ofMap(pct, 0.2f, 1.f, 0.f, 1.f, true);
                renderGlowParticle( pos, pct );
            }
        }
        
        if(closestDistanceb < lightRadius) {
            float pct = 1.f - (closestDistanceb / lightRadius);
            if(pct >= .2f) {
                pct = ofMap(pct, 0.2f, 1.f, 0.f, 1.f, true);
                renderGlowParticle( posb, pct );
            }
        }
    }
    dust.getTextureReference().unbind();
    glDepthMask(true);
    ofDisableBlendMode();
    
    cam.end();
    
    lightFbo.end();
    
    glDisable( GL_DEPTH_TEST );
    
    ofSetColor(255);
    ofEnableAlphaBlending();
    blur.begin();
//    ofClear(255, 255, 255, 0);
    ofClear( 0, 0, 0, 255);
    lightFbo.draw(0, 0, lightFbo.getWidth(), lightFbo.getHeight() );
    lightFbo.draw(0, 0, lightFbo.getWidth(), lightFbo.getHeight() );
    blur.end();
    
    
//    ofSetColor(84,204,254);
//    h.drawCentered( true, false);
//    h.drawCentered( false, true );
    
    depthOfField.begin(); 
    cam.begin( depthOfField.getDimensions() );
    ofSetColor(84,204,254);
    h.drawCentered( true, false);
    h.drawCentered( false, true );
    cam.end();
    depthOfField.end();
    
//    ofSetColor(255, 255, 255);
//    glDepthMask(false);
//    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnableAlphaBlending();
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetColor(255, 255, 255, 255);
    depthOfField.getFbo().draw(0, 0);
    ofDisableBlendMode();
    
    
    
    ofSetLineWidth(2.f);
    cam.begin();
    bool bDebugLights = ((ofxUIToggle*)gui->getWidget("Debug Lights"))->getValue();
    if(bDebugLights) {
        ofSetColor(255, 255, 255);
        ofNoFill();
        for(int i = 0; i < NUM_LIGHTS; i++ ) {
            ofPushMatrix(); {
                ofDrawArrow( ofPoint(0,0,0), ofPoint(1,0,0) );
                ofTranslate( lights[i].getPosition() );
                
                ofMatrix4x4 mat;
                mat.makeRotationMatrix(ofVec3f(0,0,1), cam.getLookAtDir() );
                ofPushMatrix();
                ofMultMatrix(mat.getPtr());
                ofCircle(0, 0, lightRadiuss[i] );
                ofPopMatrix();
                
                
            } ofPopMatrix();
        }
        ofFill();
    }
    cam.end();
    
    
    
    
    
    ofEnableBlendMode( OF_BLENDMODE_ADD );
//    glBlendFunc(GL_SRC_COLOR, GL_ONE);
    ofSetColor(84,204,254);
//    blur.getTextureReference().draw(0,768,1024,-768);
    blur.getTextureReference().draw(0,0);
    ofSetColor(255, 255, 255, 255);
    blur.getTextureReference().draw(0,0);
//    blur.getTextureReference().draw(0,768,1024,-768);
////    blur.getTextureReference().draw(0,768,1024,-768);
    ofSetColor(255, 255, 255, 40);
//    lightFbo.draw(0,768,1024,-768 );
    ofDisableBlendMode();
    
//    glDepthMask(true);
    
    
    bool bRenderFbos = ((ofxUIToggle*)gui->getWidget("Render FBOs"))->getValue();
    if(bRenderFbos) {
        ofSetColor(0);
        ofRect(220, 20, ofGetWidth() * .25f, ofGetHeight() * .25f );
        ofSetColor(255,255,255,255);
        lightFbo.draw(220, 20 + ofGetHeight() * .25, ofGetWidth() * .25, -ofGetHeight() * .25);
    }
    

}

//--------------------------------------------------------------
void testApp::renderGlowParticle( ofVec3f aPos, float pct ) {
    ofVec3f camUp = cam.getUpDir();
    ofVec3f camRight = cam.getSideDir();
    float dustScale = .15f;
    ofVec3f p1, p2, p3, p4;
    
    float sw = dust.getWidth() * dustScale;
    float sh = dust.getHeight() * dustScale;
    p1 = aPos + -camUp*sh + -camRight*sw;
    p2 = aPos + -camUp*sh + camRight*sw;
    p3 = aPos + camUp*sh + camRight*sw;
    p4 = aPos + camUp*sh + -camRight*sw;
    
    ofSetColor(255, 255, 255, 255 * pct );
    
    glBegin(GL_TRIANGLE_STRIP );
    glTexCoord2f(0., 0.);
    glVertex3f(p1.x, p1.y, p1.z);
    glTexCoord2f( dust.getWidth(), 0.);
    glVertex3f(p2.x, p2.y, p2.z);
    glTexCoord2f(0., dust.getHeight() );
    glVertex3f(p4.x, p4.y, p4.z);
    glTexCoord2f(dust.getWidth(), dust.getHeight());
    glVertex3f(p3.x, p3.y, p3.z);
    glEnd();
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
    depthOfField.setup( w, h );
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}