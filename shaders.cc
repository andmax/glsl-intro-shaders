/**
 *
 *    Introduction to GPU Programming with GLSL
 *
 *  GLSL Shaders Programming Examples
 *
 *  Maximo, Andre -- May, 2009
 *  Marroquim, Ricardo -- 2009
 *
 **/

/// -----------------------------------   Definitions   -------------------------------------

#include "glslKernel.h" // using lcg glsl kernel

#include "materials.h" // color materials constants

#include <cstdio> // standard i/o

extern "C" {
#include <GL/glut.h> // gl-utility library
#include <jpeglib.h>
}

#include "arcball.h"

#include "jpeg.h"

#include <iostream> // i/o stream

#include <math.h> 

#define NUM_SHADERS 7

using std::cout;
using std::cerr;
using std::flush;
using std::endl;

/// ------------------------------------   Variables   --------------------------------------

static const char titleWin[] = "Introduction to GPU Programming with GLSL"; ///< Window title

static int winWidth = 1024, winHeight = 768; ///< Window size

static int buttonPressed = -1; ///< Button state

static int modelId = 1; ///< Current model to be drawn
static const int NUM_MODELS = 3;

static bool showInfo = true, showHelp = false; ///< Show information or help flags

static GLfloat arot[4], aang[4], zoom; ///< Simple trackball for model and light
static int xm, ym; ///< x and y mouse center position

static bool wireframe = false, light = true; ///< Wireframe and Lighting switch
static mat_name mat = transp; ///< Current material type

static GLuint tex_envmap, tex_envmap2, tex_normalmap; ///< Textures

static glslKernel shTier[NUM_SHADERS]; ///< GLSL Kernel Shaders
static bool gsOK = true; ///< Geometry Shader support flag
static bool vsON = false, gsON = false, fsON = false; ///< Vertex, Geometry and Fragment Shader on/off flag
static int currTier = 0; ///< Current shader tier

static bool applyTex = false; ///< Apply texture as normalmap (false) or as texture (true)

// Vertex and Fragment Shader file names
static const char vsFile[NUM_SHADERS][255] = { "helloworld.vert", "simple.vert", "cartoon.vert",
					       "brick.vert", "phong.vert", "envmap.vert",
					       "normalmap.vert" };
static const char gsFile[NUM_SHADERS][255] = { "helloworld.geom", "simple.geom", "------------",
					       "----------", "----------", "----------",
					       "----------" };
static const char fsFile[NUM_SHADERS][255] = { "helloworld.frag", "simple.frag", "cartoon.frag",
					       "brick.frag", "phong.frag", "envmap.frag",
					       "normalmap.frag" };

/// ------------------------------------   TEXTURES   --------------------------------------

static int textureId = 0;
static const int NUM_TEXTURES = 4;
static const char textureFile[NUM_TEXTURES][255] = { "sib09logo.jpg", "earth.jpg", 
								"monet.jpg", "ore.jpg"};

/// ------------------------------------   ARCBALL   --------------------------------------

// scene parameters
const vec eye( 0.0f, 0.0f, -5.0f );
const vec centre( 0.0f, 0.0f, 0.0f );
const vec up( 0.0f, 1.0f, 0.0f );
const float SPHERE_RADIUS = 1.0f;

const float PI = 3.141592654f;

/// ------------------------------------   Functions   --------------------------------------

