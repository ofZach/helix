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
    
    void generate();
    void drawCenteredForDOF( ofShader & dofShader, bool bDrawInner = true, bool bDrawOuter = true  );
    void drawCentered(  bool bDrawInner = true );
    void drawCenteredNormals(  bool bDrawInner = true );
    
    void drawDebug();
};
