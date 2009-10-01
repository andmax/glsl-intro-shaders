#
# Makefile -- Introduction to GPU Programming with GLSL
#
# Maximo, Andre
#

CXX = g++

MAC_FLAG = -I/opt/local/include -D__MAC__

MAC_LINK = -framework GLUT -framework OpenGL -L/opt/local/lib \
	-L"/Library/Frameworks/GLUT.framework" -L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
	-lGL -lGLU -lGLEW -lm -lobjc -lstdc++

INCLUDES = -I./ -I../libs/

FLAGS = -O3 -ffast-math -Wno-deprecated $(INCLUDES) $(MAC_FLAG)

LIBS = $(MAC_LINK)
#LIBS = -lGL -lGLU -lglut

OBJS = particles.o ../libs/glslKernel.o ../libs/arcball.o #../libs/GLee.o

APP = particles

#------------------------------------- Make Commands -----------------------------------------

$(APP):	$(OBJS)
	@echo "Linking..."
	$(CXX) -o $@ $(OBJS) $(LIBS)

depend:		$*.cc
	@echo "Dependency..."
	rm -f .depend
	$(CXX) -M $(FLAGS) $*.cc > .depend

%.o:		%.cc
	@echo "Compiling ..."
	$(CXX) $(FLAGS) -o $@ -c $<

%.o:		%.cpp
	@echo "Compiling ..."
	$(CXX) $(FLAGS) -o $@ -c $<

%.o:		%.c
	@echo "Compiling ..."
	$(CXX) $(FLAGS) -o $@ -c $<

clean:
	@echo "Cleaning..."
	rm -f *.o *~ \#* .depend $(APP)

ifeq (.depend,$(wildcard .depend))
include .depend
endif
