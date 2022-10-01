#include "AssimpImportExport.h"

namespace vivid {

    constexpr float MAX_ROUGHNESS = 1000;

    namespace AssimpExport {
        // matches assimp file formats to actual file endings
        std::map<std::string, std::string> fileFormats =
                {
                        {"glb2",    ".glb"},
                        {"gltf2",   ".gltf"},
                        {"obj",     ".obj"},
                        {"fbx",     ".fbx"},
                        {"ply",     ".ply"},
                        {"gltf",    ".gltf"},
                        {"3ds",     ".3ds"},
                        {"stl",     ".stl"},
                        {"stlb",    ".stl"},
                        {"collada", ".dae"}
                };

        map<string, string> TextureNameToIndex = {};
        string OutputPath = "";

        int AssimpExporter(CModel &arModel, const std::string &arFileType, std::string aOutputPath) {
            Assimp::Exporter exp;
            int RET_VALUE;

            OutputPath = aOutputPath;
            aiScene *scene = GenerateScene(arModel);

            aOutputPath += fileFormats[arFileType];
            RET_VALUE = exp.Export(scene, arFileType, aOutputPath);
            if (RET_VALUE != AI_SUCCESS) {
                cerr << exp.GetErrorString() << endl;
            }

            TextureNameToIndex.clear();
            delete scene;
            return RET_VALUE;
        }

        int AnimationExporter(CAnimation &arAnimation, const std::string &arFileType, std::string aOutputPath) {
            int RET_VALUE;
            Assimp::Exporter exp;

            OutputPath = aOutputPath;

            aiScene *scene = GenerateAnimationScene(arAnimation);


            aOutputPath += fileFormats[arFileType];
            RET_VALUE = exp.Export(scene, arFileType, aOutputPath);
            if (RET_VALUE != AI_SUCCESS) {
                cerr << exp.GetErrorString() << endl;
            }

            TextureNameToIndex.clear();
            delete scene;
            return RET_VALUE;
        }

        aiScene *GenerateScene(const vivid::CModel &model) {
            //setup scene
            aiScene *scene = new aiScene();
            scene->mFlags = AI_SCENE_FLAGS_NON_VERBOSE_FORMAT + AI_SCENE_FLAGS_ALLOW_SHARED;
            scene->mRootNode = new aiNode();
            scene->mRootNode->mName = aiString("root_node");

            scene->mNumMeshes = model.GetNumMeshes();
            scene->mMeshes = new aiMesh *[scene->mNumMeshes];

            scene->mMaterials = new aiMaterial *[scene->mNumMeshes];
            scene->mNumMaterials = scene->mNumMeshes;

            scene->mRootNode->mNumChildren = 1;
            scene->mRootNode->mChildren = new aiNode *[1];
            scene->mRootNode->mChildren[0] = GenerateNode("basic_node", 0, scene->mNumMeshes);
            scene->mRootNode->mChildren[0]->mParent = scene->mRootNode;


            vector<vivid::CModelComponent> meshes = model.GetMeshes();

            string texture_path;
            for (int i = 0; i < scene->mNumMeshes; i++) {
                //handle mesh texture
                texture_path = AddTexture(meshes[i].GetColorMap());
                //setting up the mesh structure
                scene->mMeshes[i] = GenerateMesh(&meshes[i]);
                scene->mMaterials[i] = GenerateMaterial(meshes[i].GetMaterial(), texture_path, i);
                scene->mMeshes[i]->mMaterialIndex = i;
            }
            return scene;
        }

