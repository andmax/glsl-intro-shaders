/**	
 *    Introduction to GPU Programming with GLSL
 *
 *  Fragment Shader -- Normal map shader
 *
 *  Maximo, Andre -- Jul, 2009
 *
 **/

varying vec3 vert, norm;

uniform sampler2D normalMapTex;
uniform bool applyTex;

void main(void) {
	
    vec3 texel = texture2D( normalMapTex, vec2(gl_TexCoord[0].s, gl_TexCoord[0].t) ).rgb;

    if( !applyTex ) {
        gl_FragColor = vec4(texel, 1.0);
        return;

    }


}

//~ void main(void) {
//~ 
	//~ //vec3 texel = texture2D( normalMapTex, gl_TexCoord[0].st ).rgb;
	//~ vec3 texel = texture2D( normalMapTex, vec2(-gl_TexCoord[0].s, gl_TexCoord[0].t) ).rgb;
	//~ vec3 normal;
//~ 
	//~ if( applyTex )
		//~ normal = norm;
	//~ else
		//~ normal = texel;
//~ 
	//~ vec3 light_dir = normalize( gl_LightSource[0].position.xyz - vert );
//~ 
	//~ vec3 eye_dir = normalize( -vert.xyz );
//~ 
	//~ vec3 ref = normalize( -reflect( light_dir, normal ) );
//~ 
	//~ vec4 la = gl_FrontLightProduct[0].ambient;
	//~ vec4 ld = gl_FrontLightProduct[0].diffuse * max( dot(normal, light_dir), 0.0 );
	//~ vec4 ls = gl_FrontLightProduct[0].specular
		//~ * pow( max( dot(ref, eye_dir), 0.0 ), gl_FrontMaterial.shininess );
//~ 
	//~ if( applyTex )
		//~ gl_FragColor = vec4(texel, 1.0) + la + ld + ls;
	//~ else
		//~ gl_FragColor = gl_FrontLightModelProduct.sceneColor + la + ld + ls;
//~ 
//~ 
//~ }
