/**
 *
 *    Introduction to GPU Programming with GLSL
 *
 *  GLSL Simple Particle Simulation Using Textures
 *
 *  Marroquim, Ricardo -- September, 2009
 *
 **/

/// -----------------------------------   Definitions   -------------------------------------

#include "glslKernel.h" // using lcg glsl kernel

extern "C" {
#ifdef __MAC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h> // gl-utility library
#endif
}

#include <math.h> 
#include <stdio.h> // standard i/o
#include <stdlib.h>

#include "arcball.h"

#include <iostream> // i/o stream

#define TEXTURE_TYPE GL_RGBA32F_ARB

using std::cout;
using std::cerr;
using std::flush;
using std::endl;

/// ------------------------------------   Variables   --------------------------------------

static int buttonPressed = -1; ///< Button state

static const char titleWin[] = "Simple GLSL Particle Simulation"; ///< Window title

static int winWidth = 1024, winHeight = 768; ///< Window size

static GLuint tex_velocity, tex_originalVelocity, tex_position, fbo; ///< Textures
static GLuint tex_size = 64;
static GLuint numParticles;

static glslKernel computeShader; ///< GLSL Kernel Shaders
static glslKernel displayShader; ///< GLSL Kernel Shaders

// Vertex and Fragment Shader file names
static const char vsFile[2][255] = { "display.vert", "compute.vert" };

static const char fsFile[2][255] = { "display.frag", "compute.frag" };

static GLfloat time_step = 0.01;
static GLint step = 0;

static GLint point_size = 2;
/// ------------------------------------   ARCBALL   --------------------------------------

// scene parameters
const vec eye( 0.0f, 0.0f, 3.0f );
const vec center( 0.0f, 0.0f, -1.0f );
const vec up( 0.0f, 1.0f, 0.0f );
const float SPHERE_RADIUS = 1.0f;
static GLfloat zoom = 0.6;

const float PI = 3.141592654f;

/// ------------------------------------   Functions   --------------------------------------


/// OpenGL Write
/// @arg x, y raster position
/// @arg str string to write

void glWrite( const GLdouble& x, const GLdouble& y, const char *str ) {

	// You should call glColor* before glWrite;
	// And the font color is also affected by texture and lighting
	glRasterPos2d(x, y);
	for (char *s = (char*)str; *s; s++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *s);

}

/// Show Information/Help

void showIH( void ) {

	glColor3f(0.0, 0.0, 0.0);

	char str[256];
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();	
	glLoadIdentity();
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();	
	glLoadIdentity();
	
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

	sprintf(str, "Keys" );
	glWrite(-0.95, 0.9, str);
	
	sprintf(str, "[ / ] : point size" );
	glWrite(-0.95, 0.8, str);

	sprintf(str, "+ / - : animation speed" );
	glWrite(-0.95, 0.7, str);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();


}


inline vec rotate_x( vec v, float sin_ang, float cos_ang )
{
	return vec(
	    v.x,
	    (v.y * cos_ang) + (v.z * sin_ang),
	    (v.z * cos_ang) - (v.y * sin_ang)
	    );
}

inline vec rotate_y( vec v, float sin_ang, float cos_ang )
{
	return vec(
	    (v.x * cos_ang) + (v.z * sin_ang),
	    v.y,
	    (v.z * cos_ang) - (v.x * sin_ang)
	    );
}

/// Renders the currently selected model
/// to insert a new model remember to increment the NUM_MODEL const
void drawModel ( int m ) {


}

void drawBoundingBox(void) {
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	/// Draw bounding box
	glBegin(GL_QUADS);
	///front
	glVertex3f(-1.0, -1.0, -1.0);
	glVertex3f( 1.0, -1.0, -1.0);
	glVertex3f( 1.0,  1.0, -1.0);
	glVertex3f(-1.0,  1.0, -1.0);
	
	///back
	glVertex3f(-1.0, -1.0,  1.0);
	glVertex3f( 1.0, -1.0,  1.0);
	glVertex3f( 1.0,  1.0,  1.0);
	glVertex3f(-1.0,  1.0,  1.0);
	
	///bottom
	glVertex3f(-1.0, -1.0, -1.0);
	glVertex3f( 1.0, -1.0, -1.0);
	glVertex3f( 1.0, -1.0,  1.0);
	glVertex3f(-1.0, -1.0,  1.0);
	
	///top
	glVertex3f(-1.0,  1.0, -1.0);
	glVertex3f( 1.0,  1.0, -1.0);
	glVertex3f( 1.0,  1.0,  1.0);
	glVertex3f(-1.0,  1.0,  1.0);
	
	///left
	glVertex3f(-1.0, -1.0, -1.0);
	glVertex3f(-1.0,  1.0, -1.0);
	glVertex3f(-1.0,  1.0,  1.0);
	glVertex3f(-1.0, -1.0,  1.0);	

	///right
	glVertex3f( 1.0, -1.0, -1.0);
	glVertex3f( 1.0,  1.0, -1.0);
	glVertex3f( 1.0,  1.0,  1.0);
	glVertex3f( 1.0, -1.0,  1.0);	


	glEnd();
	
	glBegin(GL_LINES);
	
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glVertex3f( 1.0, -1.0, 1.0);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glVertex3f(-1.0,  1.0, 1.0);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glVertex3f(-1.0, -1.0, -1.0);

	glEnd();
	
}