        aiScene *GenerateAnimationScene(CAnimation &arAnimation) {
            //setup scene
            aiScene *scene = new aiScene();
            scene->mRootNode = new aiNode();
            scene->mRootNode->mName = aiString("root_node");

            vector<vivid::CModel> models = arAnimation.GetModels();

            scene->mNumMeshes = arAnimation.GetNumMeshes();
            scene->mMeshes = new aiMesh *[scene->mNumMeshes];

            scene->mMaterials = new aiMaterial *[scene->mNumMeshes];
            scene->mNumMaterials = scene->mNumMeshes;

            scene->mRootNode->mNumChildren = 1;
            scene->mRootNode->mChildren = new aiNode *[1];
            aiNode *center_node = GenerateNode("center_node", 0, 0);
            scene->mRootNode->mChildren[0] = center_node;
            scene->mRootNode->mChildren[0]->mParent = scene->mRootNode;
            center_node->mNumChildren = models.size();
            center_node->mChildren = new aiNode *[models.size()];

            scene->mNumAnimations = 1;//models.size();
            scene->mAnimations = new aiAnimation *[1]; //models.size()


            CStopMotionAnimation *SMAnimation = dynamic_cast<CStopMotionAnimation *>(&arAnimation);
            if (!SMAnimation) {
                scene->mAnimations[0] = GenerateAnimation(arAnimation);
            } else {
                scene->mAnimations[0] = GenerateStopMotionAnimation(*SMAnimation);
            }

            vector<vivid::CModelComponent> meshes;
            string texture_path;
            size_t mesh_counter = 0;
            for (int model_index = 0; model_index != models.size(); model_index++) {
                meshes = models[model_index].GetMeshes();
                center_node->mChildren[model_index] = GenerateNode("anim_node_" + to_string(model_index), mesh_counter,
                                                                   mesh_counter + meshes.size());
                center_node->mChildren[model_index]->mParent = center_node;
                for (int mesh_index = 0; mesh_index < meshes.size(); mesh_index++) {
                    //handle mesh texture
                    texture_path = AddTexture(meshes[mesh_index].GetColorMap());
                    //setting up the mesh structure
                    scene->mMeshes[mesh_counter] = GenerateMesh(&meshes[mesh_index]);
                    scene->mMaterials[mesh_counter] = GenerateMaterial(meshes[mesh_index].GetMaterial(), texture_path, mesh_counter);
                    scene->mMeshes[mesh_counter]->mMaterialIndex = mesh_counter;
                    mesh_counter++;
                }
            }
            return scene;
        }

        aiAnimation *GenerateAnimation(CAnimation &arAnimation) {
            aiAnimation *anim = new aiAnimation;
            anim->mNumChannels = arAnimation.GetModels().size();
            anim->mChannels = new aiNodeAnim *[anim->mNumChannels];
            anim->mDuration = arAnimation.GetDuration();
            anim->mTicksPerSecond = arAnimation.GetTicksPerSecond();
            anim->mName = aiString("animation");
            for (int i = 0; i != anim->mNumChannels; i++) {
                anim->mChannels[i] = GenerateAnimationChannel(arAnimation, i);
            }
            return anim;
        }

        aiAnimation *GenerateStopMotionAnimation(CStopMotionAnimation &arSMAnimation) {
            aiAnimation *anim = new aiAnimation;
            anim->mNumChannels = arSMAnimation.GetModels().size();
            anim->mChannels = new aiNodeAnim *[anim->mNumChannels];
            int ticks_per_frame = int(arSMAnimation.GetTicksPerSecond() * arSMAnimation.GetSecondsPerFrame());
            anim->mDuration = anim->mNumChannels * ticks_per_frame;
            anim->mTicksPerSecond = arSMAnimation.GetTicksPerSecond();
            anim->mName = aiString("animation");
            for (int i = 0; i != anim->mNumChannels; i++) {
                anim->mChannels[i] = new aiNodeAnim;
                anim->mChannels[i]->mNodeName = aiString("anim_node_" + to_string(i));
//            anim->mChannels[i]->mPreState = aiAnimBehaviour_CONSTANT;
//            anim->mChannels[i]->mPostState = aiAnimBehaviour_CONSTANT;
                //Move
                CPoint animation_value = arSMAnimation.GetMoveAnim(i);
                MoveAnimation(anim->mChannels[i], (i + 1) * ticks_per_frame, &animation_value, i * ticks_per_frame);
                //Rotate
                animation_value = arSMAnimation.GetRotateAnim(i);
                RotateAnimation(anim->mChannels[i], (i + 1) * ticks_per_frame, &animation_value, i * ticks_per_frame);
                //Scale
                animation_value = arSMAnimation.GetScaleAnim(i);
                StopMotionScaleAnimation(anim->mChannels[i], (i + 1) * ticks_per_frame, &animation_value,
                                         i * ticks_per_frame);
            }
            return anim;
        }

