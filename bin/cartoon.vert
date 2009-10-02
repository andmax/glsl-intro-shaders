/**
 *    Introduction to GPU Programming with GLSL
 *
 *  Vertex Shader -- Cartoon shader
 *
 *  Maximo, Andre -- Mar, 2009
 *
 **/

varying vec3 normal;

void main(void) {

	normal = normalize( gl_NormalMatrix * gl_Normal ); // Store normal to FS

	gl_FrontColor = gl_Color; // Pass color
	gl_Position = ftransform(); // Pass vertex

}
