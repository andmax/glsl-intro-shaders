/**
 *
 *        glslKernel.cc
 *
 *  Base class for GLSL application using GLee
 *  Note: To read more about OpenGL Easy Extension (GLee)
 *  library go to:   http://www.opengl.org/sdk/libs/GLee
 *
 *  Authors:
 *    Claudio Esperanca -- esperanc@lcg.ufrj.br
 *    Andre Maximo -- andre@lcg.ufrj.br
 *
 *  Location:
 *    Computer Graphics Laboratory (LCG) in
 *    Rio de Janeiro Federal University (UFRJ)
 *    www.lcg.ufrj.br
 *
 *  Date:
 *    Created in August, 2005
 *    Last update in November, 2007
 *
 **/

#include <cassert>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>

#include "glslKernel.h"
#ifdef __MAC__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

using namespace std;

///
/// Auxiliary Functions
///

/// Tells whether the system support OpenGL SL capabilities
/// @return true if the system is ready for OpenGL SL
bool glsl_support() { 
#ifdef __MAC__
  return true;
#else
  return (GLEE_VERSION_2_0);
#endif
}

/// Tells whether graphics board support Geometry Shader
/// @return true if the graphics board could run Geometry Shader
bool geom_shader_support () { 
#ifdef __MAC__
  return true;
#else
  return (GLEE_EXT_geometry_shader4);
#endif
}

/// Print out the attribytes information for a GLSL program 
/// @arg prg handle for a GLSL program
static void check_attributes (GLuint prg) {

	GLsizei bufSize = 255, length;
	GLint size;
	GLenum type;
	GLchar name[bufSize];
	GLint num_attribs;

	glGetProgramiv(prg, GL_ACTIVE_ATTRIBUTES, &num_attribs);

	for (GLint i=0; i<num_attribs; ++i) {
		glGetActiveAttrib(prg, i, bufSize, &length, &size, &type, name);
		cout << "attrib[" << i << "]: name = " << name << " ; length = "
			<< length << " ; size = " << size << " type = "
			<< (type==GL_INT?"int":(type==GL_FLOAT?"float":
			(type==GL_FLOAT_VEC4?"vec4":(type==GL_FLOAT_VEC3?"vec3":
			(type==GL_FLOAT_VEC2?"vec2":"_"))))) << endl;
	}

}

/// Print out the information log for a shader object 
/// @arg obj handle for a shader object
static void printShaderInfoLog (GLuint obj) {

	GLint infologLength = 0, charsWritten = 0;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 2) {

		GLchar* infoLog = new GLchar [infologLength];
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		cerr << infoLog << endl;
		delete infoLog;

	}

}

/// Print out the information log for a shader object 
/// @arg obj handle for a program object
static void printProgramInfoLog (GLuint obj) {

	GLint infologLength = 0, charsWritten = 0;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 2) {

		GLchar* infoLog = new GLchar [infologLength];
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		cerr << infoLog << endl;
		delete infoLog;

	}

}

/// Returns 1 if an OpenGL error occurred, 0 otherwise.
/// @arg str error string specifying where the error occurs
static int error_check (const char* str = NULL) {

	GLenum glErr;
	int retCode = 0;

	glErr = glGetError();

	while (glErr != GL_NO_ERROR) {

		cerr << "glError : " << gluErrorString(glErr) << " : "
		     << str << "\n";
		retCode = 1;
		glErr = glGetError();

	}

	return retCode;

}

/// Local-static variables
static std::vector <char> buffer; ///< Buffer for storing texts for shaders
static std::vector <char*> line; ///< Points to individual lines inside buffer

/// Utility method for reading a text file one line at a time into array
/// of strings 'line'. Vector 'buffer' provides storage for the text.
/// @arg filename name of text file
static void load_file (const char* filename) {

	ifstream f (filename);
	vector<int> n;
	assert (f);
	char buf [1000];
	buffer.clear ();
	line.clear();

	while (!f.eof()) {

		f.getline (buf,1000);
		n.push_back(strlen(buf));
		copy (buf, buf+n.back(), back_inserter (buffer));
		buffer.push_back ('\n');
		buffer.push_back ('\0');

	}

	int length = 0;

	for (unsigned int i = 0; i < n.size (); i++) {

		line.push_back (&buffer[length]);
		length += n[i]+2;

	}

}

