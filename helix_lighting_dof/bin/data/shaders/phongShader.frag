#version 120

varying vec3 normal;
varying vec3 vertex;

float mapValue( float value, float inputMin, float inputMax, float outputMin, float outputMax ) {
    return ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
}

void main() {
    vec3 L = normalize(gl_LightSource[0].position.xyz - vertex);
    vec3 E = normalize(-vertex); // we are in Eye Coordinates, so EyePos is (0,0,0)
    vec3 R = normalize(-reflect(L, normal));
    
    //calculate Ambient Term:
    vec4 Iamb = gl_FrontLightProduct[0].ambient;
    
    //calculate Diffuse Term:
    vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(normal,L), 0.0);
    Idiff = clamp(Idiff, 0.0, 1.0);
    
    // calculate Specular Term:
    vec4 Ispec = gl_FrontLightProduct[0].specular * pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);
    Ispec = clamp(Ispec, 0.0, 1.0);
    
    float newalpha = Ispec.r + Idiff.r;
    newalpha = clamp( newalpha, 0.0, 1.0 );
    // write Total Color:
    vec4 final_color = gl_FrontLightModelProduct.sceneColor + Iamb + Idiff + Ispec;
    final_color.a = newalpha;
    gl_FragColor = final_color;
//    gl_FragColor.a = newalpha;
}