        void StopMotionScaleAnimation(aiNodeAnim *arAnim, coord_t aDuration, CPoint *arAnimValue, coord_t aStartTime) {
            if (*arAnimValue != CPoint()) {
                if (aStartTime == 0) { //make the frame pop out and in when needed
                    arAnim->mNumScalingKeys = int(aDuration - aStartTime) + 3;
                    arAnim->mScalingKeys = new aiVectorKey[int(aDuration - aStartTime) + 3];
                    arAnim->mScalingKeys[0] = aiVectorKey(aStartTime, aiVector3D(1, 1, 1));
                    arAnim->mScalingKeys[1] = aiVectorKey(aDuration - 1, aiVector3D(1, 1, 1));
                    arAnim->mScalingKeys[int(aDuration - aStartTime) + 2] = aiVectorKey(aDuration,
                                                                                        aiVector3D(1e-10, 1e-10,
                                                                                                   1e-10));
                } else {
                    arAnim->mNumScalingKeys = int(aDuration - aStartTime) + 5;
                    arAnim->mScalingKeys = new aiVectorKey[int(aDuration - aStartTime) + 5];
                    arAnim->mScalingKeys[0] = aiVectorKey(0, aiVector3D(1e-10, 1e-10, 1e-10));
                    arAnim->mScalingKeys[1] = aiVectorKey(aStartTime - 1, aiVector3D(1e-10, 1e-10, 1e-10));
                    arAnim->mScalingKeys[2] = aiVectorKey(aStartTime, aiVector3D(1, 1, 1));
                    arAnim->mScalingKeys[int(aDuration - aStartTime) + 3] = aiVectorKey(aStartTime,
                                                                                        aiVector3D(1, 1, 1));
                    arAnim->mScalingKeys[int(aDuration - aStartTime) + 4] = aiVectorKey(aDuration,
                                                                                        aiVector3D(1e-10, 1e-10,
                                                                                                   1e-10));
                }
                //do scale anim in the time the user askes for it
                *arAnimValue = *arAnimValue / (aDuration - aStartTime);
                aiVector3D temp_vec;
                for (int i = 0; i != (aDuration - aStartTime); i++) {
                    temp_vec = aiVector3D((arAnimValue->X() * i) + 1, (arAnimValue->Y() * i) + 1,
                                          (arAnimValue->Z() * i) + 1);
                    arAnim->mScalingKeys[i + 2] = aiVectorKey(i + aStartTime, temp_vec);
                }
            } else { //no scale animation from user, only do stop motion
                if (aStartTime == 0) {
                    arAnim->mNumScalingKeys = 3;
                    arAnim->mScalingKeys = new aiVectorKey[3];
                    arAnim->mScalingKeys[0] = aiVectorKey(aStartTime, aiVector3D(1, 1, 1));
                    arAnim->mScalingKeys[1] = aiVectorKey(aDuration - 1, aiVector3D(1, 1, 1));
                    arAnim->mScalingKeys[2] = aiVectorKey(aDuration, aiVector3D(1e-10, 1e-10, 1e-10));
                } else {
                    arAnim->mNumScalingKeys = 5;
                    arAnim->mScalingKeys = new aiVectorKey[5];
                    arAnim->mScalingKeys[0] = aiVectorKey(0, aiVector3D(1e-10, 1e-10, 1e-10));
                    arAnim->mScalingKeys[1] = aiVectorKey(aStartTime - 1, aiVector3D(1e-10, 1e-10, 1e-10));
                    arAnim->mScalingKeys[2] = aiVectorKey(aStartTime, aiVector3D(1, 1, 1));
                    arAnim->mScalingKeys[3] = aiVectorKey(aDuration - 1, aiVector3D(1, 1, 1));
                    arAnim->mScalingKeys[4] = aiVectorKey(aDuration, aiVector3D(1e-10, 1e-10, 1e-10));
                }
            }
        }


