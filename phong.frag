/**
 *    Introduction to GPU Programming with GLSL
 *
 *  Fragment Shader -- Phong shader
 *
 *  Maximo, Andre -- Mar, 2009
 *
 **/

varying vec3 normal, vert;

void main(void) {

	vec3 light_dir = normalize( gl_LightSource[0].position.xyz - vert );

	vec3 eye_dir = normalize( -vert.xyz );

	vec3 ref = normalize( -reflect( light_dir, normal ) );

	vec4 la = gl_FrontLightProduct[0].ambient;
	vec4 ld = gl_FrontLightProduct[0].diffuse * max( dot(normal, light_dir), 0.0 );
	vec4 ls = gl_FrontLightProduct[0].specular
		* pow( max( dot(ref, eye_dir), 0.0 ), gl_FrontMaterial.shininess );

	gl_FragColor = gl_FrontLightModelProduct.sceneColor + la + ld + ls;

}
