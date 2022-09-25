#include "AssimpImportExport.h"


namespace vivid {
    // matches assimp file formats to actual file endings
    std::map<std::string, std::string> fileFormats =
            {
                    {"glb2",  ".glb"},
                    {"gltf2", ".gltf"},
                    {"obj",   ".obj"},
                    {"fbx",   ".fbx"}
            };

    /*
     Exports the model into a file of a certain type
     parameters: arModel - the model
                 aFileType - the assimp-format type of the file
                 aOutputPath - the path of the file, including the name but not including the file ending
     */
    int AssimpExporter(CModel &arModel, std::string aFileType, std::string aOutputPath) {
        Assimp::Exporter exp;
        int RET_VALUE;

        aOutputPath += "_Assimp"; // debug

        aiScene *scene = GenerateScene(arModel, aOutputPath);

        aOutputPath +=  fileFormats[aFileType];
        RET_VALUE = exp.Export(scene, aFileType, aOutputPath);
        if (RET_VALUE != AI_SUCCESS) {
            cerr << exp.GetErrorString() << endl;
        }
        delete scene;
        return RET_VALUE;
    }


    aiScene *GenerateScene(vivid::CModel &model, std::string &arOutputPath, bool aImbeddedTexture) {
        //setup scene
        aiScene *scene = new aiScene();
        scene->mRootNode = new aiNode();

        scene->mNumMeshes = model.GetNumMeshes();
        scene->mMeshes = new aiMesh *[scene->mNumMeshes];
        scene->mRootNode->mNumMeshes = scene->mNumMeshes;
        scene->mRootNode->mMeshes = new unsigned int[scene->mRootNode->mNumMeshes];
        vector<vivid::CModelComponent> meshes = model.GetMeshes();

        scene->mMaterials = new aiMaterial *[model.GetNumMeshes()];
        scene->mNumMaterials = model.GetNumMeshes();

        aiTexture** temp_texture_array = new aiTexture * [meshes.size()];
        map<string,string> TextureNameToIndex{};
        string texture_path;
        size_t embedded_texture_count = 0;
        for (int i = 0; i < scene->mNumMeshes; i++) {
            //handle mesh texture
            CColorMap MeshCLM = meshes[i].GetClm();
            if (TextureNameToIndex.find(MeshCLM.GetName()) != TextureNameToIndex.end()) { //check if texture has been written already
                texture_path = TextureNameToIndex[MeshCLM.GetName()];
            } // need to write the texture
            else{
                if(!aImbeddedTexture) texture_path = GenerateTexturePNG(MeshCLM, arOutputPath);
                if(aImbeddedTexture) {
                    temp_texture_array[embedded_texture_count] = GenerateTextureEmbedded(MeshCLM);
                    texture_path = '*' + embedded_texture_count;
                    embedded_texture_count++;
                }
                TextureNameToIndex[MeshCLM.GetName()] = texture_path;
            }

            //setting up the mesh structure
            scene->mMeshes[i] = GenerateMesh(&meshes[i]);
            scene->mMaterials[i] = GenerateMaterial(meshes[i], texture_path);
            scene->mMeshes[i]->mMaterialIndex = i;
            scene->mRootNode->mMeshes[i] = i;
        }
        //setup scene->mTextures
        scene->mTextures = new aiTexture *[embedded_texture_count]; // should be number of texture in models but we dont track that data yet
        scene->mNumTextures = embedded_texture_count; // should be number of texture in models but we dont track that data yet
        for(int i = 0; i < embedded_texture_count; i++){
            scene->mTextures[i] = temp_texture_array[i];
        }
        return scene;
    }

    string GenerateTexturePNG(CColorMap &arMeshTexture, std::string &arOutputPath){
        string textureName = arOutputPath + arMeshTexture.GetName() + "_texture.png";
        vector<unsigned char> ClmBuffer = arMeshTexture.GetColorTexture();
        vivid::encodePNG(textureName, ClmBuffer, 1,
                         ClmBuffer.size() / 4);
        return textureName.substr(textureName.find_last_of("/\\")+1); // remove the path component from the texture name
    }