        void ScaleAnimation(aiNodeAnim *arAnim, coord_t aDuration, CPoint *arAnimValue, coord_t aStartTime) {
            if (*arAnimValue != CPoint()) {
                arAnim->mNumScalingKeys = int(aDuration - aStartTime);
                arAnim->mScalingKeys = new aiVectorKey[int(aDuration - aStartTime)];
                *arAnimValue = *arAnimValue / (aDuration - aStartTime);
                aiVector3D temp_vec;
                for (int i = 0; i != (aDuration - aStartTime); i++) {
                    temp_vec = aiVector3D((arAnimValue->X() * i) + 1, (arAnimValue->Y() * i) + 1,
                                          (arAnimValue->Z() * i) + 1);
                    arAnim->mScalingKeys[i] = aiVectorKey(i + aStartTime, temp_vec);
                }
            } else {
                arAnim->mNumScalingKeys = 1;
                arAnim->mScalingKeys = new aiVectorKey[1];
                arAnim->mScalingKeys[0] = aiVectorKey(aStartTime, aiVector3D(1, 1, 1));
            }
        }

        void MoveAnimation(aiNodeAnim *arAnim, coord_t aDuration, CPoint *arAnimValue, coord_t aStartTime) {
            if (*arAnimValue != CPoint()) {
                arAnim->mNumPositionKeys = int(aDuration - aStartTime);
                arAnim->mPositionKeys = new aiVectorKey[int(aDuration - aStartTime)];
                *arAnimValue = (*arAnimValue) / (aDuration - aStartTime);
                aiVector3D temp_vec;
                for (int i = 0; i != aDuration - aStartTime; i++) {
                    temp_vec = aiVector3D((arAnimValue->X() * i), (arAnimValue->Y() * i), (arAnimValue->Z() * i));
                    arAnim->mPositionKeys[i] = aiVectorKey(i + aStartTime, temp_vec);
                }
            } else {
                arAnim->mNumPositionKeys = 1;
                arAnim->mPositionKeys = new aiVectorKey[1];
                arAnim->mPositionKeys[0] = aiVectorKey(aStartTime, aiVector3D(0, 0, 0));
            }
        }

        void RotateAnimation(aiNodeAnim *arAnim, coord_t aDuration, CPoint *arAnimValue, coord_t aStartTime) {
            if (*arAnimValue != CPoint()) {
                arAnim->mNumRotationKeys = int(aDuration - aStartTime);
                arAnim->mRotationKeys = new aiQuatKey[int(aDuration - aStartTime)];
                *arAnimValue = *arAnimValue / (aDuration - aStartTime);
                aiQuaternion temp_quat;
                for (int i = 0; i != (aDuration - aStartTime); i++) {
                    temp_quat = aiQuaternion((arAnimValue->Y() * i), (arAnimValue->Z() * i), (arAnimValue->X() * i));
                    arAnim->mRotationKeys[i] = aiQuatKey(i + aStartTime, temp_quat);
                }
            } else {
                arAnim->mNumRotationKeys = 1;
                arAnim->mRotationKeys = new aiQuatKey[1];
                arAnim->mRotationKeys[0] = aiQuatKey(aStartTime, aiQuaternion(1, 0, 0, 0));
            }
        }

