#version 120
#extension GL_ARB_texture_rectangle : enable

varying vec3 normal;
varying vec3 vertex;

void main() {
    vertex = vec3(gl_ModelViewMatrix * gl_Vertex);
    normal = normalize(gl_NormalMatrix * gl_Normal);
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