void setupTexture ( int t );


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

	glColor3ub(0, 0, 0);

	if( light ) glDisable(GL_LIGHTING);

	if( showInfo ) { /// Show informations

		char str[256];

		sprintf(str, "Model: GLUT Teapot" );
		glWrite(-0.9, 0.7, str);

		if( !gsOK ) {
			sprintf(str, "!!No Geometry Shader!!" );
			glWrite(-0.9, 0.8, str);
		}

		sprintf(str, "Tier %d: %s", currTier,
			(currTier==0)?"No Shaders":
			(currTier==1)?"Hello World":
			(currTier==2)?"Simple shader":
			(currTier==3)?"Cartoon Shader":
			(currTier==4)?"Brick Shader":
			(currTier==5)?"Phong Shader":
			(currTier==6)?"Environment Map Shader":
			(currTier==7)?"Normal Map Shader":"");
		glWrite(-0.9, -0.4, str);

		if( currTier > 0 ) {
			sprintf(str, "Shaders: VS(%s) -> GS(%s) -> FS(%s)",
				(vsON)?"on":"off", (gsON)?"on":"off", (fsON)?"on":"off");
			glWrite(-0.9, -0.5, str);
		}

		sprintf(str, "Material: %s",
			(mat == transp)?"color (blue)":
			(mat == pewter)?"pewter":
			(mat == bronze)?"bronze":
			(mat == ruby)?"ruby":"");
		glWrite(-0.9, -0.6, str);

		sprintf(str, "Draw mode: %s - Light (%s)",
			( (wireframe)?"Wireframe":"Solid" ),
			( light ? "on" : "off" ) );
		glWrite(-0.9, -0.7, str);

		sprintf(str, "Resolution: %d x %d", winWidth, winHeight );
		glWrite(-0.9, -0.8, str);


	}

	if( showHelp ) { /// Show help

		glWrite( 0.72,  0.9, "(h|?) close help");
		glWrite(-0.12,  0.6, "(left-button) trackball rotate model");
		glWrite(-0.12,  0.5, "(wheel-button) zoom in/out model");
		glWrite(-0.12,  0.4, "(right-button) trackball rotate light");
		glWrite(-0.12,  0.3, "(l) turn light on/off");
		glWrite(-0.12,  0.2, "(w) switch wireframe/solid draw modes");
		glWrite(-0.12,  0.1, "(s) show/hide information");
		glWrite(-0.12,  0.0, "(c) change to color blue");
		glWrite(-0.12, -0.1, "(b) change to bronze material");
		glWrite(-0.12, -0.2, "(p) change to pewter material");
		glWrite(-0.12, -0.3, "(r) change to ruby material");
		glWrite(-0.12, -0.4, "(0-7) change shader tiers");
		glWrite(-0.12, -0.5, "(v|g|f) on/off vertex/geometry/fragment shader");
		glWrite(-0.12, -0.7, "(q|esc) close application");

	} else if( showInfo ) {

		glWrite(0.72, 0.9, "(h|?) open help");

	}

	if( light ) glEnable(GL_LIGHTING);

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

	if (m == 0) {
		GLUquadric *qobj = gluNewQuadric();
		gluQuadricTexture(qobj,GL_TRUE);
		gluSphere(qobj, 1, 150, 150);
		gluDeleteQuadric(qobj);
	}
	else if (m == 1) {

		if( wireframe ) glutWireTeapot(1.);
		else glutSolidTeapot(1.);
	}
	else if (m == 2) {
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-0.5, -0.5, 0.0);
		glTexCoord2f(1.0, 0.0);
		glVertex3f( 0.5, -0.5, 0.0);
		glTexCoord2f(1.0, 1.0);
		glVertex3f( 0.5,  0.5, 0.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-0.5,  0.5, 0.0);
		glEnd();
	}
}

/// Display

void display( void ) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


	if( showHelp ) {
		showIH();
		glutSwapBuffers();
		return;
	}

	///***************LIGHT ***************//
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(arot[2]+aang[2], 1., 0., 0.);
	glRotatef(arot[3]+aang[3], 0., 1., 0.);

	const GLfloat lp[] = { 0., 8., 64., 1. }; // Light Position
	glLightfv(GL_LIGHT0, GL_POSITION, lp);
	glPopMatrix();
	///************************************//


	glPushMatrix();
	glScalef(zoom, zoom, zoom);
    arcball_rotate();	
	

	glColor3ub(92, 161, 230);

	if( currTier > 0 ) shTier[currTier-1].use();

	if( currTier == 4 && fsON ) {

		shTier[3].set_uniform("BrickColor", 0.8f, 0.1f, 0.1f);
		shTier[3].set_uniform("MortarColor", 0.8f, 0.8f, 0.8f);
		shTier[3].set_uniform("BrickSize", 0.6f, 0.3f);
		shTier[3].set_uniform("BrickPct", 0.9f, 0.8f);

	} else if( currTier == 6 && fsON ) {

		glEnable(GL_TEXTURE_2D);

		shTier[5].set_uniform("envMapTex", 0);
		//shTier[5].set_uniform("envMapTex2", 1);


	} else if( currTier == 7 && fsON ) {

		glEnable(GL_TEXTURE_2D);

		shTier[6].set_uniform("normalMapTex", 2);
		shTier[6].set_uniform("applyTex", applyTex);

	}

	drawModel(modelId);
	
	if( currTier > 0 ) shTier[currTier-1].use(false);

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	
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
        centre.x, centre.y, centre.z,
        up.x, up.y, up.z );
	// set up the arcball using the current projection matrix
	arcball_setzoom( SPHERE_RADIUS, eye, up );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;	

}

