CXX = g++ -g -fPIC
CCX = g++ -g
CPPFLAGS = -Wall -std=c++11
CFLAGS = -Wall -std=c++11 -shared
LIBLINKEGE = ../../external_libraries/huji_rich/librich.a
INCLUDEPATH = -I../../external_libraries/huji_rich/source/3D/GeometryCommon -I../../external_libraries/boost_dump/boost_1_66_0 -I../../external_libraries/include


all: run_vivid


vivid_lib: set_libs_env Mesh.so Model.so Surf.so

Mesh.so: Mesh.cpp
	$(CXX) Mesh.cpp -L../../external_libraries/huji_rich -llibrich.a $(INCLUDEPATH) $(CFLAGS) -o Mesh.so

Model.so: Model.cpp
	$(CXX) Model.cpp $(LIBLINKEGE) $(INCLUDEPATH) $(CFLAGS) -o Model.so

Surf.so: Surf.cpp
	$(CXX) Surf.cpp $(LIBLINKEGE) $(INCLUDEPATH) $(CFLAGS) -o Surf.so



run_vivid: set_libs_env Surf.o Mesh.o Model.o
	$(CCX) Main.cpp Model.o Mesh.o Surf.o $(LIBLINKEGE) $(INCLUDEPATH) $(CPPFLAGS) -o vivid

Model.o: Model.cpp Model.h
	$(CCX) Model.cpp $(INCLUDEPATH) $(CPPFLAGS) -c -o Model.o

Mesh.o: Mesh.cpp Mesh.h Point.h IndexedFace.h
	$(CCX) Mesh.cpp $(INCLUDEPATH) $(CPPFLAGS) -c -o Mesh.o

Surf.o: Surf.cpp Surf.h Point.h SurfFace.h Utils.h
	$(CCX) Surf.cpp $(INCLUDEPATH) $(CPPFLAGS) -c -o Surf.o



set_libs_env: ../../external_libraries/include/H5Cpp.h ../../external_libraries/boost_dump/boost_1_66_0/boost/container/static_vector.hpp

../../external_libraries/include/H5Cpp.h: ../../external_libraries/hdf5_dump/hdf5-1.10.3/c++/src/H5Cpp.h
	cd ../../external_libraries/hdf5_dump/hdf5-1.10.3 && \
	./configure --enable-cxx --prefix=`cd ../.. && pwd`
	cd ../../external_libraries/hdf5_dump/hdf5-1.10.3 && make
	cd ../../external_libraries/hdf5_dump/hdf5-1.10.3 && make install

../../external_libraries/hdf5_dump/hdf5-1.10.3/c++/src/H5Cpp.h: | ../../external_libraries/hdf5_dump/hdf5-1.10.3.tar.gz
	cd ../../external_libraries/hdf5_dump/ && tar xf ./hdf5-1.10.3.tar.gz

../../external_libraries/boost_dump/boost_1_66_0/boost/container/static_vector.hpp: | ../../external_libraries/boost_dump/boost_1_66_0.tar.bz2
	cd ../../external_libraries/boost_dump/ && tar xf ./boost_1_66_0.tar.bz2



../../external_libraries/hdf5_dump/hdf5-1.10.3.tar.gz:
	mkdir -p ../../external_libraries/hdf5_dump
	cd ../../external_libraries/hdf5_dump && \
	wget https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.10/hdf5-1.10.3/src/hdf5-1.10.3.tar.gz

../../external_libraries/boost_dump/boost_1_66_0.tar.bz2:
	mkdir -p ../../external_libraries/boost_dump
	cd ../../external_libraries/boost_dump && \
	wget 'http://sourceforge.net/projects/boost/files/boost/1.66.0/boost_1_66_0.tar.bz2/download'
	cd ../../external_libraries/boost_dump && mv download boost_1_66_0.tar.bz2

clean:
	rm -f *.o all
