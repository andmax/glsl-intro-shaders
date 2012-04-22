/**
 *    Introduction to GPU Programming with GLSL
 *
 *  Geometry Shader -- Wireframe effect
 *
 *  Marroquim, Ricardo -- Apr, 2012
 *
 **/


#version 120
#extension GL_EXT_gpu_shader4 : enable
#extension GL_EXT_geometry_shader4: enable

//uniform vec2 viewport;
varying vec3 v0, v1, v2;
varying vec3 normal, vert;

void main() {

  vec3 v01 = gl_PositionIn[1].xyz - gl_PositionIn[0].xyz;
  vec3 v02 = gl_PositionIn[2].xyz - gl_PositionIn[0].xyz;
  vec3 fn =  normalize(-cross( v01, v02 ));


  vec4 proj[3];
  vec3 vertex[3];
  for (int i = 0; i < gl_VerticesIn; ++i) {
    proj[i] = gl_ModelViewProjectionMatrix * gl_PositionIn[i];
    // compute fragcoord position
    //vertex[i] = ((proj[i].xy / proj[i].w) * 0.5 + vec2(0.5)) * viewport;
    vertex[i] = (gl_ModelViewMatrix * gl_PositionIn[i]).xyz;
  }

  for (int i = 0; i < gl_VerticesIn; ++i) {
    v0 = vertex[0];
    v1 = vertex[1];
    v2 = vertex[2];
    vert = vec3( gl_ModelViewMatrix * gl_PositionIn[i] );
    normal = fn;
    gl_Position = proj[i];
    EmitVertex();
  }


  EndPrimitive();
}