/// Change Material

void changeMaterial( void ) {

	glDisable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   Mats[mat]+MA );
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   Mats[mat]+MD );
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  Mats[mat]+MS );
	glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, Mats[mat][SH]);

}

/// Geometry shader dependendancy
/// @arg true if geometry shader is part of the tier

bool gsDepend( void ) {

	return currTier == 1 || currTier == 2;

}

/// Keyboard
/// @arg key key pressed
/// @arg x, y window position when key was pressed

void keyboard( unsigned char key, int x, int y ) {

	switch(key) {
	case '0': // change to no shaders
		currTier = 0;
		vsON = gsON = fsON = false;
		break;
	case '1': // change to hello world
		currTier = 1;
		vsON = gsON = fsON = true;
		shTier[0].vertex_source(vsFile[0]);
		shTier[0].geometry_source(gsFile[0]);
		shTier[0].fragment_source(fsFile[0]);
		if( !gsOK ) {
			gsON = false;
			shTier[0].geometry_source(0);
		}
		shTier[0].install();
		break;
	case '2': // change to simple shader
		currTier = 2;
		vsON = gsON = fsON = true;
		shTier[1].vertex_source(vsFile[1]);
		shTier[1].geometry_source(gsFile[1]);
		shTier[1].fragment_source(fsFile[1]);
		if( !gsOK ) {
			gsON = false;
			shTier[1].geometry_source(0);
		}
		shTier[1].install();
		break;
	case '3': // change to cartoon shader
		currTier = 3;
		vsON = fsON = true; gsON = false;
		shTier[2].vertex_source(vsFile[2]);
		shTier[2].fragment_source(fsFile[2]);
		shTier[2].install();
		break; 
	case '4': // change to brick shader
		currTier = 4;
		vsON = fsON = true; gsON = false;
		shTier[3].vertex_source(vsFile[3]);
		shTier[3].fragment_source(fsFile[3]);
		shTier[3].install();
		break; 
	case '5': // change to phong shader
		currTier = 5;
		vsON = fsON = true; gsON = false;
		shTier[4].vertex_source(vsFile[4]);
		shTier[4].fragment_source(fsFile[4]);
		shTier[4].install();
		break; 
	case '6': // change to phong shader
		currTier = 6;
		vsON = fsON = true; gsON = false;
		shTier[5].vertex_source(vsFile[5]);
		shTier[5].fragment_source(fsFile[5]);
		shTier[5].install();
		break;
	case '7': // change to normal map shader
		currTier = 7;
		vsON = fsON = true; gsON = false;
		shTier[6].vertex_source(vsFile[6]);
		shTier[6].fragment_source(fsFile[6]);
		shTier[6].install();
		break;
	case 'v': case 'V': // vertex shader on/off
		if( currTier == 0 || currTier == 7 ) return;
		if( !fsON ) return;
		vsON = !vsON;
		if( !vsON && gsON && gsDepend() ) {
			gsON = false;
			shTier[currTier-1].geometry_source(0);
		}
		if( vsON ) shTier[currTier-1].vertex_source(vsFile[currTier-1]);
		else shTier[currTier-1].vertex_source(0);
		shTier[currTier-1].install();
		break;
	case 'g': case 'G': // geometry shader on/off
		if( !gsOK || (currTier != 1 && currTier != 2) ) return;
		if( currTier == 7 ) return;
		if( !vsON && !fsON ) return;
		gsON = !gsON;
		if( gsON && !vsON ) {
			vsON = true;
			shTier[currTier-1].vertex_source(vsFile[currTier-1]);
		}
		if( gsON ) shTier[currTier-1].geometry_source(gsFile[currTier-1]);
		else shTier[currTier-1].geometry_source(0);
		shTier[currTier-1].install();
		break;
	case 'f': case 'F': // fragment shader on/off
		if( currTier == 0 ) return;
		if( !vsON && !gsON ) return;
		fsON = !fsON;
		if( fsON ) shTier[currTier-1].fragment_source(fsFile[currTier-1]);
		else shTier[currTier-1].fragment_source(0);
		shTier[currTier-1].install();
		break;
	case 'l': case 'L': // turn light on/off
		light = !light;
		if( light ) glEnable(GL_LIGHTING);
		else glDisable(GL_LIGHTING);
		break;
	case 'w': case 'W': // switch wireframe/solid draw modes
		if( gsDepend() ) return;
		wireframe = !wireframe;
		break;
	case 'c': case 'C': // color material
		mat = transp;
		changeMaterial();
		glEnable(GL_COLOR_MATERIAL);
		break;
	case 'p': case 'P': // pewter material
		mat = pewter;
		changeMaterial();
		break;
	case 'b': case 'B': // brass material
		mat = bronze;
		changeMaterial();
		break;
	case 'r': case 'R': // ruby material
		mat = ruby;
		changeMaterial();
		break;
	case 't': case 'T': // ruby material
		applyTex = !applyTex;
		break;
	case 'h': case 'H': case '?': // show help
		showHelp = !showHelp;
		break;
	case 's': case 'S': // show information
		showInfo = !showInfo;
		break;
	case '[':
		modelId --;
		if (modelId < 0) modelId = NUM_MODELS-1;
		modelId = modelId%NUM_MODELS;
		break;
	case ']':
		modelId = (modelId+1)%NUM_MODELS;
		break;
	case ',':
		textureId --;
		if (textureId < 0) textureId = NUM_TEXTURES-1;
		textureId = textureId%NUM_TEXTURES;
		setupTexture(textureId);
		break;
	case '.':
		textureId = (textureId+1)%NUM_TEXTURES;
		setupTexture(textureId);
		break;		
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

	buttonPressed = button; xm = x; ym = y;

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
			
			if( button == GLUT_RIGHT_BUTTON ) {
				arot[3] += aang[2];
				arot[2] += aang[3];
				if( arot[2] > 360. ) arot[2] -= 360.;
				if( arot[2] < 0. ) arot[2] += 360.;
				if( arot[3] > 360. ) arot[3] -= 360.;
				if( arot[3] < 0. ) arot[3] += 360.;
				aang[2] = 0.;
				aang[3] = 0.;
			}
		}

	}

	glutPostRedisplay();

}

