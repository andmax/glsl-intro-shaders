/**
 *    Introduction to GPU Programming with GLSL
 *
 *  Fragment Shader -- Wireframe shader
 *
 *  Marroquim, Ricardo -- Apr, 2012
 *
 **/


varying vec2 v0, v1, v2;
varying vec3 normal, vert;

void main(void) {

  vec2 v[3];
  v[0] = v0;
  v[1] = v1;
  v[2] = v2;
  vec2 screen_vert = gl_FragCoord.xy;
  vec2 line, vec, proj;
  float dist;
  line = normalize(v1 - v0);
  vec = screen_vert - v0;
  proj = dot(vec, line) * line;
  dist = pow(length(vec), 2.0) - pow(length(proj), 2.0);

  line = normalize(v2 - v0);
  proj = dot(vec, line) * line;
  dist = min(dist, pow(length(vec), 2.0) - pow(length(proj), 2.0));

  line = normalize(v2 - v1);
  vec = screen_vert - v1;
  proj = dot(vec, line) * line;
  dist = min(dist, pow(length(vec), 2.0) - pow(length(proj), 2.0));
  
  dist = exp(-0.05*dist);
  
  if (dist < 0.6)
   discard;

  dist  = 1.0 - dist;
  vec4 color = vec4(dist, dist, dist, 1.0);
  //gl_FragColor = color;


  vec3 light_dir = normalize( gl_LightSource[0].position.xyz - vert );

  vec3 eye_dir = normalize( -vert.xyz );

  vec3 ref = normalize( -reflect( light_dir, normal ) );

  //vec4 la = gl_FrontLightProduct[0].ambient;
  vec4 ld = color * abs( dot(normal, light_dir) );
  vec4 ls = color * pow( abs( dot(ref, eye_dir) ), gl_FrontMaterial.shininess );

  gl_FragColor = ld + ls;



	//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Change color to Red

}