///
/// GLSL Kernel class methods
///

/// Constructor
/// @arg geom_source array of strings containing geometry shader source
/// @arg frag_source array of strings containing fragment shader source
/// @arg vtx_source array of strings containing vertex shader source
glslKernel::glslKernel (const char ** geom_source, const char ** frag_source, const char ** vtx_source)
	: programObject (0), geometryShader(0), fragmentShader(0), vertexShader(0),
	  geomSource(geom_source), fragSource(frag_source), vtxSource(vtx_source),
	  geomFileName(0), fragFileName(0), vtxFileName(0),
	  geomVtxOut(3), geomTypeIn(GL_TRIANGLES), geomTypeOut(GL_TRIANGLE_STRIP) {

}

/// Destructor
glslKernel::~glslKernel() {

	if (installed()) {

		glUseProgram(0);

		if (geometryShader) {

			glDetachShader(programObject, geometryShader);
			glDeleteShader(geometryShader);

		}

		if (fragmentShader) {

			glDetachShader(programObject, fragmentShader);
			glDeleteShader(fragmentShader);

		}

		if (vertexShader) {

			glDetachShader(programObject, vertexShader);
			glDeleteShader(vertexShader);

		}

		glDeleteProgram(programObject);

	}

}

/// Size of GLSL Kernel
/// @return openGL usage in Bytes
int glslKernel::size_of(void) {

	return ( ( 4 * sizeof(GLuint) ) + ///< All GLuints
		 ( 3 * sizeof(GLint) ) + ///< All GLints
		 ( 6 * sizeof(int) ) ///< pointers
		);

}

/// Sets the name of a geometry shader source file
/// @arg filename name of geometry shader source file
void glslKernel::geometry_source (const char* filename) {

	geomFileName = filename;

}

/// Sets the name of a fragment shader source file
/// @arg filename name of fragment shader source file
void glslKernel::fragment_source (const char* filename) {

	fragFileName = filename;

}

/// Sets the name of a vertex shader source file
/// @arg filename name of vertex shader source file
void glslKernel::vertex_source (const char* filename) {

	vtxFileName = filename;

}

/// Tells whether the GLSL program is ready to run
/// @return true if and only if a program object was built
bool glslKernel::installed () {

	return programObject != 0;

}

