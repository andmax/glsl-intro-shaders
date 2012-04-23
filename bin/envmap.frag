/**
 *    Introduction to GPU Programming with GLSL
 *
 *  Fragment Shader -- Environment map shader
 *
 *  Maximo, Andre -- May, 2009
 *  Marroquim, Ricardo -- 2009
 *
 **/

varying vec3 r, vert, normal;

uniform sampler2D envMapTex;

void main(void) {

	/* Phong shading */
	vec3 light_dir = normalize( gl_LightSource[0].position.xyz - vert );
	vec3 eye_dir = normalize( -vert.xyz );
	vec3 ref = normalize( -reflect( light_dir, normal ) );
	vec4 la = gl_FrontLightProduct[0].ambient;
	vec4 ld = gl_FrontLightProduct[0].diffuse * max( dot(normal, light_dir), 0.0 );
	vec4 ls = gl_FrontLightProduct[0].specular
		* pow( max( dot(ref, eye_dir), 0.0 ), gl_FrontMaterial.shininess );

	/* Environment mapping */
	// map from screen coords range [-1, 1] to texture coords [0,1]
	vec2 coord = vec2(r.x/2.0 + 0.5, r.y/2.0 + 0.5);
	vec4 env = texture2D( envMapTex, coord.st);
	
	gl_FragColor = env*1.0 + (gl_FrontLightModelProduct.sceneColor + la + ld + ls)*0.2;
}
