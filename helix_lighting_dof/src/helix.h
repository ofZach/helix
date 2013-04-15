#include "ofMain.h"

#pragma once


class helix {
    
public:
    
    
    class line{
    public:
        ofPoint a;
        ofPoint b;
    };
    
    ofPolyline helix0;
    ofPolyline helix1;
    vector < line > lines;
    
    ofMesh strip1;
    ofMesh strip2;
    
    ofImage lineImage;
    
    void generate();
    
    void drawCenteredForDOF( ofShader & dofShader, bool bDrawInner = true, bool bDrawOuter = true  );
    void drawCentered(  bool bDrawOuter = true, bool bDrawInner = true );
    void drawCenteredNormals(  bool bDrawInner = true );
    
    ofVec3f getMidPoint();
    
    void drawDebug();
};