/// Installs the shaders as the current shaders for the current context
/// Compiles and links the shaders if this was not done earlier
/// @arg debug flags the debug information output
void glslKernel::install (bool debug) {

	assert (vtxSource || fragSource || geomSource ||
		vtxFileName || fragFileName || geomFileName);

	if (programObject != 0)
		glDeleteProgram( programObject );

	programObject = glCreateProgram();

	assert( programObject != 0 );

	if (geomSource || geomFileName) {

		geometryShader = glCreateShader (GL_GEOMETRY_SHADER_EXT);

		assert(geometryShader != 0);

		if (geomSource) {

			glShaderSource(geometryShader, 1, geomSource, NULL);

		} else {

			load_file (geomFileName);
			glShaderSource (geometryShader, line.size(), (const GLchar**) (&line[0]), NULL);

		}

		assert (!error_check("Creating Geometry Shader"));

		glCompileShader (geometryShader);

		if (debug) printShaderInfoLog (geometryShader);
		assert (!error_check("Compiling Geometry Shader"));

		GLint compileGeom;
		glGetShaderiv (geometryShader, GL_COMPILE_STATUS, &compileGeom);
		assert (compileGeom == GL_TRUE);

		glAttachShader (programObject, geometryShader);

		glProgramParameteriEXT (programObject, GL_GEOMETRY_VERTICES_OUT_EXT, geomVtxOut);
		glProgramParameteriEXT (programObject, GL_GEOMETRY_INPUT_TYPE_EXT, geomTypeIn);
		glProgramParameteriEXT (programObject, GL_GEOMETRY_OUTPUT_TYPE_EXT, geomTypeOut);

		assert (!error_check("Attaching Geometry Shader"));

	}

	if (fragSource || fragFileName) {

		fragmentShader = glCreateShader (GL_FRAGMENT_SHADER);

		assert(fragmentShader != 0);

		if (fragSource) {

			glShaderSource(fragmentShader, 1, fragSource, NULL);

		} else {

			load_file (fragFileName);
			glShaderSource (fragmentShader, line.size(), (const GLchar**) (&line[0]), NULL);

		}

		assert (!error_check("Creating Fragment Shader"));

		glCompileShader (fragmentShader);

		if (debug) printShaderInfoLog (fragmentShader);
		assert (!error_check("Compiling Fragment Shader"));

		GLint compileFrag;
		glGetShaderiv (fragmentShader, GL_COMPILE_STATUS, &compileFrag);
		assert (compileFrag == GL_TRUE);

		glAttachShader (programObject, fragmentShader);
		assert (!error_check("Attaching Fragment Shader"));
	}

	if (vtxSource || vtxFileName) {

		vertexShader = glCreateShader (GL_VERTEX_SHADER);

		assert(vertexShader != 0);

		if (vtxSource) {

			glShaderSource(vertexShader, 1, vtxSource, NULL);

		} else {

			load_file (vtxFileName);
			glShaderSource (vertexShader, line.size(), (const GLchar**) &line[0], NULL);

		}

		assert (!error_check("Creating Vertex Shader"));

		glCompileShader (vertexShader);

		if (debug) printShaderInfoLog (vertexShader);
		assert (!error_check("Compiling Vertex Shader"));

		GLint compileVertex;
		glGetShaderiv (vertexShader, GL_COMPILE_STATUS, &compileVertex);
		assert (compileVertex == GL_TRUE);

		glAttachShader (programObject, vertexShader);
		assert (!error_check("Attaching Vertex Shader"));

	}

	// Link the shader into a complete GLSL program.
	glLinkProgram(programObject);

	if (debug) printProgramInfoLog (programObject);

	GLint progLinkSuccess;
	glGetProgramiv(programObject, GL_LINK_STATUS, &progLinkSuccess);
	assert (progLinkSuccess);

	if (debug) check_attributes (programObject);

	assert (installed ());

}

/// Sets the current kernel as the one in use
/// @arg use_kernel if false, instructs opengl not to use any kernel 
void glslKernel::use (bool use_kernel) {

	glUseProgram (use_kernel?programObject:0);
	assert (!error_check("Using shaders"));

}

/// Gets an uniform location by name
/// @arg name name of uniform variable
/// @return location handle of uniform variable
GLint glslKernel::get_uniform_location (const GLchar* name) {

	assert (installed());
	return glGetUniformLocation (programObject, name);

}

/// Gets a n-float uniform value by name
/// @arg name name of uniform variable.
/// @arg p pointer to GLfloat array to be filled. The number of floats copied
///        depends on the size of the uniform as specified in the shader prog.
void glslKernel::get_uniform (const GLchar* name, GLfloat *p) {

	assert (installed ());
	assert (programObject);
	GLint location = glGetUniformLocation (programObject, name);
	assert (location != -1);
	glGetUniformfv (programObject, location, p);
	assert (!error_check());

}

/// Sets a {1|2|3|4}-{integer|float|double} uniform value by {name|location}
/// converting double (not accepted) to float
/// @arg name name of uniform variable
/// @arg location location handle of uniform variable
/// @arg a first value
/// @arg b second value
/// @arg c third value
/// @arg d fourth value