/// Motion
/// @arg x, y window position of the mouse

void motion( int x, int y ) {

	if( buttonPressed == GLUT_LEFT_BUTTON || buttonPressed == GLUT_RIGHT_BUTTON ) {

		
		if( buttonPressed == GLUT_RIGHT_BUTTON ) {			

			aang[3] = (x - xm) * 360 / (GLfloat) winWidth;
			aang[2] = (y - ym) * 180 / (GLfloat) winHeight;
		}


		if( buttonPressed == GLUT_LEFT_BUTTON) {
			int invert_y = (winHeight - y) - 1;
			arcball_move(x,invert_y);
		}

		glutPostRedisplay();

	}

}


void setupTexture ( int t ) {
		
	
	int jpeg_w, jpeg_h, jpeg_c;

	unsigned char* tex_img = jpeg_load(textureFile[t], &jpeg_w, &jpeg_h, &jpeg_c);
	
	glGenTextures(1, &tex_normalmap);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tex_normalmap);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, jpeg_w, jpeg_h, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_img);

	delete [] tex_img;

}



/// OpenGL Utility (GLUT) Setup

void setupGL( void ) {

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(32, 32);
	glutCreateWindow(titleWin);

	glClearColor(1., 1., 1., 0.);

	const GLfloat la[] = { .15, .15, .15, 1. }; // Light Ambient
	const GLfloat ld[] = { .85, .85, .85, 1. }; // Light Diffuse
	const GLfloat ls[] = { .95, .95, .95, 1. }; // Light Specular
	glLightfv(GL_LIGHT0, GL_AMBIENT,  la);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  ld);
	glLightfv(GL_LIGHT0, GL_SPECULAR, ls);
	glEnable(GL_LIGHT0);

	if( light ) glEnable(GL_LIGHTING);

	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);

	mat = transp;
	changeMaterial();
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel( GL_SMOOTH );
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	for (int i=0; i<4; ++i)
		arot[i] = aang[i] = 0.;
	zoom = 1.;

	//char* tex_img = openJpeg("envmap1.jpg");
	int jpeg_w, jpeg_h, jpeg_c;
	unsigned char* tex_img = jpeg_load("envmap.jpg", &jpeg_w, &jpeg_h, &jpeg_c);

	glGenTextures(1, &tex_envmap);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_envmap);
	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_img);

	delete [] tex_img;

	tex_img = jpeg_load("envmap.jpg", &jpeg_w, &jpeg_h, &jpeg_c);

	glGenTextures(1, &tex_envmap2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex_envmap2);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, jpeg_w, jpeg_h, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_img);

	delete [] tex_img;

	setupTexture(textureId);	

}

