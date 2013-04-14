#version 120

uniform vec3 lightPos;
varying vec4 vertex;
//uniform int lightIndex;
uniform float lightRadius;

float mapValue( float value, float inputMin, float inputMax, float outputMin, float outputMax ) {
    return ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
}

void main() {
    vec3 lightDir = lightPos - vertex.xyz;
    float distance = length( lightDir.xyz );
    
//    vec4 ambient = vec4(0.0, 0.0, 0.0, 1.0);
//    vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 specular = vec4(0.0, 0.0, 0.0, 0.0);
    
    if(distance < lightRadius) {
        float distancePercent = distance/lightRadius;
        float damping_factor = 1.0 - pow(distancePercent, 3);
        
//        specular = gl_LightSource[lightIndex].specular;
        specular = vec4(1.0, 1.0, 1.0, 1.0);
//        specular *= damping_factor;
//        specular.a = damping_factor;
        specular.a = 1.0 - distancePercent;
        if(specular.a > 0.7) {
            specular.a = 1.0;
        } else {
            specular.a = mapValue( specular.a, 0.0, 0.7, 0.0, 1.0 );
        }
    }
//    specular.a = 0.0;
    
    vec4 final_color = vec4(specular);
    
	gl_FragColor = final_color;
}

