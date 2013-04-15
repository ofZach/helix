#include "testApp.h"


helix h;

//--------------------------------------------------------------
void testApp::setup() {
    
    lightFbo.allocate( ofGetWidth(), ofGetHeight(), GL_RGBA );
    depthOfField.setup( ofGetWidth(), ofGetHeight() );
    
    depthOfField.setBlurAmount( 1.5f );
    dofFocalRange = 60.f;
    
    ofBackground(10);
    
    lightRadius = 280.f;
    
    h.generate();
    ofSetColor(255);

    gui = new ofxUICanvas(0, 0, 200, ofGetHeight());
    gui->setColorBack(ofColor(255,255,255,100));
    gui->addLabel("Lights");
    gui->addWidgetDown( new ofxUIToggle( "Render FBOs", false, 16, 16) );
    gui->addWidgetDown( new ofxUIToggle( "Debug Lights", false, 16, 16) );
    gui->addWidgetDown( new ofxUIToggle( "Draw Normals", false, 16, 16) );
    gui->addSlider("Light Radius", 10, 2000, &lightRadius);
    gui->addSlider("DoF Focal Range", 10, 1000, &dofFocalRange );
    
    gui->loadSettings("settings.xml");
    
    lightShader.load("shaders/lightLine.vert", "shaders/lightLine.frag");
    
    //int width, int height, int radius = 32, float shape = .2, int passes = 1, float downsample = .5
    blur.setup( ofGetWidth(), ofGetHeight(), 4, .3, 3, .5 );
    blurDof.setup( ofGetWidth(), ofGetHeight(), 4, .3, 1, .8 );
    
    phongShader.load("shaders/phongShader.vert", "shaders/phongShader.frag");
    
    dust.loadImage("glow.png");
    bgImage.loadImage("city0.png");
    
    ofSetSphereResolution( 12 );
    
    
    light.setPointLight();
    light.setAmbientColor(ofColor(0, 0, 0));
    light.setDiffuseColor(ofColor(255, 255, 255));
    light.setSpecularColor(ofColor(255,255,255));
    
    material.setAmbientColor( ofColor(0,0,0) );
    material.setDiffuseColor( ofColor(255, 255, 255 ) );
    material.setSpecularColor( ofColor(255, 255, 255) );
    material.setShininess(4);
    
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
//        lights[i].setPosition( lightPos );
        lightPosViewSpace[i] = lightPos;
        
        lightRadiuss[i] = lightRadius * (cos(ofGetElapsedTimef() * (((float)i/4.f)+.3f)) *.5f + 1.f);
        
        
        ofVec3f lightPosInViewSpace = lightPos * camModelViewMatrix;
        lightPoss[i] = ( lightPosInViewSpace );
    }
    
    //swim the depth of field
//	depthOfField.setFocalDistance(ofMap(sin(ofGetElapsedTimef()/2),-1,1, 20, 250));
    
    depthOfField.setFocalDistance( ofMap((float)ofGetMouseX() / (float)ofGetWidth(), 0, 1, 100, 300.f, true) );
    depthOfField.setFocalRange( dofFocalRange );
//    depthOfField.setFocalRange( ofMap((float)ofGetMouseY() / (float)ofGetHeight(), 0, 1, 10, 300, true ) );
//    depthOfField.setFocalDistance( ofMap( cos(ofGetElapsedTimef()), -1, 1, 100, 300.f, true) );
//    depthOfField.setFocalRange( ofMap( sin(ofGetElapsedTimef() * .75), -1, 1.f, 20, 150, true ) );
}

//--------------------------------------------------------------
void testApp::draw() {
    
//    ofBackground(50);
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
    
    ofSetLineWidth(4.f);
    
    
    lightShader.begin();
    lightShader.setUniform1f( "lightRadius", lightRadius );
    lightShader.setUniform1i( "numLights", NUM_LIGHTS );
    
    ofVec3f helixMidPt = h.getMidPoint();
    lightShader.setUniform3fv("lightPoss", (float*)lightPoss, NUM_LIGHTS );
    lightShader.setUniform1fv("lightRadiuss", (float*)lightRadiuss, NUM_LIGHTS );
    
    glDepthMask(false);
    ofSetColor(255);
    h.drawCentered(true, false);
    h.drawCentered( false, true );
    glDepthMask(true);
    
    
    ofDisableLighting();
    lightShader.end();
    
    
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
            ofVec3f lightPos = lightPoss[j];
            distance = (lightPos - pos).length();
            if(distance < closestDistance) closestDistance = distance;
            
            distance = (lightPos - posb).length();
            if(distance < closestDistanceb) closestDistanceb = distance;
        }
        
        if(closestDistance < lightRadius) {
            float pct = 1.f - (closestDistance / lightRadius);
            if(pct >= .3f) {
                pct = ofMap(pct, 0.3f, 1.f, 0.f, .75f, true);
                renderGlowParticle( pos, pct );
            }
        }
        
        if(closestDistanceb < lightRadius) {
            float pct = 1.f - (closestDistanceb / lightRadius);
            if(pct >= .3f) {
                pct = ofMap(pct, 0.3f, 1.f, 0.f, .75f, true);
                renderGlowParticle( posb, pct );
            }
        }
    }
    dust.getTextureReference().unbind();
    glDepthMask(true);
    ofDisableBlendMode();
    
    
    
    
    // render phong shading around the outter strip //
    ofEnableBlendMode( OF_BLENDMODE_ADD );
    ofEnableLighting();
    float lightx = ofMap(ofGetMouseX(), 0, ofGetWidth(), -700, 700, true);
    light.setPosition( cos(ofGetElapsedTimef()*1.5) * 700.f, sin(ofGetElapsedTimef()*1.5) * 700.f, 0 );
    
    light.enable();
    material.begin();
    phongShader.begin();
    h.drawCentered( true, false );
    phongShader.end();
    material.end();
    
    light.disable();
    ofDisableLighting();
    ofDisableBlendMode();
    
    
    cam.end();
    
    lightFbo.end();
    
    glDisable( GL_DEPTH_TEST );
    
    
    ofSetColor(255);
    ofEnableAlphaBlending();
    blur.begin();
