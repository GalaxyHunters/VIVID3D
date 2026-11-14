# Writing models to files and blobs (assimp, OBJ) and colormap textures (lodepng).
target_sources(vivid PRIVATE
        AssimpImportExport.cpp
        ObjImportExport.cpp
        BlobData.cpp
        ColorMapToPng.cpp)
target_include_directories(vivid PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
