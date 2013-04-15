//
//  helix.cpp
//  helix
//
//  Created by zach lieberman on 3/15/13.
//
//

#include "helix.h"



    
void helix::generate(){
    
    lineImage.loadImage("line.png");
        
    
    //float radius = 200;
    
    for (int i = 0; i < 200; i++){
        
        float pct = i / 200.0;
        
        float r = 200;
        float x = r * cos(  pct * TWO_PI * 2 );
        float y = r * sin(  pct * TWO_PI * 2 );
        float z = pct * 900;
        
        ofPoint pa(x, y, z);
        
        helix0.addVertex(  ofPoint(x,y,z) );
        
        x = r * cos(  pct * TWO_PI * 2 + PI );
        y = r * sin(  pct * TWO_PI * 2 + PI);
        
        ofPoint pb(x,y,z);
        
        float tcoordx = pct * 64.f * 20.f;
        
        ofVec3f normal = pa - pb;
        normal.normalize();
        strip1.addVertex( ofPoint(pa.x, pa.y, pa.z - 6) );
        strip1.addNormal( normal );
        strip1.addTexCoord( ofVec2f(tcoordx, 0) );
        
        strip1.addVertex( ofPoint(pa.x, pa.y, pa.z + 6) );
        strip1.addNormal( normal );
        strip1.addTexCoord( ofVec2f(tcoordx, 64) );
        
        normal *= -1.f;
        strip2.addVertex( ofPoint( pb.x, pb.y, pb.z - 6) );
        strip2.addNormal( normal );
        strip2.addTexCoord( ofVec2f(tcoordx, 0) );
        
        strip2.addVertex( ofPoint( pb.x, pb.y, pb.z + 6) );
        strip2.addNormal( normal );
        strip2.addTexCoord( ofVec2f(tcoordx, 64) );
        
        if (i % 3 == 0){
            
            line l;
            l.a = pa;
            l.b = pb;
            lines.push_back(l);
        }
        
        helix1.addVertex(  ofPoint(x,y, z) );
        
    }
    
    strip1.setMode( OF_PRIMITIVE_TRIANGLE_STRIP );
    strip1.setupIndicesAuto();
    
    strip2.setMode( OF_PRIMITIVE_TRIANGLE_STRIP );
    strip2.setupIndicesAuto();
    
}

    
void helix::drawCenteredForDOF( ofShader & dofShader, bool bDrawInner, bool bDrawOuter ){
        
               
        GLint sideLocation = dofShader.getAttributeLocation("side");
        GLint nextLocation = dofShader.getAttributeLocation("next");
        
        
        
        ofPoint midPt;
        int count = 0;
        
        for (int i = 0; i < helix0.size(); i++){
            midPt += helix0[i];
            count++;
        }
        
        for (int i = 0; i < helix1.size(); i++){
            midPt += helix1[i];
            count++;
        }
        
        for (int i = 0; i < lines.size(); i++){
            midPt += lines[i].a;
            midPt += lines[i].b;
            count += 2;
        }
        
        midPt /= (float)count;
        
        
    
    
        ofPushMatrix();
        ofTranslate(-midPt.x, -midPt.y, -midPt.z);
    
    
    if (bDrawOuter){
        glBegin(GL_TRIANGLE_STRIP);
        
        for (int i = 0; i < helix0.size()-1; i++){
            ofPoint cur = helix0[i];
            ofPoint next = helix0[i+1];
            
            ofPoint temp0 = cur;
            ofPoint temp1 = next;
            temp0.normalize();
            temp1.normalize();
            ofPoint normal = temp0.cross(temp1).normalize();
            
            
            dofShader.setAttribute1f(sideLocation, -.5);
            dofShader.setAttribute3f(nextLocation, next.x, next.y, next.z);
            
            //glNormal3f(normal.x, normal.y, normal.y);
            glVertex3f(cur.x, cur.y, cur.z);
            
            dofShader.setAttribute1f(sideLocation, +.5);
            dofShader.setAttribute3f(nextLocation, next.x, next.y, next.z);
            
            //glNormal3f(normal.x, normal.y, normal.y);
            glVertex3f(cur.x, cur.y, cur.z);
            
        }
        
        
        glEnd();
        
        glBegin(GL_TRIANGLE_STRIP);
        
        for (int i = 0; i < helix1.size()-1; i++){
            ofPoint cur = helix1[i];
            ofPoint next = helix1[i+1];
            
            ofPoint temp0 = cur;
            ofPoint temp1 = next;
            temp0.normalize();
            temp1.normalize();
            ofPoint normal = temp0.cross(temp1).normalize();
            
            
            dofShader.setAttribute1f(sideLocation, -.5);
            dofShader.setAttribute3f(nextLocation, next.x, next.y, next.z);
            //glNormal3f(normal.x, normal.y, normal.y);
            
            glVertex3f(cur.x, cur.y, cur.z);
            
            dofShader.setAttribute1f(sideLocation, +.5);
            dofShader.setAttribute3f(nextLocation, next.x, next.y, next.z);
            //glNormal3f(normal.x, normal.y, normal.y);
            
            glVertex3f(cur.x, cur.y, cur.z);
            
        }
        
        glEnd();
    }
    
        if (bDrawInner)
        for (int i = 0; i < lines.size(); i++){
            
            int divisions = 20;
            
            glBegin(GL_TRIANGLE_STRIP);
            
            
            for (int j = 0; j < 20; j++){
                
                
                ofPoint cur = lines[i].a + ((lines[i].b - lines[i].a)/(float)19) * j;
                ofPoint next = lines[i].a + ((lines[i].b - lines[i].a)/(float)19) * (j+1);
                
                dofShader.setAttribute1f(sideLocation, -.5);
                dofShader.setAttribute3f(nextLocation, next.x, next.y, next.z);
                glVertex3f(cur.x, cur.y, cur.z);
                glTexCoord2d( ofRandom(0,50), ofRandom(0,50));
                
                
                
                dofShader.setAttribute1f(sideLocation, +.5);
                dofShader.setAttribute3f(nextLocation, next.x, next.y, next.z);
                glVertex3f(cur.x, cur.y, cur.z);
                glTexCoord2d(ofRandom(0,50), ofRandom(0,50));
                
                
                
                
                
            }
            
            glEnd();
            
            
        }
    
    
    
     ofPopMatrix();
        
        
}


