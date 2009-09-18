/**
 *    Introduction to GPU Programming with GLSL
 *
 *  Vertex Shader -- Phong shader
 *
 *  Maximo, Andre -- Mar, 2009
 *
 **/

varying vec3 normal, vert;

void main(void) {

	normal = normalize( gl_NormalMatrix * gl_Normal ); // Store normal to FS
	vert = vec3( gl_ModelViewMatrix * gl_Vertex ); // Store vertex to FS

	gl_FrontColor = gl_Color; // Pass color
	gl_Position = ftransform(); // Pass vertex

}
