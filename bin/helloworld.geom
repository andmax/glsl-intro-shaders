/**
 *    Introduction to GPU Programming with GLSL
 *
 *  Geometry Shader -- Hello World (simple passthrough shader)
 *
 *  Maximo, Andre -- Mar, 2009
 *
 **/

#version 120

#extension GL_EXT_geometry_shader4 : enable

void main(void) {

	for (int i = 0; i < gl_VerticesIn; ++i) {

		gl_FrontColor = gl_FrontColorIn[i]; // Pass color
		gl_Position = gl_PositionIn[i]; // Pass position

		EmitVertex(); // Done with this vertex

	}

	EndPrimitive(); // Done with the input primitive

}