/// location & integer
void glslKernel::set_uniform (GLint location, GLint a, GLint b, GLint c, GLint d) {
	assert (location != -1);
	glUniform4i (location, a, b, c, d);
	assert (!error_check());
}
void glslKernel::set_uniform (GLint location, GLint a, GLint b, GLint c) {
	assert (location != -1);
	glUniform3i (location, a, b, c);
	assert (!error_check());
}
void glslKernel::set_uniform (GLint location, GLint a, GLint b) {
	assert (location != -1);
	glUniform2i (location, a, b);
	assert (!error_check());
}
void glslKernel::set_uniform (GLint location, GLint a) {
	assert (location != -1);
	glUniform1i (location, a);
	assert (!error_check());
}
/// name & integer
void glslKernel::set_uniform (const GLchar* name, GLint a, GLint b, GLint c, GLint d) {
	GLint location = get_uniform_location (name);
	set_uniform (location, a, b, c, d);
}
void glslKernel::set_uniform (const GLchar* name, GLint a, GLint b, GLint c) {
	GLint location = get_uniform_location (name);
	set_uniform (location, a, b, c);
}
void glslKernel::set_uniform (const GLchar* name, GLint a, GLint b) {
	GLint location = get_uniform_location (name);
	set_uniform (location, a, b);
}
void glslKernel::set_uniform (const GLchar* name, GLint a) {
	GLint location = get_uniform_location (name);
	set_uniform (location, a);
}
/// location & float
void glslKernel::set_uniform (GLint location, GLfloat a, GLfloat b, GLfloat c, GLfloat d) {
	assert (location != -1);
	glUniform4f (location, a, b, c, d);
	assert (!error_check());
}
void glslKernel::set_uniform (GLint location, GLfloat a, GLfloat b, GLfloat c) {
	assert (location != -1);
	glUniform3f (location, a, b, c);
	assert (!error_check());
}
void glslKernel::set_uniform (GLint location, GLfloat a, GLfloat b) {
	assert (location != -1);
	glUniform2f (location, a, b);
	assert (!error_check());
}
void glslKernel::set_uniform (GLint location, GLfloat a) {
	assert (location != -1);
	glUniform1f (location, a);
	assert (!error_check());
}
/// name & float
void glslKernel::set_uniform (const GLchar* name, GLfloat a, GLfloat b, GLfloat c, GLfloat d) {
	GLint location = get_uniform_location (name);
	set_uniform (location, a, b, c, d);
}
void glslKernel::set_uniform (const GLchar* name, GLfloat a, GLfloat b, GLfloat c) {
	GLint location = get_uniform_location (name);
	set_uniform (location, a, b, c);
}
void glslKernel::set_uniform (const GLchar* name, GLfloat a, GLfloat b) {
	GLint location = get_uniform_location (name);
	set_uniform (location, a, b);
}
void glslKernel::set_uniform (const GLchar* name, GLfloat a) {
	GLint location = get_uniform_location (name);
	set_uniform (location, a);
}
/// location & double
void glslKernel::set_uniform (GLint location, GLdouble a, GLdouble b, GLdouble c, GLdouble d) {
	assert (location != -1);
	glUniform4f (location, (GLfloat)a, (GLfloat)b, (GLfloat)c, (GLfloat)d);
	assert (!error_check());
}
void glslKernel::set_uniform (GLint location, GLdouble a, GLdouble b, GLdouble c) {
	assert (location != -1);
	glUniform3f (location, (GLfloat)a, (GLfloat)b, (GLfloat)c);
	assert (!error_check());
}
void glslKernel::set_uniform (GLint location, GLdouble a, GLdouble b) {
	assert (location != -1);
	glUniform2f (location, (GLfloat)a, (GLfloat)b);
	assert (!error_check());
}
void glslKernel::set_uniform (GLint location, GLdouble a) {
	assert (location != -1);
	glUniform1f (location, (GLfloat)a);
	assert (!error_check());
}
/// name & double
void glslKernel::set_uniform (const GLchar* name, GLdouble a, GLdouble b, GLdouble c, GLdouble d) {
	GLint location = get_uniform_location (name);
	set_uniform (location, a, b, c, d);
}
void glslKernel::set_uniform (const GLchar* name, GLdouble a, GLdouble b, GLdouble c) {
	GLint location = get_uniform_location (name);
	set_uniform (location, a, b, c);
}
void glslKernel::set_uniform (const GLchar* name, GLdouble a, GLdouble b) {
	GLint location = get_uniform_location (name);
	set_uniform (location, a, b);
}
void glslKernel::set_uniform (const GLchar* name, GLdouble a) {
	GLint location = get_uniform_location (name);
	set_uniform (location, a);
}

/// Sets a {integer|float} uniform vector value by {name|location}
/// @arg name name of uniform variable
/// @arg location location handle of uniform variable
/// @arg v vector value
/// @arg nvalues number of values in the vector v
/// @arg count number of elements of the uniform vector array to be modified
///            a count of 1 should be used if modifying the value of a single vector

