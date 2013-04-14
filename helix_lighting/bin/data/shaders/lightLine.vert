#version 120
#extension GL_ARB_texture_rectangle : enable

uniform vec3 lightPos;
varying vec4 vertex;
//uniform int lightIndex;

void main() {
    
    vertex = gl_ModelViewMatrix * gl_Vertex;
//    vertex = gl_Vertex;
    
//    lightPos = gl_LightSource[lightIndex].position;

    // divide the color alpha by the area
    gl_FrontColor = gl_Color;
//    gl_Position = ftransform();
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;;
    
}
