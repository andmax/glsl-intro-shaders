/**
 *    Introduction to GPU Programming with GLSL
 *
 *  Vertex Shader -- Normal map shader
 *
 *  Maximo, Andre -- Jul, 2009
 *
 **/

varying vec3 vert, norm;

void main(void) {

	gl_TexCoord[0] = gl_MultiTexCoord0 *  gl_TextureMatrix[0];

	vert = vec3( gl_ModelViewMatrix * gl_Vertex ); // Store vertex to FS
	norm = normalize( gl_NormalMatrix * gl_Normal ); // Store normal to FS

	gl_FrontColor = gl_Color; // Pass color
	gl_Position = ftransform(); // Pass vertex

}
