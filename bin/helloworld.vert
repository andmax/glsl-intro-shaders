/**
 *    Introduction to GPU Programming with GLSL
 *
 *  Vertex Shader -- Hello World (simple passthrough shader)
 *
 *  Maximo, Andre -- Mar, 2009
 *
 **/

void main(void) {

	gl_FrontColor = vec4(1.0,1.0,0.0,1.0); // Pass color
	gl_FrontColor = gl_Vertex.xyzw * gl_Vertex.rgba;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; // Pass position

}