//    ofClear(255, 255, 255, 0);
    ofClear( 0, 0, 0, 255);
    lightFbo.draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight() ); // 0,768,1024,-768
    lightFbo.draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight() );
    blur.end();
    
    
    
    // add a slight blur to everything, to reduce hard edges 
    blurDof.begin();
    ofClear(0, 0, 0, 255);
    glDepthMask(false);
//    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetColor(84,204,254);
    cam.begin();
    h.drawCentered(true, false);
    h.drawCentered( false, true );
    cam.end();
    glDepthMask(true);
    blurDof.end();
    
    
    
    
    
    ofEnableAlphaBlending();
    depthOfField.begin();
//    cam.begin( depthOfField.getDimensions() );
    cam.begin();
    ofSetColor(84,204,254);
    h.drawCentered( true, false);
    h.drawCentered( false, true );
    cam.end();
    depthOfField.end();
    
    ofDisableBlendMode();
    
    // render all of the lines with a slight blur
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetColor(255, 255, 255, 45);
    blurDof.getTextureReference().draw( 0, ofGetHeight(), ofGetWidth(), -ofGetHeight() );
    // render the depth of field buffer
    ofSetColor(255, 255, 255, 105);
    depthOfField.getFbo().draw( 0, ofGetHeight(), ofGetWidth(), -ofGetHeight() );
    
    ofDisableBlendMode();
    
    // render the blur texture once with blur color and then once with white to make it stronger
    ofEnableBlendMode( OF_BLENDMODE_ADD );
    ofSetColor(84,204,254);
    blur.getTextureReference().draw(0,0);
    ofSetColor(255, 255, 255, 25);
    blur.getTextureReference().draw(0,0);
//    ofSetColor(255, 255, 255, 40);
//    lightFbo.draw(0,768,1024,-768 );
    ofDisableBlendMode();
    
    
    
    bool bDrawNormals = ((ofxUIToggle*)gui->getWidget("Draw Normals"))->getValue();
    if(bDrawNormals) {
        cam.begin();
        ofSetColor(255, 0, 0);
        h.drawCenteredNormals();
        cam.end();
    }
    
    
    // render out the light locations //
    ofSetLineWidth(2.f);
    bool bDebugLights = ((ofxUIToggle*)gui->getWidget("Debug Lights"))->getValue();
    if(bDebugLights) {
        cam.begin();
        ofSetColor(255, 255, 255);
        ofNoFill();
        for(int i = 0; i < NUM_LIGHTS; i++ ) {
            ofPushMatrix(); {
                ofDrawArrow( ofPoint(0,0,0), ofPoint(1,0,0) );
                ofTranslate( lightPosViewSpace[i] );
                
                ofMatrix4x4 mat;
                mat.makeRotationMatrix(ofVec3f(0,0,1), cam.getLookAtDir() );
                ofPushMatrix();
                ofMultMatrix(mat.getPtr());
                ofCircle(0, 0, lightRadiuss[i] );
                ofPopMatrix();
                
                
            } ofPopMatrix();
        }
        ofFill();
        
        ofSetColor(255, 255, 0);
        light.draw();
        
        cam.end();
    }
    
    
    bool bRenderFbos = ((ofxUIToggle*)gui->getWidget("Render FBOs"))->getValue();
    if(bRenderFbos) {
        float fboW = ofGetWidth() * .35f;
        float fboH = ofGetHeight() * .35f;
        
        ofEnableAlphaBlending();
        ofSetColor(0);
        ofRect(220, 20, fboW, fboH );
        ofSetColor(255,255,255,255);
        ofDrawBitmapString( "Light Mask FBO", 220, 16);
        lightFbo.draw(220, 20 + fboH, fboW, -fboH );
        
        ofSetColor( 0, 0, 0, 255);
        ofRect(240 + fboW, 20, fboW, fboH );
        ofSetColor(255, 255, 255, 255);
        ofDrawBitmapString( "Blur FBO", 240+fboW, 16);
        blur.getTextureReference().draw( 240 + fboW, 20, fboW, fboH );
        
        
        ofSetColor(0, 0, 0, 255);
        ofRect( 220, 40 + fboH, fboW, fboH);
        ofSetColor(255, 255, 255, 255);
        ofDrawBitmapString( "DoF FBO", 220, 36 + fboH );
        depthOfField.getFbo().draw( 220, 40 + fboH, fboW, fboH);
        
        
        ofSetColor(0, 0, 0, 255);
        ofRect( 240+fboW, 40 + fboH, fboW, fboH);
        ofSetColor(255, 255, 255, 255);
        ofDrawBitmapString( "Blur DoF FBO (added for strength)", 240+fboW, 36 + fboH );
        blurDof.getTextureReference().draw( 240+fboW, 40 + fboH, fboW, fboH);
        
        
    }
    

}

//--------------------------------------------------------------
void testApp::renderGlowParticle( ofVec3f aPos, float pct ) {
    ofVec3f camUp = cam.getUpDir();
    ofVec3f camRight = cam.getSideDir();
    float dustScale = .2f;
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
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}