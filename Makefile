#
# Makefile -- Introduction to GPU Programming with GLSL
#

#---- Compilation and Linking ----

CXX = g++

# Use GLEW instead of GLee (must use GLEW in MAC)
USE_GLEW = #-D__GLEW__

MAC_FLAG = -I/opt/local/include -D__MAC__

MAC_LINK = -framework GLUT -framework OpenGL -L/opt/local/lib \
	-L"/Library/Frameworks/GLUT.framework" -L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
	-lGL -lGLU -lGLEW -lm -lobjc -lstdc++

INCLUDES = -Iinclude -Ilib/GL -Ilib/glslKernel -Ilib/arcball

# Enable MAC_FLAGS in MAC
FLAGS = -O3 -ffast-math -Wno-deprecated $(INCLUDES) $(USE_GLEW) #$(MAC_FLAG)

# MAC LIBS
#LIBS = $(MAC_LINK)

# Windows LIBS
#LIBS = -lglu32 -lopengl32 -lglut32 

# Linux LIBS
LIBS = -lGL -lGLU -lglut

#---- External Sources and Objects ----

# Enable GLee.c if not using GLEW
EXT_SRCS = lib/arcball/arcball.cpp lib/glslKernel/glslKernel.cc lib/GL/GLee.c
EXT_OBJS = obj/arcball.o obj/glslKernel.o obj/GLee.o

#---- Sources and Objects ----

SHADER_SRC = src/shaders.cc
SHADER_OBJ = obj/shaders.o
SHADER_APP = bin/shaders

PARTICLE_SRC = src/particles.cc
PARTICLE_OBJ = obj/particles.o
PARTICLE_APP = bin/particles

#------------------------------------- Make Commands -----------------------------------------

all:			$(PARTICLE_APP) $(SHADER_APP)

$(PARTICLE_APP):	$(PARTICLE_OBJ) $(EXT_OBJS)
	@echo "Linking..."
	$(CXX) -o $@ $(PARTICLE_OBJ) $(EXT_OBJS) $(LIBDIR) $(LIBS)

$(SHADER_APP):		$(SHADER_OBJ) $(EXT_OBJS)
	@echo "Linking..."
	$(CXX) -o $@ $(SHADER_OBJ) $(EXT_OBJS) $(LIBDIR) $(LIBS)

$(SHADER_OBJ):		$(SHADER_SRC)
	@echo "Compiling ..."
	$(CXX) $(FLAGS) -o $@ -c $<

$(PARTICLE_OBJ):	$(PARTICLE_SRC)
	@echo "Compiling ..."
	$(CXX) $(FLAGS) -o $@ -c $<

obj/arcball.o:		lib/arcball/arcball.cpp
	@echo "Compiling ..."
	$(CXX) $(FLAGS) -o $@ -c $<

obj/glslKernel.o:	lib/glslKernel/glslKernel.cc
	@echo "Compiling ..."
	$(CXX) $(FLAGS) -o $@ -c $<

obj/GLee.o:	lib/GL/GLee.c
	@echo "Compiling ..."
	$(CXX) $(FLAGS) -o $@ -c $<

clean:
	@echo "Cleaning..."
	rm -f $(PARTICLE_APP) $(SHADER_APP) obj/*.o bin/*~ src/*~ include/*~ *~ .depend

depend:		$*.cc
	@echo "Dependency..."
	rm -f .depend
	$(CXX) -M $(FLAGS) $*.cc > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif
