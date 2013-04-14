
varying vec2 pta;
varying vec2 ptb;
varying vec4 pos;
varying vec2 gEndpoints[2];
varying float offsetAmount;
uniform float bump;
//varying vec3 normal;

float DistToLine(vec2 pt1, vec2 pt2, vec2 testPt)
{
    vec2 lineDir = pt2 - pt1;
    vec2 perpDir = vec2(lineDir.y, -lineDir.x);
    vec2 dirToPt1 = pt1 - testPt;
    return abs(dot(normalize(perpDir), dirToPt1));
}


void main() {
    
    
    
    vec2 x1 = gEndpoints[0] * vec2(512.0, 384.0) + vec2(512.0, 384.0);
    vec2 x2 = gEndpoints[1] * vec2(512.0, 384.0) + vec2(512.0, 384.0);
    float d = (DistToLine(x1,x2, gl_FragCoord.xy)) / offsetAmount;
    
    
	gl_FragColor = gl_Color; //vec4( d ); //vec4(distance/1000.0);
    gl_FragColor.a = (d + bump/10.0);
}

