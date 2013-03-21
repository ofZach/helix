uniform float focusDistance;
uniform float aperture;
uniform float lineWidth;
uniform float aspectRatio;

attribute float side;
attribute vec3 next;

varying vec2 pta;
varying vec2 ptb;
varying vec4 pos;
varying float offsetAmount;
//varying vec3 normal; 

varying vec2 verpos;
varying vec2 gEndpoints[2];


vec2 to_screen(vec3 v)
{
    v = (gl_ModelViewMatrix * vec4(v, 1)).xyz;
    vec4 u = gl_ProjectionMatrix * vec4(v, 1.0);
    return u.xy / u.w;
}


const mat2 rotate90 = mat2(
	0, -1,
	+1, 0);

void main() {
    
    // get the homogeneous 2d positions
    vec4 curScreenPosition = gl_ModelViewProjectionMatrix * gl_Vertex;
    vec4 nextScreenPosition = gl_ModelViewProjectionMatrix * vec4(next, 1.);
    pos = gl_ModelViewMatrix * gl_Vertex;
    pta = curScreenPosition.xy;
    ptb = nextScreenPosition.xy;
    gEndpoints[0] = to_screen(vec3(gl_Vertex.xyz));
    gEndpoints[1] = to_screen(vec3(next));
    vec2 diff = nextScreenPosition.xy - curScreenPosition.xy;
    vec2 normal = normalize(diff) * rotate90;
    float diameter = abs(curScreenPosition.z - focusDistance) * aperture + lineWidth;
    if (diameter < 2.0) diameter = 2.0;
    if (diameter > 10.0) diameter = 10.0;
    

    // divide the color alpha by the area
    gl_FrontColor = gl_Color;
    gl_FrontColor.a /= (diameter / lineWidth);

    vec4 offset = vec4(side * normal * diameter, 0, 0);
    offset.y *= aspectRatio;
    gl_Position = curScreenPosition + offset;
    
    offsetAmount = diameter;
    
    //normal        = normalize(gl_NormalMatrix * gl_Normal);
    

}
