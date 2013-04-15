#pragma once

#include "ofMain.h"
#include "ofxBlur.h"
#include "helix.h"
#include "ofxDOF.h"
#include "ofxUI.h"

#define NUM_LIGHTS 6

class testApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    
    void renderGlowParticle( ofVec3f aPos, float pct );

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    ofEasyCam cam;
    
    ofMaterial material;
    ofLight light;
    ofVec3f lightPosViewSpace[ NUM_LIGHTS ];
    ofVec3f lightPoss[ NUM_LIGHTS ];
    float lightRadiuss[ NUM_LIGHTS ];
    
    ofxDOF depthOfField;
    float dofFocalRange;
    
    ofShader phongShader;
    
    ofShader lightShader;
    ofFbo lightFbo;
    
    ofxUICanvas *gui;
    
    float lightRadius;
    
    ofxBlur blur;
    ofxBlur blurDof;
    
    ofImage dust;
    ofImage bgImage;
    
};