/// location and integer
void glslKernel::set_uniform (GLint location, const GLint* v, GLuint nvalues, GLsizei count) {

	assert (location != -1);
	assert ((nvalues > 0) && (nvalues < 5));

	switch (nvalues) {

	case 1: glUniform1iv (location, count, v); break;
	case 2: glUniform2iv (location, count, v); break;
	case 3: glUniform3iv (location, count, v); break;
	case 4: glUniform4iv (location, count, v); break;

	}

	assert (!error_check());

}
/// location and float
void glslKernel::set_uniform (GLint location, const GLfloat* v, GLuint nvalues, GLsizei count) {

	assert (location != -1);
	assert ((nvalues > 0) && (nvalues < 5));

	switch (nvalues) {

	case 1: glUniform1fv (location, count, v); break;
	case 2: glUniform2fv (location, count, v); break;
	case 3: glUniform3fv (location, count, v); break;
	case 4: glUniform4fv (location, count, v); break;

	}

	assert (!error_check());

}
/// name and integer
void glslKernel::set_uniform (const GLchar* name, const GLint* v, GLuint nvalues, GLsizei count) {

	GLint location = get_uniform_location (name);
	set_uniform (location, v, nvalues, count);

}
/// name and float
void glslKernel::set_uniform (const GLchar* name, const GLfloat* v, GLuint nvalues, GLsizei count) {

	GLint location = get_uniform_location (name);
	set_uniform (location, v, nvalues, count);

}

/// Sets a uniform matrix value by {name|location}
/// @arg name name of uniform variable
/// @arg location location handle of uniform variable
/// @arg m matrix value with dim^2 values
/// @arg dim dimension of the matrix m
/// @arg transpose if transpose is GL_FALSE, each matrix is assumed to be supplied in
///                column major order, otherwise is in row major order.
/// @arg count number of elements of the uniform matrix array to be modified
///            a count of 1 should be used if modifying the value of a single matrix

/// location
void glslKernel::set_uniform_matrix (GLint location, const GLfloat* m, GLuint dim,
		GLboolean transpose, GLsizei count) {

	assert (location != -1);
	assert ((dim > 1) && (dim < 5));

	switch (dim) {

	case 2: glUniformMatrix2fv (location, count, transpose, m); break;
	case 3: glUniformMatrix3fv (location, count, transpose, m); break;
	case 4: glUniformMatrix4fv (location, count, transpose, m); break;

	}

	assert (!error_check());

}
/// name
void glslKernel::set_uniform_matrix (const GLchar* name, const GLfloat* m, GLuint dim,
		GLboolean transpose, GLsizei count) {

	GLint location = get_uniform_location (name);
	set_uniform_matrix (location, m, dim, transpose, count);

}

/// Gets an attribute index by name
/// @arg name name of attribute variable
/// @return location handle of attribute variable
GLint glslKernel::get_attribute_index (const GLchar* name) {

	assert (installed());
	assert (programObject);
	return glGetAttribLocation (programObject, name);

}

/// Sets a {1|2|3|4}-{short|float|double} attribute value by {name|index}
/// @arg name name of attribute variable
/// @arg index index handle of attribute variable
/// @arg a first value
/// @arg b second value
/// @arg c third value
/// @arg d fourth value

