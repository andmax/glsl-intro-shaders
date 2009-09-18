/**
 *    Introduction to GPU Programming with GLSL
 *
 *  Vertex Shader -- Hello World (simple passthrough shader)
 *
 *  Maximo, Andre -- Mar, 2009
 *
 **/

void main(void) {

	gl_FrontColor = gl_Color; // Pass color
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; // Pass position

}
