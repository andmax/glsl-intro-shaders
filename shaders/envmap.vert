/**
 *    Introduction to GPU Programming with GLSL
 *
 *  Vertex Shader -- Environment map shader
 *
 *  Maximo, Andre -- May, 2009
 *
 **/

varying vec3 r, vert, normal;

void main(void) {
	vert = vec3( gl_ModelViewMatrix * gl_Vertex ); // Store vertex to FS
	normal = normalize( gl_NormalMatrix * gl_Normal ); // Store normal to FS
	
	gl_TexCoord[0] = gl_MultiTexCoord0;// * gl_TextureMatrix[0];
	vec3 u = normalize( vec3(gl_ModelViewMatrix * gl_Vertex) );
	vec3 n = normalize( gl_NormalMatrix * gl_Normal );
	r = reflect( u, n );
	gl_Position = ftransform();
	gl_FrontColor = gl_Color; // Pass color
}
