/**
 *    Introduction to GPU Programming with GLSL
 *
 *  Marroquim, Ricardo -- Sep, 2009
 *
 **/

uniform sampler2D positionTex;

void main(void) {

	gl_TexCoord[0] = gl_MultiTexCoord0 *  gl_TextureMatrix[0];
	
	vec3 vert = texture2D( positionTex, vec2(gl_TexCoord[0].s, gl_TexCoord[0].t) ).rgb;

	//vert = vec3( gl_ModelViewMatrix * gl_Vertex ); // Store vertex to FS

	gl_Position = vec4(vert, 1.0); // Pass vertex

}
