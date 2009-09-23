#
# Makefile -- Introduction to GPU Programming with GLSL
#
# Maximo, Andre
#

CXX = g++

INCLUDES = -I./ -I../libs/

FLAGS = -O3 -ffast-math -Wno-deprecated $(INCLUDES)

LIBS = -lGL -lGLU -lglut

OBJS = particles.o ../libs/glslKernel.o ../libs/GLee.o ../libs/arcball.o

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

%.o:		%.c
	@echo "Compiling ..."
	$(CXX) $(FLAGS) -o $@ -c $<

clean:
	@echo "Cleaning..."
	rm -f *.o *~ \#* .depend $(APP)

ifeq (.depend,$(wildcard .depend))
include .depend
endif