void computeCinematics( void ) {
	

	/*********** COMPUTE THE ROTATED GRAVITY VECTOR ************/
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glScalef(zoom, zoom, zoom);	
	arcball_rotate();	
	
	GLfloat g[3] = {0.0, -1.0, 0.0};
	
	GLfloat mv[4][4];
	glGetFloatv(GL_MODELVIEW_MATRIX, &mv[0][0]);

	/// multiply by the inverse of the modelview
	GLfloat gravity[3] = {g[0]*mv[0][0] + g[1]*mv[0][1] + g[2]*mv[0][2],
		    			  g[0]*mv[1][0] + g[1]*mv[1][1] + g[2]*mv[1][2],
		    			  g[0]*mv[2][0] + g[1]*mv[2][1] + g[2]*mv[2][2]};
	
	glLoadIdentity();
	
	
	glViewport(0, 0, tex_size, tex_size);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();	
	glLoadIdentity();
	
	gluOrtho2D(0.0, tex_size, 0.0, tex_size);
	
	glEnable(GL_TEXTURE_2D);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	GLenum drawBufs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, drawBufs);

	computeShader.use();

	computeShader.set_uniform("positionTex", 0);
	computeShader.set_uniform("velocityTex", 1);
	computeShader.set_uniform("originalVelocityTex", 2);
	computeShader.set_uniform("time_step", (GLfloat)time_step);
	computeShader.set_uniform("step", (GLint)step);
	computeShader.set_uniform("numParticles", (GLint)numParticles);
	computeShader.set_uniform("gravity", gravity[0], gravity[1], gravity[2]);
	
	glShadeModel(GL_FLAT);	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex2f(0.0, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex2f( tex_size, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex2f( tex_size,  tex_size);
	glTexCoord2f(0.0, 1.0);
	glVertex2f(0.0, tex_size);
	glEnd();
	
	computeShader.use(0);
	
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glViewport(0, 0, winWidth, winHeight);
	glFlush();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDisable(GL_TEXTURE_2D);

	
}


/// Display

void display( void ) {

	glDrawBuffer(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef(zoom, zoom, zoom);
    arcball_rotate();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_position);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex_velocity);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tex_originalVelocity);

	GLfloat *tex_data = new GLfloat[4*numParticles];

	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glReadPixels(0, 0, tex_size, tex_size, GL_RGBA, GL_FLOAT, &tex_data[0]);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	
	glEnable(GL_COLOR_MATERIAL);
	
	glPointSize(point_size);
	glBegin(GL_POINTS);
	for (int i = 0; i < numParticles; ++i) {
		
		if (i%3 == 0)
			glColor3f(1.0, 0.0, 0.0);
		else if (i%3 == 1)
			glColor3f(0.0, 1.0, 0.0);
		else
			glColor3f(0.0, 0.0, 1.0);

		glVertex3f( tex_data[4*i + 0], tex_data[4*i + 1], tex_data[4*i + 2]);
	}
	glEnd();
	
	glColor3f(0.0, 0.0, 0.0);
	drawBoundingBox();
	
	showIH();
	
	delete [] tex_data;
		
	glutSwapBuffers();
}

/// Reshape
/// @arg w, h new window width and height

void reshape( int w, int h ) {
	
	winWidth = w;
	winHeight = h;
    float aspect_ratio = (float) winWidth / (float) winHeight;

    glViewport(0, 0, winWidth, winHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 50.0f, aspect_ratio, 1.0f, 50.0f );
    gluLookAt(
        eye.x, eye.y, eye.z,
        center.x, center.y, center.z,
        up.x, up.y, up.z );
	// set up the arcball using the current projection matrix
	arcball_setzoom( SPHERE_RADIUS, eye, up );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;	

}


/// Keyboard
/// @arg key key pressed
/// @arg x, y window position when key was pressed

void keyboard( unsigned char key, int x, int y ) {

	switch(key) {
	case '+': 
		time_step += 0.005;
		return;		
	case '-': 
		time_step -= 0.005;
		if (time_step < 0)
			time_step = 0;
		return;
	case '[': 
		point_size --;
		if (point_size < 1)
			point_size = 1;
		return;		
	case ']': 
		point_size++;
		return;					
		
	case 'q': case 'Q': case 27: // quit application
		glutDestroyWindow( glutGetWindow() );
		return;
	default: // any other key (just to avoid warnings)
		cerr << "[Error] No key bind for " << key
		     << " in (" << x << ", " << y << ")" << endl;
		return;
	}

	glutPostRedisplay();

}


