/**
 *    Introduction to GPU Programming with GLSL
 *
 *  Fragment Shader -- Wireframe shader
 *
 *  Marroquim, Ricardo -- Apr, 2012
 *
 **/

//uniform vec2 viewport;
varying vec3 v0, v1, v2;
varying vec3 normal, vert;

void main(void) {

  vec3 line, vec, proj;
  float dist;

  // compute minimum distance from current interpolated 3d vertex to triangle edges
  // edge v1-v0
  line = normalize(v1 - v0);
  vec = vert - v0;
  proj = dot(vec, line) * line;
  dist = length (vec - proj);

  // edge v2-v0
  line = normalize(v2 - v0);
  proj = dot(vec, line) * line;
  dist = min(dist, length (vec - proj));

  // edge v2-v1
  line = normalize(v2 - v1);
  vec = vert - v1;
  proj = dot(vec, line) * line;
  dist = min(dist, length (vec - proj));

  // maximum perpendicular distance to edges inside the triangle is at the centroid
  vec3 centroid = (v0 + v1 + v2) / 3.0;
  vec = centroid - v1;
  proj = dot(vec, line) * line;
  float max_dist = length(vec - proj);
  
  // normalize min distance
  dist /= max_dist;

  // Gaussian falloff from edge
  dist = exp(-2.0*dist*dist);
 
  // discard interior of triangle
  if (dist < 0.5)
   discard;

  dist  = 1.0 - dist;
  vec4 color = vec4(dist, dist, dist, 1.0);
  color = vec4(0.0, 0.0, 0.0, 1.0);
  //gl_FragColor = color;

  // do some phong shading
  vec3 light_dir = normalize( gl_LightSource[0].position.xyz - vert );
  vec3 eye_dir = normalize( -vert.xyz );
  vec3 ref = normalize( -reflect( light_dir, normal ) );

  vec4 ld = color * abs( dot(normal, light_dir) );
  vec4 ls = color * pow( abs( dot(ref, eye_dir) ), gl_FrontMaterial.shininess );

  gl_FragColor = ld + ls;

}