        aiNodeAnim *GenerateAnimationChannel(CAnimation &arAnimation, size_t aIndex) {
            aiNodeAnim *anim_channel = new aiNodeAnim;
            anim_channel->mNodeName = aiString("anim_node_" + to_string(aIndex));
            //Move
            CPoint &animation_value = arAnimation.GetMoveAnim(aIndex);
            MoveAnimation(anim_channel, arAnimation.GetDuration(), &animation_value);
            //Rotate
            animation_value = arAnimation.GetRotateAnim(aIndex);
            RotateAnimation(anim_channel, arAnimation.GetDuration(), &animation_value);
            //Scale
            animation_value = arAnimation.GetScaleAnim(aIndex);
            ScaleAnimation(anim_channel, arAnimation.GetDuration(), &animation_value);

            return anim_channel;
        }

        aiNode *GenerateNode(string aNodeName, size_t aMeshIndexStart, size_t aMeshIndexEnd) {
            aiNode *node = new aiNode();
            node->mName = aiString(aNodeName);
            node->mNumMeshes = aMeshIndexEnd - aMeshIndexStart;
            node->mMeshes = new unsigned int[node->mNumMeshes];
            size_t index = 0;
            for (size_t MeshIndex = aMeshIndexStart; MeshIndex != aMeshIndexEnd; MeshIndex++) {
                node->mMeshes[index] = MeshIndex;
            }
            return node;
        }

        string AddTexture(CColorMap arMeshCLM) {
            string texture_path;
            if (TextureNameToIndex.find(arMeshCLM.GetName()) !=
                TextureNameToIndex.end()) { //check if texture has been written already
                texture_path = TextureNameToIndex[arMeshCLM.GetName()];
            } // need to write the texture
            else {
                texture_path = GenerateTexturePNG(arMeshCLM, OutputPath);
                TextureNameToIndex[arMeshCLM.GetName()] = texture_path;
            }
            return texture_path;
        }
        
        string GenerateTexturePNG(CColorMap &arMeshTexture, std::string &arOutputPath) {
            string textureName = arOutputPath + arMeshTexture.GetName() + "_texture.png";
            vector<unsigned char> ClmBuffer = arMeshTexture.GetColorTexture();
            vivid::encodePNG(textureName, ClmBuffer, 1,
                             ClmBuffer.size() / 4);
            return textureName.substr(
                    textureName.find_last_of("/\\") + 1); // remove the path component from the texture name
        }

        aiTexture *GenerateTextureEmbedded(CColorMap &arMeshTexture) { //for now this will be deprecated
            aiTexture *embedded_texture = new aiTexture;
            vector<unsigned char> ClmBuffer = arMeshTexture.GetColorTexture();
            embedded_texture->mHeight = ClmBuffer.size() / 4;
            embedded_texture->mWidth = 1;
            strcpy(embedded_texture->achFormatHint, "rgba8888");
            embedded_texture->pcData = new aiTexel[ClmBuffer.size() / 4];
            for (int p = 0;
                 p < ClmBuffer.size() - 3; p += 4) { // ClmBuffer is a vector<unsigned char> with r,g,b,a,r,g,b,a..
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
            switch (apMesh->GetObjType()){
                case TRIANGLES:
                    OutMesh->mPrimitiveTypes = aiPrimitiveType_TRIANGLE;
                    break;
                case LINES:
                    OutMesh->mPrimitiveTypes = aiPrimitiveType_LINE;
                    break;
                case POINTS:
                    OutMesh->mPrimitiveTypes = aiPrimitiveType_POINT;
                    break;
                default:
                    OutMesh->mPrimitiveTypes = aiPrimitiveType_POLYGON;
            }

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
                const std::vector<size_t> indexes = it->GetPoints();
                face.mNumIndices = indexes.size();
                face.mIndices = new unsigned int[indexes.size()];
                for (int i = 0; i < indexes.size(); i++) {
                    face.mIndices[i] = indexes[i];
                    vetUVcoord[indexes[i]].insert(it->GetUVcoord());
                }
            }

            //assign points
            double tempAvr, val;
            for (int j = 0; j < vVertices.size(); j++) {
                OutMesh->mVertices[j] = aiVector3D(vVertices[j].X(), vVertices[j].Y(), vVertices[j].Z());
                if (!vetUVcoord[j].empty()) { //uv coords
                    tempAvr = 0;
                    for (auto it = vetUVcoord[j].begin(); it != vetUVcoord[j].end(); ++it) {
                        tempAvr += *it;
                    }
                    val = max(0.01, min(1 - tempAvr / vetUVcoord[j].size(), 1.0));
                    OutMesh->mTextureCoords[0][j] = aiVector3D(0, val, 0);
                } else { // should never happen
                    OutMesh->mTextureCoords[0][j] = aiVector3D(0, 0, 0);
                }
            }
            return OutMesh;
        }

