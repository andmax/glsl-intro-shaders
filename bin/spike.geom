/**
 *    Introduction to GPU Programming with GLSL
 *
 *  Geometry Shader -- Spike effect
 *
 *  Maximo, Andre -- Mar, 2009
 *
 **/

#version 120
#extension GL_EXT_gpu_shader4 : enable
#extension GL_EXT_geometry_shader4: enable

varying out vec3 normal, vert;

void main() {

	/// Compute triangle-in centroid
	vec3 c = ( gl_PositionIn[0].xyz + gl_PositionIn[1].xyz + gl_PositionIn[2].xyz ) / 3.0;

	/// Compute face normal
	vec3 v01 = gl_PositionIn[1].xyz - gl_PositionIn[0].xyz;
	vec3 v02 = gl_PositionIn[2].xyz - gl_PositionIn[0].xyz;
	vec3 fn = -cross( v01, v02 );

	/// Compute middle vertex position and color
	vec3 mt = ( gl_TexCoordIn[0][0].xyz + gl_TexCoordIn[1][0].xyz + gl_TexCoordIn[2][0].xyz ) / 3.0;
	vec3 mc = ( gl_FrontColorIn[0].xyz + gl_FrontColorIn[1].xyz + gl_FrontColorIn[2].xyz ) / 3.0;
	vec3 mp = c + 0.5*fn;

	/// Generate 3 triangles using middle vertex
	for (int i = 0; i < gl_VerticesIn; ++i) {

		v01 = gl_PositionIn[ (i+1)%3 ].xyz - gl_PositionIn[ i ].xyz;
		v02 = mp - gl_PositionIn[ i ].xyz;
		fn = -cross(v01, v02);

		gl_TexCoord[0] = gl_TexCoordIn[i][0];
		gl_FrontColor = gl_FrontColorIn[i];
		gl_Position = gl_ModelViewProjectionMatrix * gl_PositionIn[i];
		normal = normalize(fn);
		vert = vec3( gl_ModelViewMatrix * gl_PositionIn[i] );
		EmitVertex();

		gl_TexCoord[0] = gl_TexCoordIn[ (i+1)%3 ][0];
		gl_FrontColor = gl_FrontColorIn[ (i+1)%3 ];
		gl_Position = gl_ModelViewProjectionMatrix * gl_PositionIn[ (i+1)%3 ];
		normal = normalize(fn);
		vert = vec3( gl_ModelViewMatrix * gl_PositionIn[ (i+1)%3 ] );
		EmitVertex();

		gl_TexCoord[0] = vec4( mt, 1.0 );
		gl_FrontColor = vec4(mc, 1.0);
		gl_Position = gl_ModelViewProjectionMatrix * vec4( mp, 1.0 );
		normal = normalize(fn);
		vert = vec3( gl_ModelViewMatrix * vec4( mp, 1.0 ) ).xyz;
		EmitVertex();

		EndPrimitive();

	}

}
