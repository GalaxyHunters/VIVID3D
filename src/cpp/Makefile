CPPFLAGS = -Wall -c -o
CFLAGS = -Wall -fPIC -shared -o
LIBINCLUDE = -L #need to put the directory for include for elad code and boost


all: vivid

python: mesh.so model.so surf.so

Mesh.so: Mesh.cpp
    g++ Mesh.cpp $(LIBINCLUDE) $(CFLAGS) Mesh.so
    
vivid: Model.o Mesh.o Surf.o IndexedFace.o SurfFace.o Point.o Utils.o
    g++ -Wall Main.cpp Model.o Mesh.o Surf.o IndexedFace.o SurfFace.o Point.o Utils.o -o vivid
    
Model.o: Model.cpp Model.h
    g++ Model.cpp $(CPPFLAGS) Model.o 