        aiMaterial *GenerateMaterial(const vivid::CMaterial& arMaterial, const string& aTextureName, size_t mat_index) {
            aiMaterial *material = new aiMaterial();

            const aiString *name = new aiString(arMaterial.GetLabel() + "_mat"+ to_string(mat_index));
            material->AddProperty(name, AI_MATKEY_NAME);

            const array<float, 3> mat_emissive_color = ToNormalRGB(arMaterial.GetEmissionColor());
            const aiColor3D *emissive_color = new aiColor3D(mat_emissive_color[0], mat_emissive_color[1], mat_emissive_color[2]);
            material->AddProperty(emissive_color, 3,AI_MATKEY_COLOR_EMISSIVE);
        
            const int *shading_model = new int(2);
            material->AddProperty(shading_model, 1, AI_MATKEY_SHADING_MODEL);

            const int *illum = new int(0);
            material->AddProperty(illum, 1, "$mat.illum", 4, 0);  // going around assimp system

            const aiColor4D *diffuse_color = new aiColor4D(1, 1, 1, 1);
            material->AddProperty(diffuse_color, 3, AI_MATKEY_COLOR_DIFFUSE);

            const aiColor3D *ambient_color = new aiColor3D(1, 1, 1);
            material->AddProperty(ambient_color, 3, AI_MATKEY_COLOR_AMBIENT);

            const aiColor3D *specular_color = new aiColor3D(0, 0, 0);
            material->AddProperty(specular_color, 3, AI_MATKEY_COLOR_SPECULAR);


            const aiColor3D *color_transparent = new aiColor3D(1, 1, 1);
            material->AddProperty(color_transparent, 3, AI_MATKEY_COLOR_TRANSPARENT);

            
            const float *shininess = new float(arMaterial.GetShininess() * MAX_ROUGHNESS);
            material->AddProperty(shininess, 1, AI_MATKEY_SHININESS);

            const float *opacity = new float(arMaterial.GetOpacity());
            material->AddProperty(opacity, 1, AI_MATKEY_OPACITY);

            const float *anisotrpy_factor = new float(0);
            material->AddProperty(anisotrpy_factor, 1, AI_MATKEY_ANISOTROPY_FACTOR);
            
            const float *emissive_intensity = new float(arMaterial.GetEmissionStrength());
            material->AddProperty(emissive_intensity, 1, AI_MATKEY_EMISSIVE_INTENSITY);

            const float *refracti = new float(1);
            material->AddProperty(refracti, 1, AI_MATKEY_REFRACTI);

            const int *twosided = new int(!0);
            material->AddProperty(twosided, 1, AI_MATKEY_TWOSIDED);
            //texture property

            const int *uvwsrc = new int(0);
            material->AddProperty(uvwsrc, 1, AI_MATKEY_UVWSRC_AMBIENT(0));

            material->AddProperty(uvwsrc, 1, AI_MATKEY_UVWSRC_DIFFUSE(0));

            material->AddProperty(uvwsrc, 1, AI_MATKEY_UVWSRC_EMISSIVE(0));

            const aiString *texIndex = new aiString(aTextureName.substr(aTextureName.find_last_of("/\\") + 1));
            material->AddProperty(texIndex, AI_MATKEY_TEXTURE_DIFFUSE(0));

            material->AddProperty(texIndex, AI_MATKEY_TEXTURE_AMBIENT(0));

            material->AddProperty(texIndex, AI_MATKEY_TEXTURE_EMISSIVE(0));

            return material;
        }
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