/// Setup GLSL Shaders

bool setupShaders( void ) {

	if( !glsl_support() ) {

		cerr << "[Error] No GLSL support!" << endl;
		return false;

	}

	if( !geom_shader_support() ) {

		cerr << "[Error] No Geometry Shader support!" << endl;
		gsOK = false;

	}

	cout << "[Shader] Tier 1: Hello World:" << endl;

	shTier[0].vertex_source(vsFile[0]);
	shTier[0].fragment_source(fsFile[0]);
	if( gsOK ) {
		shTier[0].geometry_source(gsFile[0]);
		shTier[0].set_geom_max_output_vertices( 3 );
		shTier[0].set_geom_input_type(GL_TRIANGLES);
		shTier[0].set_geom_output_type(GL_TRIANGLE_STRIP);
	}
	shTier[0].install(true);

	cout << "[Shader] Tier 2: Simple shader:" << endl;

	shTier[1].vertex_source(vsFile[1]);
	shTier[1].fragment_source(fsFile[1]);
	if( gsOK ) {
		shTier[1].geometry_source(gsFile[1]);
		shTier[1].set_geom_max_output_vertices( 3 );
		shTier[1].set_geom_input_type(GL_TRIANGLES);
		shTier[1].set_geom_output_type(GL_TRIANGLE_STRIP);
	}
	shTier[1].install(true);

	cout << "[Shader] Tier 3: Cartoon Shader:" << endl;

	shTier[2].vertex_source(vsFile[2]);
	shTier[2].fragment_source(fsFile[2]);
	shTier[2].install(true);

	cout << "[Shader] Tier 4: Brick Shader:" << endl;

	shTier[3].vertex_source(vsFile[3]);
	shTier[3].fragment_source(fsFile[3]);
	shTier[3].install(true);

	cout << "[Shader] Tier 5: Phong Shader:" << endl;

	shTier[4].vertex_source(vsFile[4]);
	shTier[4].fragment_source(fsFile[4]);
	shTier[4].install(true);

	cout << "[Shader] Tier 6: Environment Map Shader:" << endl;

	shTier[5].vertex_source(vsFile[5]);
	shTier[5].fragment_source(fsFile[5]);
	shTier[5].install(true);

	cout << "[Shader] Tier 7: Normal Map Shader:" << endl;

	shTier[6].vertex_source(vsFile[6]);
	shTier[6].fragment_source(fsFile[6]);
	shTier[6].install(true);

	return true;

}

/// -------------------------------------   Main   ---------------------------------------

int main( int argc, char** argv ) {

	cout << "[Init] Starting GLUT... " << flush;

	glutInit(&argc, argv);

	cout << "done!\n[Init] Setting OpenGL up... " << flush;

	setupGL();

	cout << "done!\n[Init] Setup Shaders:" << endl;

	if( !setupShaders() ) return 1;

	cout << "Finish!" << endl;

	glutMainLoop();

	return 0;

}