/// Mouse
/// @arg button which button activate the callback
/// @arg state which state of the mouse
/// @arg x, y window position of the mouse

void mouse( int button, int state, int x, int y ) {

	buttonPressed = button;

	if( state == GLUT_DOWN ) {

		if( button == GLUT_LEFT_BUTTON) {
			int invert_y = (winHeight - y) - 1; // OpenGL viewport coordinates are Cartesian
			arcball_start(x,invert_y);
		}

		if( button == 3 ) // wheel up
			zoom -= 0.1;
		else if( button == 4 ) // wheel down
			zoom += 0.1;

	} else if( state == GLUT_UP ) {

		if( button == GLUT_LEFT_BUTTON || button == GLUT_RIGHT_BUTTON ) {

		}

	}

	glutPostRedisplay();

}

/// Motion
/// @arg x, y window position of the mouse

void motion( int x, int y ) {

	if( buttonPressed == GLUT_LEFT_BUTTON || buttonPressed == GLUT_RIGHT_BUTTON ) {


		if( buttonPressed == GLUT_LEFT_BUTTON) {
			int invert_y = (winHeight - y) - 1;
			arcball_move(x,invert_y);
		}

		glutPostRedisplay();

	}
}


/// Setup position and velocity textures with random initial
void setupTextures ( void ) {
		
	numParticles = tex_size*tex_size;		
	GLfloat *tex_data = new GLfloat[4*numParticles];
	
	srand ( time(NULL) );
	
	for (int i = 0; i < numParticles; ++i) {
		tex_data[4*i + 0] = 0.0;
		tex_data[4*i + 1] = 0.0;
		tex_data[4*i + 2] = 0.0;
		tex_data[4*i + 3] = i/(GLfloat)numParticles;
	}
	
	glGenTextures(1, &tex_position);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_position);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, TEXTURE_TYPE, tex_size, tex_size, 0, GL_RGBA, GL_FLOAT, &tex_data[0]);

	for (int i = 0; i < numParticles; ++i) {		
		tex_data[i*4+0] = (2.0*((GLfloat)rand() / ((GLfloat)RAND_MAX)) - 1.0)*0.25;
		tex_data[i*4+1] = ((GLfloat)rand() / ((GLfloat)RAND_MAX)); 
		tex_data[i*4+2] = (2.0*((GLfloat)rand() / ((GLfloat)RAND_MAX)) - 1.0)*0.25;
		tex_data[i*4+3] = 1.0;
	}

	glGenTextures(1, &tex_velocity);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex_velocity);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, TEXTURE_TYPE, tex_size, tex_size, 0, GL_RGBA, GL_FLOAT, &tex_data[0]);

	glGenTextures(1, &tex_originalVelocity);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tex_originalVelocity);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, TEXTURE_TYPE, tex_size, tex_size, 0, GL_RGBA, GL_FLOAT, &tex_data[0]);	

	delete [] tex_data;

	glGenFramebuffersEXT(1, &fbo);
  	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

	glBindTexture(GL_TEXTURE_2D, tex_position);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex_position, 0);

	glBindTexture(GL_TEXTURE_2D, tex_velocity);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, tex_velocity, 0);

	if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT)
	printf("ERROR - Incomplete FrameBuffer\n");

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

}

void timer(int extra) {	
	
	computeCinematics();
	
	step+=1;
	if (step >= numParticles)
		step = 0;
	glutTimerFunc(10, timer, 0);	
}


/// OpenGL Utility (GLUT) Setup

void setupGL( void ) {

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(titleWin);

#ifdef __GLEW__
	if( GLEW_OK != glewInit() ) {
	  cerr << "glew failed" << endl;
	  exit(1);
	}
#endif

	glClearColor(1., 1., 1., 0.);

	glDisable(GL_DEPTH_TEST);

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutTimerFunc(0,timer,0);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

}

/// Setup GLSL Shaders

bool setupShaders( void ) {

	if( !glsl_support() ) {

		cerr << "[Error] No GLSL support!" << endl;
		return false;

	}

	//~ displayShader.vertex_source(vsFile[0]);
	//~ displayShader.fragment_source(fsFile[0]);
	//~ displayShader.install(true);

	computeShader.vertex_source(vsFile[1]);
	computeShader.fragment_source(fsFile[1]);
	computeShader.install(true);

	return true;

}

/// -------------------------------------   Main   ---------------------------------------

int main( int argc, char** argv ) {

	cout << "[Init] Starting GLUT... " << flush;

	glutInit(&argc, argv);

	cout << "done!\n[Init] Setting OpenGL up... " << flush;

	setupGL();
	setupTextures();

	cout << "done!\n[Init] Setup Shaders:" << endl;

	if( !setupShaders() ) return 1;

	cout << "Finish!" << endl;

	glutMainLoop();

	return 0;

}
