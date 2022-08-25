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
                    {"OBJ",   ".obj"},
                    {"fbx",   ".fbx"}
            };

    /*
     Exports the model into a file of a certain type
     parameters: arModel - the model
                 arFileType - the assimp-format type of the file
                 arOutputPath - the path of the file, including the name but not including the file ending
     */
    void ModelExporter(CModel &arModel, std::string arFileType, std::string &arOutputPath, bool aWithTexture) {
        Assimp::Importer imp;
        Assimp::Exporter exp;
        arModel.ExportToObj(arOutputPath, aWithTexture);
        const aiScene *scene = imp.ReadFile(std::string(arOutputPath + ".obj"),
                                            aiProcess_CalcTangentSpace |
                                            aiProcess_Triangulate |
                                            aiProcess_JoinIdenticalVertices |
                                            aiProcess_SortByPType);
        if (!scene) {
            std::cout << imp.GetErrorString();
        } else {
            std::cout << "scene loaded" << std::endl;
        }
        arOutputPath += "aaa";
        arOutputPath += fileFormats[arFileType];
        std::cout << arOutputPath << std::endl;
        std::cout << exp.Export(scene, arFileType, arOutputPath) << std::endl;
    }

    void ModelExporter2(CModel &arModel, std::string arFileType, std::string &arOutputPath, bool aWithTexture) {
        Assimp::Importer imp;
        Assimp::Exporter exp;
        const aiScene *scene = imp.ReadFile(std::string(arOutputPath + ".obj"),
                                            aiProcess_CalcTangentSpace |
                                            aiProcess_Triangulate |
                                            aiProcess_JoinIdenticalVertices |
                                            aiProcess_SortByPType);
        if (!scene) {
            std::cout << imp.GetErrorString();
        } else {
            std::cout << "scene loaded" << std::endl;
            //checkProperties(scene);
            //printMeshes(scene->mMeshes, scene->mNumMeshes);
        }
        arOutputPath += "ccc";
        arOutputPath += fileFormats[arFileType];
        std::cout << arOutputPath << std::endl;
        std::cout << exp.Export(scene, arFileType, arOutputPath) << std::endl;
    }

    void ModelExporter3(CModel &arModel, std::string arFileType, std::string arOutputPath, bool aWithTexture) {
        Assimp::Exporter exp;
        aiScene *scene = GenerateScene(arModel, arOutputPath);

        arOutputPath += "_Assimp" + fileFormats[arFileType];
        std::cout << arOutputPath << std::endl;
        if (exp.Export(scene, arFileType, arOutputPath) != AI_SUCCESS) {
            cerr << exp.GetErrorString() << endl;
        }
        delete scene;
    }


    aiScene *GenerateScene(vivid::CModel &model, string filename) {
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

        vector<vector<unsigned char>> textures;

        for (int i = 0; i < scene->mNumMeshes; i++) {
            //export the texture for this mesh into a png
            textures.push_back(meshes[i].GetClm().GetColorTexture());
            vivid::encodePNG(string(filename + meshes[i].GetClm().GetName() + "_texture.png"), textures.back(), 1,
                             textures.back().size() / 4);


            //setting up the mesh structure
            scene->mMeshes[i] = GenerateMesh(meshes[i]);
            scene->mMaterials[i] = GenerateMaterial(meshes[i]);
            scene->mMeshes[i]->mMaterialIndex = i;
            scene->mRootNode->mMeshes[i] = i;
        }
        return scene;
    }

    aiMesh *GenerateMesh(CModelComponent &mesh) {
        //setting up the mesh structure
        aiMesh *OutMesh = new aiMesh();
        OutMesh->mName = mesh.GetLabel();
        OutMesh->mPrimitiveTypes = aiPrimitiveType_TRIANGLE;

        //setting up vertices and uv texture coordinates
        vector<vivid::CPoint> vVertices = mesh.GetPoints();
        OutMesh->mVertices = new aiVector3D[vVertices.size()];
        OutMesh->mNumVertices = vVertices.size();
        for (int j = 0; j < vVertices.size(); j++) {
            OutMesh->mVertices[j] = aiVector3D(vVertices[j].X(), vVertices[j].Y(), vVertices[j].Z());
        }

        //setting up faces
        const vector<vivid::CFace> vTriangles = mesh.GetFaces();
        OutMesh->mFaces = new aiFace[vTriangles.size()];
        OutMesh->mNumFaces = vTriangles.size();

        //setting up UV cords
        OutMesh->mTextureCoords[0] = new aiVector3D[vTriangles.size()];
        OutMesh->mNumUVComponents[0] = vTriangles.size(); //to be checked
        for (auto it = vTriangles.begin(); it != vTriangles.end(); ++it) {
            OutMesh->mTextureCoords[0][it - vTriangles.begin()] = aiVector3D(0, it->GetQuan(), 0);
            aiFace &face = OutMesh->mFaces[it - vTriangles.begin()];
            face.mNumIndices = 3;
            const std::vector<size_t> indexes = it->GetPoints();
            face.mIndices = new unsigned int[3];
            face.mIndices[0] = indexes[0];
            face.mIndices[1] = indexes[1];
            face.mIndices[2] = indexes[2];
        }
        return OutMesh;
    }

    aiMaterial * GenerateMaterial(vivid::CModelComponent& mesh){
        aiMaterial *material = new aiMaterial();

        const aiString *name = new aiString(mesh.GetLabel()+"_mat");
        material->AddProperty(name, AI_MATKEY_NAME);

        const aiColor3D *diffuse_color = new aiColor3D(1,1,1);
        material->AddProperty(diffuse_color, 3,AI_MATKEY_COLOR_DIFFUSE);

        const aiColor3D *specular_color = new aiColor3D(1,1,1);
        material->AddProperty(specular_color, 3,AI_MATKEY_COLOR_SPECULAR);

        const aiColor3D *ambient_color = new aiColor3D(1,1,1);
        material->AddProperty(ambient_color, 3,AI_MATKEY_COLOR_AMBIENT);

        const float *opacity = new float(mesh.GetAlpha());
        material->AddProperty(opacity, 1, AI_MATKEY_OPACITY);

        return material;
    }

}




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