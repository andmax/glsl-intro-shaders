/**
 *    Introduction to GPU Programming with GLSL
 *
 *  Vertex Shader -- Spike effect
 *
 *  Maximo, Andre -- Mar, 2009
 *
 **/

void main(void) {

	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0; // Pass texcoord
	gl_Position = gl_Vertex; // Pass vertex

}
