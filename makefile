MASEXEC=DGRMaster
SLVEXEC=DGRSlave
RLYEXEC=DGRRelay

HEADERS=

GLEW_INC=
GLEW_LIB=-lGLEW

ifeq "$(HOSTNAME)" "ivs.research.mtu.edu"
GLEW_INC=-I/opt/viz/include
GLEW_LIB=-L/opt/viz/lib -lGLEW
endif

# We statically link GLEW on CCSR with our binaries so when we
# transfer the binaries to the IVS tiles, glew works properly. CCSR
# and IVS have different glew versions installed in different places
# with different library filenames.
ifeq "$(HOSTNAME)" "ccsr.ee.mtu.edu"
GLEW_INC=-I/usr/local/glew/1.9.0/include
GLEW_LIB=-L/usr/local/glew/1.9.0/lib -Wl,-Bstatic -lGLEW -Wl,-Bdynamic
endif

ALL_INC=$(GLEW_INC)


MUTSOURCE=DGRMutant.cpp
RLYSOURCE=DGRRelay.cpp

CC=g++

CCFLAGS=-fpermissive -O2 -g -Wall


all: all_linux

#target specific settings
all_linux:  LDFLAGS = $(GLEW_LIB) -lglut -lX11 -lGL -lGLU -lstdc++ -lc -pthread 
all_linux clean_linux: SYSTEM = clean_linux
all_osx: LDFLAGS = $(GLEW_LIB) -framework GLUT -framework OpenGL 
all_win32:



all_linux all_osx all_win32: $(SLVEXEC) $(RLYEXEC) $(MASEXEC)

$(MASEXEC): $(MUTSOURCE) $(HEADERS)
	@echo "=== COMPILING MASTER ==="
	$(CC) -DDGR_MASTER=1 $(FLAGS) $(MUTSOURCE) -o $(MASEXEC) $(ALL_INC) $(LDFLAGS)
	@echo "=== COMPILING SLAVE ==="
	$(CC) $(CCFLAGS) $(MUTSOURCE) -o $(SLVEXEC) $(ALL_INC) $(LDFLAGS)

$(SLVEXEC): $(MASEXEC)

$(RLYEXEC): $(RLYSOURCE)
	@echo "=== COMPILING RELAY ==="
	$(CC) $(FLAGS) $(RLYSOURCE) -o $(RLYEXEC) $(ALL_INC) $(LDFLAGS)

clean:
	rm -f $(SLVEXEC) $(RLYEXEC) $(MASEXEC) *.o
