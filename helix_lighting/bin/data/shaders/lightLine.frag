#version 120


#define MAX_LIGHTS 32
uniform vec3 lightPoss[MAX_LIGHTS];
uniform int numLights;
//uniform vec3 lightPos;
varying vec4 vertex;
//uniform int lightIndex;
uniform float lightRadius;

float mapValue( float value, float inputMin, float inputMax, float outputMin, float outputMax ) {
    return ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
}

void main() {
    vec4 specular = vec4(1.0, 1.0, 1.0, 0.0);
    int i = 0;
    for(int i = 0; i < numLights; i++ ) {
//        lightPoss[i] = (gl_LightSource[i].position * gl_ModelViewMatrix).xyz;
    
        vec3 lightDir = lightPoss[i] - vertex.xyz;
        float distance = length( lightDir.xyz );
        
        if(distance < lightRadius) {
            float distancePercent = distance/lightRadius;
            float damping_factor = 1.0 - pow(distancePercent, 3);
            
    //        specular = gl_LightSource[lightIndex].specular;
//            specular = vec4(1.0, 1.0, 1.0, 1.0);
    //        specular *= damping_factor;
    //        specular.a = damping_factor;
            specular.a += 1.0 - distancePercent;
            
            if(specular.a > 1.0) specular.a = 1.0;
        }
    }
    
//    if(specular.a > 0.7) {
//        specular.a = 1.0;
//    } else {
//        specular.a = mapValue( specular.a, 0.0, 0.7, 0.0, 1.0 );
//    }
//    specular.a = 0.0;
    
    vec4 final_color = vec4(specular);
    
	gl_FragColor = final_color;
}

