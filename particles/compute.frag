/**	
 *    Introduction to GPU Programming with GLSL
 *
 *  Marroquim, Ricardo -- Sep, 2009
 *
 **/
#version 120

uniform sampler2D positionTex;
uniform sampler2D velocityTex;
uniform sampler2D originalVelocityTex;

uniform float time_step;
uniform int step;
uniform int numParticles;

uniform vec3 gravity;
//const vec3 gravity = vec3 (0.0, -0.7, 0.0);

void main(void) {

	vec3 position = texture2D( positionTex, gl_TexCoord[0].st ).xyz;
	vec3 velocity = texture2D( velocityTex, gl_TexCoord[0].st ).xyz;
	float particleId = texture2D( positionTex, gl_TexCoord[0].st ).w;

	position.xyz = position.xyz + velocity.xyz*time_step;
	velocity.xyz = velocity.xyz + gravity.xyz*time_step;

	position = clamp (position, -1.0, 1.0);

	//if (position.y <= -1.0)
	{
		if (int(particleId*float(numParticles)) > step)
		{
			position = vec3(0.0);
			velocity = texture2D( originalVelocityTex, gl_TexCoord[0].st ).xyz;
			velocity =  vec3(gl_ModelViewMatrixInverse * vec4 (velocity, 1.0));
			
			//normalize(velocity);
		}
	}

	gl_FragData[0] = vec4( position.xyz, particleId);
	gl_FragData[1] = vec4( velocity.xyz, 1.0);
}
