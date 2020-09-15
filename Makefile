# Jared Rand
# 500683609

# Linux (default)
EXE = submarine
LDFLAGS = -lGL -lGLU -lglut

# Windows (cygwin)
ifeq ($(OS), "Windows_NT")
	EXE = submarine.exe
	LDFLAGS = -lopengl32 -lglu32 -lglut32
endif

# OS X
ifeq "$(shell uname)" "Darwin"
	LDFLAGS = -framework OpenGL -framework GLUT
endif

$(EXE): main.c CubeMesh.c QuadMesh.c Vector3D.c Torpedo.c Submarine.c Propeller.c Scenery.c Mountain.c Ground.c Camera.c Util.c
# `-o $@` means "Object file is to be named the function name",
# which in this case is $(EXE)
#
# `$^` evaluates to all the parameters passed in (which in this
# case are all the C files
	gcc -o $@ $^ $(CFLAGS) $(LDFLAGS)

all: $(EXE)

clean:
	rm $(EXE)