void helix::drawCenteredNormals(  bool bDrawInner ){
    
    
    ofPoint midPt = getMidPoint();
    
    ofPushMatrix();
    ofTranslate(-midPt.x, -midPt.y, -midPt.z);
    
    
//    helix0.draw();
//    helix1.draw();
//    
//    if (bDrawInner)
//        for (int i = 0; i < lines.size(); i++){
//            ofLine(lines[i].a, lines[i].b);
//        }
//    
    
    glBegin( GL_LINES );
    for(int i = 0; i < strip1.getNumNormals(); i++ ) {
        ofVec3f normal = strip1.getNormal( i );
        normal *= 26.f;
        ofVec3f vert = strip1.getVertex( i );
        glVertex3f( vert.x, vert.y, vert.z );
        glVertex3f( vert.x+normal.x, vert.y+normal.y, vert.z+normal.z );
    }
    
    for(int i = 0; i < strip2.getNumNormals(); i++ ) {
        ofVec3f normal = strip2.getNormal( i );
        normal *= 26.f;
        ofVec3f vert = strip2.getVertex( i );
        glVertex3f( vert.x, vert.y, vert.z );
        glVertex3f( vert.x+normal.x, vert.y+normal.y, vert.z+normal.z );
    }
    glEnd();
//    glBegin(GL_LINE_STRIP);
//    
//    for (int i = 0; i < helix0.size()-1; i++){
//        
//        
//        ofVec3f temp0 = helix0[i];
//        ofVec3f temp1 = helix0[i+1];
//        temp0.normalize();
//        temp1.normalize();
//        
//        ofVec3f normal = (temp0.cross(temp1)).normalize();
//        
//        
//        glNormal3f(normal.x, normal.y, normal.z);
//        glVertex3f(helix0[i].x, helix0[i].y, helix0[i].z);
//        
//        //ofLine(helix0[i],  helix0[i] - (temp0.cross(temp1)).normalize() * 10.0);
//        
//    }
//    
//    glEnd();
    
    
    ofPopMatrix();
    
    
}



void helix::drawCentered(  bool bDrawOuter, bool bDrawInner ){
    
    ofPoint midPt = getMidPoint();

    ofPushMatrix();
    ofTranslate(-midPt.x, -midPt.y, -midPt.z);
    
    
    if (bDrawOuter) {
        lineImage.getTextureReference().bind();
        strip1.draw();
        strip2.draw();
        lineImage.getTextureReference().unbind();
//        helix0.draw();
//        helix1.draw();
    }

    if (bDrawInner) {
        for (int i = 0; i < lines.size(); i++){
            float div = 20;
            for (int j = 0; j < (int)(div-1); j++){
                ofPoint a = lines[i].a;
                ofPoint diff = (lines[i].b - lines[i].a) / (div-1.f);
                ofLine(a + diff * j, a + diff * (j+1));
            }
        }
    }
    
    ofPopMatrix();
    
}


void helix::drawDebug(){
    
    ofPoint midPt;
    int count = 0;
    
    for (int i = 0; i < helix0.size(); i++){
        midPt += helix0[i];
        count++;
    }
    
    for (int i = 0; i < helix1.size(); i++){
        midPt += helix1[i];
        count++;
    }
    
    for (int i = 0; i < lines.size(); i++){
        midPt += lines[i].a;
        midPt += lines[i].b;
        count += 2;
    }
    
    midPt /= (float)count;
    
    
    
    ofPushMatrix();
    ofTranslate(-midPt.x, -midPt.y, -midPt.z);
    

    
    ofSetColor(255,0,0);
    
    //helix0.draw();
    //helix1.draw();
    
    for (int i = 0; i < helix0.size()-1; i++){
        
        ofSetColor(255,0,0);
        ofLine(helix0[i], helix0[i+1]);
        ofSetColor(0,255,0);
        
        ofVec3f temp0 = helix0[i];
        ofVec3f temp1 = helix0[i+1];
        temp0.normalize();
        temp1.normalize();
        
        
        
        ofLine(helix0[i],  helix0[i] - (temp0.cross(temp1)).normalize() * 10.0);
        
    }
    

    
    
    
    
//    for (int i = 0; i < lines.size(); i++){
//            ofLine(lines[i].a, lines[i].b);
//        }
    
    ofPopMatrix();
}



ofVec3f helix::getMidPoint() {
    ofPoint midPt;
    int count = 0;
    
    for (int i = 0; i < helix0.size(); i++){
        midPt += helix0[i];
        count++;
    }
    
    for (int i = 0; i < helix1.size(); i++){
        midPt += helix1[i];
        count++;
    }
    
    for (int i = 0; i < lines.size(); i++){
        midPt += lines[i].a;
        midPt += lines[i].b;
        count += 2;
    }
    
    midPt /= (float)count;
    
    return midPt;
}

