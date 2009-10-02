/**
 *    Introduction to GPU Programming with GLSL
 *
 *  Vertex Shader -- Simple shader
 *
 *  Maximo, Andre -- Mar, 2009
 *
 **/

varying vec3 normal;

void main(void) {


	normal = normalize( gl_NormalMatrix * gl_Normal );

	gl_FrontColor = gl_Color; 
	gl_Position = gl_ModelViewMatrix * gl_Vertex;

}
