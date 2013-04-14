#version 120

varying vec2 gEndpoints[2];
uniform float drawBack;
varying vec4 modelViewVert;


float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x) {
     return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise(vec3 v)
  { 
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

// First corner
  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

// Other corners
  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  //   x0 = x0 - 0.0 + 0.0 * C.xxx;
  //   x1 = x0 - i1  + 1.0 * C.xxx;
  //   x2 = x0 - i2  + 2.0 * C.xxx;
  //   x3 = x0 - 1.0 + 3.0 * C.xxx;
  vec3 x1 = x0 - i1 + C.xxx;
  vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
  vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y

// Permutations
  i = mod289(i); 
  vec4 p = permute( permute( permute( 
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

// Gradients: 7x7 points over a square, mapped onto an octahedron.
// The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
  float n_ = 0.142857142857; // 1.0/7.0
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
  //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

//Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

// Mix final noise value
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), 
                                dot(p2,x2), dot(p3,x3) ) );
  }


float DistToLine(vec2 pt1, vec2 pt2, vec2 testPt)
{
    vec2 lineDir = pt2 - pt1;
    vec2 perpDir = vec2(lineDir.y, -lineDir.x);
    vec2 dirToPt1 = pt1 - testPt;
    return abs(dot(normalize(perpDir), dirToPt1));
}



void main(){

float rangeZeq0 = 0.0005;
	float threshold = 0.915;
   
    float xx = sin((gl_FragCoord.z - threshold) * 10000.0) * 10.0;
    

    vec2 x1 = gEndpoints[0] * vec2(512.0 + xx, 384.0) + vec2(512.0 + xx, 384.0);
    vec2 x2 = gEndpoints[1] * vec2(512.0 + xx, 384.0) + vec2(512.0 + xx, 384.0);
    float d = 1.0 - (DistToLine(x1,x2, gl_FragCoord.xy) / 20.0);
    if (d < 0.0) d = 0.0;
    d = pow(d, 1.1);
    
	 
    float color =  0.4; //rand(modelViewVert.xy ); // DistToLine(x1,x2, gl_FragCoord.xy)/10.0;

	vec4 colorFar = vec4(0.9, 0.9, 0.9, d * 0.5);
	vec4 colorNear = vec4(color, color, color, d * 0.5);
	vec4 colorZero = vec4(0.0, 0.0, 0.0, d * 0.5);
	
    
    
    
	if (gl_FragCoord.z < threshold){		
		if (gl_FragCoord.z < (threshold - rangeZeq0)){
        
            if (drawBack < 0.5){
                gl_FragColor = colorNear;
            }   else {
                discard;
            }
            
		} else {
			float pct = (threshold - gl_FragCoord.z) / rangeZeq0;
            
            if (drawBack < 0.5){
                gl_FragColor = mix( colorZero , colorNear, (pct));
            }   else {
                discard;
            }
		}
	} else {
		if (gl_FragCoord.z > (threshold + rangeZeq0)){
			//gl_FragColor = colorFar;
            
            if (drawBack > 0.5){
                gl_FragColor = colorFar;
            }   else {
                discard;
            }
            

		} else {
			float pct = (gl_FragCoord.z - threshold) / rangeZeq0;
            if (drawBack > 0.5){
			gl_FragColor = mix(colorZero, colorFar , pct);
             }   else {
                discard;
            }
		}
	}
    
    
	
}