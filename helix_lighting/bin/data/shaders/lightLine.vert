#version 120
#extension GL_ARB_texture_rectangle : enable

#define MAX_LIGHTS 32
uniform vec3 lightPoss[MAX_LIGHTS];
varying vec4 vertex;
uniform int numLights;
//uniform int lightIndex;
uniform float lightRadiuss[MAX_LIGHTS];

void main() {
    
    vertex = gl_ModelViewMatrix * gl_Vertex;
//    vertex = gl_Vertex;
    
//    lightPos = gl_LightSource[lightIndex].position;
    
//    int i = 0;
//    for(int i = 0; i < numLights; i++ ) {
//        lightPoss[i] = (gl_LightSource[i].position).xyz;
//    }

    // divide the color alpha by the area
    gl_FrontColor = gl_Color;
//    gl_Position = ftransform();
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;;
    
}