    aiTexture *GenerateTextureEmbedded(CColorMap &arMeshTexture){
        aiTexture *embedded_texture = new aiTexture;
        vector<unsigned char> ClmBuffer = arMeshTexture.GetColorTexture();
        embedded_texture->mHeight = ClmBuffer.size() / 4;
        embedded_texture->mWidth = 1;
        strcpy(embedded_texture->achFormatHint, "rgba8888");
        embedded_texture->pcData = new aiTexel [ClmBuffer.size() / 4];
        for(int p = 0; p < ClmBuffer.size() - 3; p += 4){ // ClmBuffer is a vector<unsigned char> with r,g,b,a,r,g,b,a..
            embedded_texture->pcData[p / 4].r = ClmBuffer[p];
            embedded_texture->pcData[p / 4].g = ClmBuffer[p + 1];
            embedded_texture->pcData[p / 4].b = ClmBuffer[p + 2];
            embedded_texture->pcData[p / 4].a = ClmBuffer[p + 3];
        }
        return embedded_texture;
    }

    aiMesh *GenerateMesh(CModelComponent * apMesh) {
        //setting up the apMesh structure
        aiMesh *OutMesh = new aiMesh();
        OutMesh->mName = apMesh->GetLabel();
        OutMesh->mPrimitiveTypes = aiPrimitiveType_TRIANGLE;

        //setting up vertices and uv texture coordinates
        vector<vivid::CPoint> vVertices = apMesh->GetPoints();
        OutMesh->mVertices = new aiVector3D[vVertices.size()];
        OutMesh->mNumVertices = vVertices.size();

        //setting up faces
        vector<CFace> vTriangles = apMesh->GetFaces();
        OutMesh->mFaces = new aiFace[vTriangles.size()];
        OutMesh->mNumFaces = vTriangles.size();

        /*
            color flow in vivid works by face. in most 3d formats (and assimp) texture cords are by vertices
            in this segments we get the average color of each vertice.
        */
        OutMesh->mTextureCoords[0] = new aiVector3D [vVertices.size()];
        OutMesh->mNumUVComponents[0] = 2; //to be checked
        //OutMesh->mTextureCoords[0][it - vTriangles.begin()] = aiVector3D(0, it->GetQuan(), 0);


        //assign faces and find avrage UVcoord per point
        vector<set<float>> vetUVcoord(vVertices.size());
        for (auto it = vTriangles.begin(); it != vTriangles.end(); ++it) {
            aiFace &face = OutMesh->mFaces[it - vTriangles.begin()];
            face.mNumIndices = 3;
            const std::vector<size_t> indexes = it->GetPoints();
            face.mIndices = new unsigned int[3];
            face.mIndices[0] = indexes[0];
            face.mIndices[1] = indexes[1];
            face.mIndices[2] = indexes[2];
            // UV coordinates
            vetUVcoord[indexes[0]].insert(it->GetQuan());
            vetUVcoord[indexes[1]].insert(it->GetQuan());
            vetUVcoord[indexes[2]].insert(it->GetQuan());
        }

        //assign points
        double tempAvr;
        for (int j = 0; j < vVertices.size(); j++) {
            OutMesh->mVertices[j] = aiVector3D(vVertices[j].X(), vVertices[j].Y(), vVertices[j].Z());
            if(!vetUVcoord[j].empty()){ //uv coords
                tempAvr = 0;
                for(auto it = vetUVcoord[j].begin(); it != vetUVcoord[j].end(); ++it){
                    tempAvr += *it;
                }
                OutMesh->mTextureCoords[0][j] = aiVector3D(0, 1-tempAvr/vetUVcoord[j].size(), 0);
            }
            else{ // should never happen
                OutMesh->mTextureCoords[0][j] = aiVector3D(0, 0, 0);
            }
        }
        return OutMesh;
    }

