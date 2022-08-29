#include "AssimpImportExport.h"



void printMeshes(aiMesh** meshes, int num);
void printMeshProps(aiMesh* mesh);
void printMeshFaces(aiFace face);
void checkProperties(const aiScene* scene);


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
                 arFileType - the assimp-format type of the file
                 arOutputPath - the path of the file, including the name but not including the file ending
     */
    void ModelExporter(CModel &arModel, std::string arFileType, std::string arOutputPath) {
        Assimp::Exporter exp;

        arOutputPath += "_Assimp"; // + fileFormats[arFileType]

        aiScene *scene = GenerateScene(arModel, arOutputPath);

        arOutputPath +=  fileFormats[arFileType];

        std::cout << arOutputPath << std::endl;
        if (exp.Export(scene, arFileType, arOutputPath) != AI_SUCCESS) {
            cerr << exp.GetErrorString() << endl;
        }
        delete scene;
    }


    aiScene *GenerateScene(vivid::CModel &model, std::string arOutputPath) {
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

        scene->mTextures = new aiTexture *[1]; // should be number of texture in models but we dont track that data yet
        scene->mNumTextures = 1; // should be number of texture in models but we dont track that data yet
        scene->mTextures[0] = new aiTexture;
        vector<unsigned char> texture = meshes[0].GetClm().GetColorTexture(); // really bad need a better way to loop over textures
        for (int t = 0; t < scene->mNumTextures; t++) {
            scene->mTextures[t]->mHeight = texture.size()/4;
            scene->mTextures[t]->mWidth = 1;
            strcpy(scene->mTextures[t]->achFormatHint, "rgba8888");
            scene->mTextures[t]->pcData = new aiTexel [texture.size()/4];
            for(int p = 0; p < texture.size()-3; p += 4){ // texture is a vector<unsigned char> with r,g,b,a,r,g,b,a..
                scene->mTextures[t]->pcData[p/4].r = texture[p];
                scene->mTextures[t]->pcData[p/4].g = texture[p+1];
                scene->mTextures[t]->pcData[p/4].b = texture[p+2];
                scene->mTextures[t]->pcData[p/4].a = texture[p+3];
            }
        }
        //export the texture for this mesh into a png - write the png ourself, assymp wont do it
        string textureName = arOutputPath + meshes[0].GetClm().GetName() + "_texture.png";
        vivid::encodePNG(textureName, texture, 1,
                             texture.size() / 4);
        for (int i = 0; i < scene->mNumMeshes; i++) {
            //setting up the mesh structure
            scene->mMeshes[i] = GenerateMesh(&meshes[i]);
            scene->mMaterials[i] = GenerateMaterial(meshes[i], textureName);
            scene->mMeshes[i]->mMaterialIndex = i;
            scene->mRootNode->mMeshes[i] = i;
        }
        return scene;
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
            color flow in vivid works by face. in most 3d formats (and asymp) texture cords are by vertices
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
        double tempAvr = 0;
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

    aiMaterial * GenerateMaterial(vivid::CModelComponent& mesh, string aTextureName){
        aiMaterial *material = new aiMaterial();

        const aiString *name = new aiString(mesh.GetLabel()+"_mat");
        material->AddProperty(name, AI_MATKEY_NAME);

        const aiColor3D *diffuse_color = new aiColor3D(1,1,1);
        material->AddProperty(diffuse_color, 3,AI_MATKEY_COLOR_DIFFUSE);

        const aiColor3D *specular_color = new aiColor3D(0,0,0);
        material->AddProperty(specular_color, 3,AI_MATKEY_COLOR_SPECULAR);

        const aiColor3D *ambient_color = new aiColor3D(1,1,1);
        material->AddProperty(ambient_color, 3,AI_MATKEY_COLOR_AMBIENT);

        const float *opacity = new float(mesh.GetAlpha());
        material->AddProperty(opacity, 1, AI_MATKEY_OPACITY);

        const aiString *texIndex = new aiString(aTextureName.substr(aTextureName.find_last_of("/\\")+1));
        material->AddProperty(texIndex, AI_MATKEY_TEXTURE_DIFFUSE(0));

        return material;
    }

}


//redacted code but maybe not




//returns a list of texture indexes of the mesh - corresponding to the mesh in order - REDACTED
vector<size_t> GetTextureIndexesList(vivid::CModelComponent* mesh) {
    vector<vivid::CFace> faces = mesh->GetFaces();
    vector<size_t> texture_indexes;
    for (int i = 0; i < faces.size(); i++) {
        texture_indexes.push_back(faces[i].GetQuan());
//            std::cout << texture_indexes.back() << std::endl;
    }
    return texture_indexes;
}


void checkProperties(const aiScene* scene)
{
    std::cout << "HasAnimations: " << scene->HasAnimations() << std::endl;
    std::cout << "HasCameras: " << scene->HasCameras() << std::endl;
    std::cout << "HasLights: " << scene->HasLights() << std::endl;
    std::cout << "HasMaterials: " << scene->HasMaterials() << std::endl;
    std::cout << "HasMeshes: " << scene->HasMeshes() << std::endl;
    std::cout << "HasTextures: " << scene->HasTextures() << std::endl;
}

void printMeshes(aiMesh** meshes, int num)
{
    std::cout << "Amount of meshes: " << num << std::endl << std::endl;

    for (int i = 0; i < num; i++)
    {
        printMeshProps(meshes[i]);
        for (int j = 0; j < meshes[i]->mNumFaces; j++)
        {
            printMeshFaces(meshes[i]->mFaces[j]);
        }
    }
}

void printMeshFaces(aiFace face)
{
    for (int i = 0; i < face.mNumIndices; i++)
    {
        std::cout << face.mIndices[i] << " ";
    }
    std::cout << std::endl;
}

void printMeshProps(aiMesh* mesh)
{
    std::cout << "  NumBones: " << mesh->mNumBones << std::endl;
    std::cout << "  NumFaces: " << mesh->mNumFaces << std::endl;
    std::cout << "  HasNormals: " << mesh->HasNormals() << std::endl;
    std::cout << "  HasPositions: " << mesh->HasPositions() << std::endl;
    std::cout << "  HasTangentsAndBitangents: " << mesh->HasTangentsAndBitangents() << std::endl;
    std::cout << std::endl;
}