/// index & short
void glslKernel::set_attribute (GLint index, GLshort a, GLshort b, GLshort c, GLshort d) {
	assert (index != -1);
	glVertexAttrib4s (index, a, b, c, d);
	assert (!error_check());
}
void glslKernel::set_attribute (GLint index, GLshort a, GLshort b, GLshort c) {
	assert (index != -1);
	glVertexAttrib3s (index, a, b, c);
	assert (!error_check());
}
void glslKernel::set_attribute (GLint index, GLshort a, GLshort b) {
	assert (index != -1);
	glVertexAttrib2s (index, a, b);
	assert (!error_check());
}
void glslKernel::set_attribute (GLint index, GLshort a) {
	assert (index != -1);
	glVertexAttrib1s (index, a);
	assert (!error_check());
}
/// name & short
void glslKernel::set_attribute (const GLchar* name, GLshort a, GLshort b, GLshort c, GLshort d) {
	GLint index = get_attribute_index (name);
	set_attribute (index, a, b, c, d);
}
void glslKernel::set_attribute (const GLchar* name, GLshort a, GLshort b, GLshort c) {
	GLint index = get_attribute_index (name);
	set_attribute (index, a, b, c);
}
void glslKernel::set_attribute (const GLchar* name, GLshort a, GLshort b) {
	GLint index = get_attribute_index (name);
	set_attribute (index, a, b);
}
void glslKernel::set_attribute (const GLchar* name, GLshort a) {
	GLint index = get_attribute_index (name);
	set_attribute (index, a);
}
/// index & float
void glslKernel::set_attribute (GLint index, GLfloat a, GLfloat b, GLfloat c, GLfloat d) {
	assert (index != -1);
	glVertexAttrib4f (index, a, b, c, d);
	assert (!error_check());
}
void glslKernel::set_attribute (GLint index, GLfloat a, GLfloat b, GLfloat c) {
	assert (index != -1);
	glVertexAttrib3f (index, a, b, c);
	assert (!error_check());
}
void glslKernel::set_attribute (GLint index, GLfloat a, GLfloat b) {
	assert (index != -1);
	glVertexAttrib2f (index, a, b);
	assert (!error_check());
}
void glslKernel::set_attribute (GLint index, GLfloat a) {
	assert (index != -1);
	glVertexAttrib1f (index, a);
	assert (!error_check());
}
/// name & float
void glslKernel::set_attribute (const GLchar* name, GLfloat a, GLfloat b, GLfloat c, GLfloat d) {
	GLint index = get_attribute_index (name);
	set_attribute (index, a, b, c, d);
}
void glslKernel::set_attribute (const GLchar* name, GLfloat a, GLfloat b, GLfloat c) {
	GLint index = get_attribute_index (name);
	set_attribute (index, a, b, c);
}
void glslKernel::set_attribute (const GLchar* name, GLfloat a, GLfloat b) {
	GLint index = get_attribute_index (name);
	set_attribute (index, a, b);
}
void glslKernel::set_attribute (const GLchar* name, GLfloat a) {
	GLint index = get_attribute_index (name);
	set_attribute (index, a);
}
/// index & double
void glslKernel::set_attribute (GLint index, GLdouble a, GLdouble b, GLdouble c, GLdouble d) {
	assert (index != -1);
	glVertexAttrib4d (index, a, b, c, d);
	assert (!error_check());
}
void glslKernel::set_attribute (GLint index, GLdouble a, GLdouble b, GLdouble c) {
	assert (index != -1);
	glVertexAttrib3d (index, a, b, c);
	assert (!error_check());
}
void glslKernel::set_attribute (GLint index, GLdouble a, GLdouble b) {
	assert (index != -1);
	glVertexAttrib2d (index, a, b);
	assert (!error_check());
}
void glslKernel::set_attribute (GLint index, GLdouble a) {
	assert (index != -1);
	glVertexAttrib1d (index, a);
	assert (!error_check());
}
/// name & double
void glslKernel::set_attribute (const GLchar* name, GLdouble a, GLdouble b, GLdouble c, GLdouble d) {
	GLint index = get_attribute_index (name);
	set_attribute (index, a, b, c, d);
}
void glslKernel::set_attribute (const GLchar* name, GLdouble a, GLdouble b, GLdouble c) {
	GLint index = get_attribute_index (name);
	set_attribute (index, a, b, c);
}
void glslKernel::set_attribute (const GLchar* name, GLdouble a, GLdouble b) {
	GLint index = get_attribute_index (name);
	set_attribute (index, a, b);
}
void glslKernel::set_attribute (const GLchar* name, GLdouble a) {
	GLint index = get_attribute_index (name);
	set_attribute (index, a);
}

/// Associates an attribute with a specefic index.
/// @arg name name of attribute variable
/// @arg location index of attribute variable
void glslKernel::bind_attribute_location (const GLchar* name, GLint index) {

	glBindAttribLocation (programObject, index, name);

}

/// Sets the maximum number of output vertices by the Geometry Shader
/// @arg vtx_out maximum number of output vertices
void glslKernel::set_geom_max_output_vertices (const GLint& vtx_out) {

	geomVtxOut = vtx_out;

}

/// Sets the input primitive type
/// @arg type_in input primitive type
void glslKernel::set_geom_input_type (const GLint& type_in) {

	geomTypeIn = type_in;

}

/// Sets the output primitive type
/// @arg type_out output primitive type
void glslKernel::set_geom_output_type (const GLint& type_out) {

	geomTypeOut = type_out;

}
