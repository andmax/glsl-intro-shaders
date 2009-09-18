#version 120
#extension GL_EXT_gpu_shader4: enable
#extension GL_EXT_geometry_shader4: enable

varying in vec3 normal[3];


void main()
{
	float factor = 0.75;
	
	vec3 v[3];
	v[0]  =   gl_PositionIn[0].xyz;
	v[1]  =   gl_PositionIn[1].xyz;
	v[2]  =   gl_PositionIn[2].xyz;

	vec3 cent = ( v[0] + v[1] + v[2] ) / 3.0;
	
	gl_FrontColor = gl_FrontColorIn[0];
	gl_Position = gl_ProjectionMatrix * vec4( cent + factor * ( v[0] - cent ), 1.0 );
	EmitVertex();
	
	gl_FrontColor = gl_FrontColorIn[1];
	gl_Position = gl_ProjectionMatrix * vec4( cent + factor * ( v[1] - cent ), 1.0 );
	EmitVertex();

	gl_FrontColor = gl_FrontColorIn[2];
	gl_Position = gl_ProjectionMatrix * vec4( cent + factor * ( v[2] - cent ), 1.0 );
	EmitVertex();

	EndPrimitive(); 
}