    //create new material and define all its properties
    //TODO this should receive our own material class containing any important data (and methods for user interface)
    aiMaterial * GenerateMaterial(vivid::CModelComponent& mesh, string aTextureName){
        aiMaterial *material = new aiMaterial();

        const aiString *name = new aiString(mesh.GetLabel()+"_mat");
        material->AddProperty(name, AI_MATKEY_NAME);

        const int *shading_model = new int(2);
        material->AddProperty(shading_model, 1, AI_MATKEY_SHADING_MODEL);

        const int *illum = new int(0);
        material->AddProperty(illum, 1, "$mat.illum", 4, 0);  // going around assimp system

        const aiColor4D *diffuse_color = new aiColor4D(1,1,1,1);
        material->AddProperty(diffuse_color, 3,AI_MATKEY_COLOR_DIFFUSE);

        const aiColor3D *ambient_color = new aiColor3D(1,1,1);
        material->AddProperty(ambient_color, 3,AI_MATKEY_COLOR_AMBIENT);

        const aiColor3D *specular_color = new aiColor3D(0,0,0);
        material->AddProperty(specular_color, 3,AI_MATKEY_COLOR_SPECULAR);

        const aiColor3D *emissive_color = new aiColor3D(0,0,0);
        material->AddProperty(emissive_color, 3,AI_MATKEY_COLOR_EMISSIVE);

        const aiColor3D *color_transparent = new aiColor3D(1,1,1);
        material->AddProperty(color_transparent, 3,AI_MATKEY_COLOR_TRANSPARENT);

        const float *shininess = new float(95);
        material->AddProperty(shininess, 1, AI_MATKEY_SHININESS);

        const float *opacity = new float(mesh.GetAlpha());
        material->AddProperty(opacity, 1, AI_MATKEY_OPACITY);

        const float *anisotrpy_factor = new float(0);
        material->AddProperty(anisotrpy_factor, 1, AI_MATKEY_ANISOTROPY_FACTOR);

        const float *emissive_intensity = new float(0.3);
        material->AddProperty(emissive_intensity, 1, AI_MATKEY_EMISSIVE_INTENSITY);

        const float *refracti = new float(1);
        material->AddProperty(refracti, 1, AI_MATKEY_REFRACTI);

        const int *twosided = new int(!0);
        material->AddProperty(twosided, 1, AI_MATKEY_TWOSIDED);
        //texture property

        const int *uvwsrc = new int(0);
        material->AddProperty(uvwsrc, 1,AI_MATKEY_UVWSRC_AMBIENT( 0));

        material->AddProperty(uvwsrc, 1,AI_MATKEY_UVWSRC_DIFFUSE( 0));

        material->AddProperty(uvwsrc, 1,AI_MATKEY_UVWSRC_EMISSIVE( 0));

        const aiString *texIndex = new aiString(aTextureName.substr(aTextureName.find_last_of("/\\")+1));
        material->AddProperty(texIndex, AI_MATKEY_TEXTURE_DIFFUSE(0));

        material->AddProperty(texIndex, AI_MATKEY_TEXTURE_AMBIENT(0));

        material->AddProperty(texIndex, AI_MATKEY_TEXTURE_EMISSIVE(0));

        return material;
    }

}
/*  assymp material components
?mat.name  AI_MATKEY_NAME  aiString    """"
$mat.shadingm  AI_MATKEY_SHADING_MODEL   int   2
$mat.illum   type: 4, idx:0 int 2
$clr.ambient  AI_MATKEY_COLOR_AMBIENT  aiColor3D
$clr.diffuse  AI_MATKEY_COLOR_DIFFUSE    aiColor3D
$clr.specular   AI_MATKEY_COLOR_SPECULAR   aiColor3D
$clr.emissive   AI_MATKEY_COLOR_EMISSIVE    aiColor3D
$mat.shininess   AI_MATKEY_SHININESS   float
$mat.opacity     AI_MATKEY_OPACITY    float
$clr.transparent   AI_MATKEY_COLOR_TRANSPARENT    float
$mat.anisotropyFactor    AI_MATKEY_ANISOTROPY_FACTOR   float
$mat.refracti    AI_MATKEY_REFRACTI    float
$tex.file      _AI_MATKEY_TEXTURE_BASE   AI_MATKEY_TEXTURE    texture_type   aiString (path to texture)
$tex.uvwsrc   _AI_MATKEY_UVWSRC_BASE  AI_MATKEY_UVWSRC texture_type int (index to uv channel)
$mat.emissiveIntensity    AI_MATKEY_EMISSIVE_INTENSITY
 */

//redacted code but maybe not


