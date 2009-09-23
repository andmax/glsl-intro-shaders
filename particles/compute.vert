/**
 *    Introduction to GPU Programming with GLSL
 *
 *  Marroquim, Ricardo -- Sep, 2009
 *
 **/



void main(void) {
	

	
	//gravity = vec3 (0.0, -1.0, 0.0);
	
	gl_TexCoord[0] = gl_MultiTexCoord0 *  gl_TextureMatrix[0];
	gl_Position = gl_ProjectionMatrix * gl_Vertex;
	
}
