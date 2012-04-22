/**
 *    Introduction to GPU Programming with GLSL
 *
 *  Fragment Shader -- Wireframe shader - Tubes
 *
 *  Marroquim, Ricardo -- Apr, 2012
 *
 **/

//uniform vec2 viewport;
varying vec3 v0, v1, v2;
varying vec3 normal, vert;

vec3 closest_point;
vec3 edge_vector;
float min_dist = 10000.0;

// computes the dist from a pt to an edge, if min is true update the closest pt info
float distToEdge (vec3 vert0, vec3 vert1, vec3 pt, bool min) {
  vec3 line = normalize(vert1 - vert0);
  vec3 vec = pt - vert0;
  vec3 edge_point = dot(vec, line) * line;
  float dist = length(vec - edge_point);
  
  if (min && dist < min_dist) {
    min_dist = dist;
    closest_point = edge_point;
    edge_vector = line;
  }

  return dist;
  
} 

void main(void) {

  // compute minimum distance from current interpolated 3d vertex to triangle edges
 
  distToEdge(v0, v1, vert, true);

  distToEdge(v1, v2, vert, true);

  distToEdge(v2, v0, vert, true);

  // maximum perpendicular distance to edges inside the triangle is at the centroid
  vec3 centroid = (v0 + v1 + v2) / 3.0;
  float radius = distToEdge(v1, v2, centroid, false) * 0.25;

  // discard fragments outside the tube radius
  if (min_dist > radius)
   discard;

  float dist = min_dist/radius;
  vec3 u = normalize(vert - closest_point); //vector perpendicular to normal
  vec3 v = normalize(cross(edge_vector, u)); // edge normal vector
  float theta = asin(dist); // tube_normal
  vec3 tube_normal = normalize(cos(theta)*u + sin(theta)*v);
  
  tube_normal.z *= -1.0;

  vec4 color = vec4(0.3, 0.7, 0.7, 1.0);

  // do some phong shading
  vec3 light_dir = normalize( gl_LightSource[0].position.xyz - vert );
  vec3 eye_dir = normalize( -vert.xyz ); // vec(0) - vert
  vec3 ref = normalize( -reflect( light_dir, tube_normal ) );

  vec4 la = gl_FrontLightProduct[0].ambient;
  vec4 ld = gl_FrontLightProduct[0].diffuse * max(0.0, dot(tube_normal, light_dir) );
  vec4 ls = gl_FrontLightProduct[0].specular * pow( max(0.0, dot(ref, eye_dir) ), 0.1*gl_FrontMaterial.shininess );

  gl_FragColor = gl_FrontLightModelProduct.sceneColor + ld + ls + la;

}
