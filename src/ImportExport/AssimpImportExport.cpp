#include "AssimpImportExport.h"
//#include "PostProcessing/MakeVerboseFormat.h"

namespace vivid {
    namespace {
        constexpr float MAX_ROUGHNESS = 1000;
        // matches assimp file formats to actual file endings
        const std::map<std::string, std::string> FILE_FORMATS =
            {
                    {"glb",     ".glb"},    // Works
                    {"gltf",    ".gltf"},   // Works
                    {"obj",     ".obj"},    // Works
                    {"fbx",     ".fbx"},    // Doesn't work well
                    {"ply",     ".ply"},    // Untested
                    {"3ds",     ".3ds"},    // Untested
                    {"stl",     ".stl"},    // Untested
                    {"stlb",    ".stl"},    // Untested
                    {"collada", ".dae"}     // Untested
            };

        std::string RenameGLTF_GLB(const std::string& arFileFormat) {
            if (arFileFormat == "glb" || arFileFormat == "gltf") {
                return arFileFormat + "2";
            }
            return arFileFormat;
        }


        map<string, string> TextureNameToIndex = {};
        vector<aiTexture*> EmbeddedTextures = {};
        bool EmbeddedTexture = false;
        string OutputPath = "";

        void StopMotionScaleAnimation(aiNodeAnim *arAnim, coord_t aDuration, CPoint *arAnimValue, coord_t aStartTime = 0) {
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


        void ScaleAnimation(aiNodeAnim *arAnim, coord_t aDuration, CPoint *arAnimValue, coord_t aStartTime = 0) {
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

        void MoveAnimation(aiNodeAnim *arAnim, coord_t aDuration, CPoint *arAnimValue, coord_t aStartTime = 0) {
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

        void RotateAnimation(aiNodeAnim *arAnim, coord_t aDuration, CPoint *arAnimValue, coord_t aStartTime = 0) {
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
            auto *anim_channel = new aiNodeAnim;
            anim_channel->mNodeName = aiString("anim_node_" + to_string(aIndex));
            //Move
            CPoint &animation_value = arAnimation.GetFrames()[aIndex].mMoveAnimation;
            MoveAnimation(anim_channel, arAnimation.GetDuration(), &animation_value);
            //Rotate
            animation_value = arAnimation.GetFrames()[aIndex].mRotateAnimation;
            RotateAnimation(anim_channel, arAnimation.GetDuration(), &animation_value);
            //Scale
            animation_value = arAnimation.GetFrames()[aIndex].mScaleAnimation;
            ScaleAnimation(anim_channel, arAnimation.GetDuration(), &animation_value);

            return anim_channel;
        }

        aiNode *GenerateNode(const string& aNodeName, size_t aMeshIndexStart, size_t aMeshIndexEnd) {
            auto *node = new aiNode();
            node->mName = aiString(aNodeName);
            node->mNumMeshes = aMeshIndexEnd - aMeshIndexStart;
            node->mMeshes = new unsigned int[node->mNumMeshes];
            for (size_t index = 0; index != node->mNumMeshes; index++) {
                node->mMeshes[index] = aMeshIndexStart;
                aMeshIndexStart++;
            }
            return node;
        }

        string GenerateTexturePNG(CColorMap &arMeshTexture, std::string &arOutputPath) {
            string textureName = arOutputPath +"_"+ arMeshTexture.GetName() + "_texture.png";
            vector<unsigned char> ClmBuffer = arMeshTexture.GetColorTexture();
            vivid::encodePNG(textureName, ClmBuffer, 1,
                             ClmBuffer.size() / 4);
            return textureName.substr(
                    textureName.find_last_of("/\\") + 1); // remove the path component from the texture name
        }

        aiTexture *GenerateTextureEmbedded(CColorMap &arMeshTexture) {
            auto *embedded_texture = new aiTexture();
            vector<unsigned char> ClmBuffer = arMeshTexture.GetColorTexture();
            const CMemoryBuffer pngBuffer = encodePNG(ClmBuffer, 1, ClmBuffer.size() / 4); // This is a work of art.
            strcpy(embedded_texture->achFormatHint, "png");
            embedded_texture->pcData = (aiTexel*)pngBuffer.mBuffer; // This is bullshit.
            embedded_texture->mWidth = pngBuffer.mSize;
            embedded_texture->mHeight = 0; // Assimp expects compressed embedded textures to be with this format.
            return embedded_texture;
        }

        string AddTexture(CColorMap arMeshCLM) {
            string texture_path;
            if(arMeshCLM.GetColorMap().size() == 1) return "";
            if (TextureNameToIndex.find(arMeshCLM.GetName()) !=
                TextureNameToIndex.end()) { //check if texture has been written already
                texture_path = TextureNameToIndex[arMeshCLM.GetName()];
            } // need to write the texture
            else {
                if(!EmbeddedTexture) texture_path = GenerateTexturePNG(arMeshCLM, OutputPath);
                if(EmbeddedTexture){
                    texture_path = '*' + std::to_string(EmbeddedTextures.size());
                    EmbeddedTextures.push_back(GenerateTextureEmbedded(arMeshCLM));
                }
                TextureNameToIndex[arMeshCLM.GetName()] = texture_path;
            }
            return texture_path;
        }

        aiMesh *GenerateMesh(CModelComponent * apMesh) {
            //setting up the apMesh structure
            auto *OutMesh = new aiMesh();
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
                in this segment we get the average color of each vertice.
            */
            //if clm is one color dont write UVs
            bool clm_is_texture = apMesh->GetColorMap().GetColorMap().size() > 1;
            if(clm_is_texture) {
                OutMesh->mTextureCoords[0] = new aiVector3D[vVertices.size()];
                OutMesh->mNumUVComponents[0] = 2;
            }


            //assign faces and find average UVcoord per point
            vector<set<float>> vetUVcoord(vVertices.size());
            for (auto it = vTriangles.begin(); it != vTriangles.end(); ++it) {
                aiFace &face = OutMesh->mFaces[it - vTriangles.begin()];
                const std::vector<size_t> indexes = it->GetPoints();
                face.mNumIndices = indexes.size();
                face.mIndices = new unsigned int[indexes.size()];
                for (int i = 0; i < indexes.size(); i++) {
                    face.mIndices[i] = indexes[i];
                    if(clm_is_texture) vetUVcoord[indexes[i]].insert(it->GetUVcoord()); //dont write uv cords if the clm is only one color
                }
            }

            //assign points
            double tempAvr, val;
            for (int j = 0; j < vVertices.size(); j++) {
                OutMesh->mVertices[j] = aiVector3D(vVertices[j].X(), vVertices[j].Y(), vVertices[j].Z());
                if(!clm_is_texture) continue; //dont write uv cords if the clm is only one color
                if (!vetUVcoord[j].empty()) { //uv coords
                    tempAvr = 0;
                    for (auto it : vetUVcoord[j]) {
                        tempAvr += it;
                    }
                    val = max(0.01, min(1 - tempAvr / vetUVcoord[j].size(), 1.0));
                    OutMesh->mTextureCoords[0][j] = aiVector3D(0, val, 0);
                } else { // should never happen
                    OutMesh->mTextureCoords[0][j] = aiVector3D(0, 0, 0);
                }
            }
            return OutMesh;
        }

        aiMaterial *GenerateMaterial(const vivid::CModelComponent& arMesh, const string& aTextureName, size_t mat_index) {
            const auto arMaterial = arMesh.GetMaterial();
            auto cmap = arMesh.GetColorMap().GetColorMap();
            auto *material = new aiMaterial();
            // name
            const aiString *name = new aiString(arMaterial.GetLabel() + "_mat"+ to_string(mat_index));
            material->AddProperty(name, AI_MATKEY_NAME);

            // base color
            aiColor4D *diffuse_color;
            if (cmap.size() == 1) {
                const auto base_color = ToNormalRGB(cmap[0]);
                diffuse_color = new aiColor4D(base_color[0], base_color[1], base_color[2], 1);
            } else {
                diffuse_color = new aiColor4D(1, 1, 1, 1);
            }
            material->AddProperty(diffuse_color, 4, AI_MATKEY_COLOR_DIFFUSE);

            //const aiColor3D *ambient_color = new aiColor3D(1, 1, 1);
            material->AddProperty(diffuse_color, 4, AI_MATKEY_COLOR_AMBIENT);
            // emissive color
            const auto emission_factor = arMaterial.GetEmissionStrength();
            const auto emission_color = min(emission_factor, 1.0f);
            aiColor3D *emissive_color;
            if (cmap.size() == 1) {
                const auto base_color = ToNormalRGB(cmap[0]);
                emissive_color = new aiColor3D(base_color[0] * emission_color, base_color[1] * emission_color, base_color[2] * emission_color);
            } else {
                emissive_color = new aiColor3D(emission_color, emission_color, emission_color);
            }
            material->AddProperty(emissive_color, 3,AI_MATKEY_COLOR_EMISSIVE);

            // emissive strength
            const float *emissive_intensity = new float(max(emission_factor, 1.0f));
            material->AddProperty(emissive_intensity, 1, AI_MATKEY_EMISSIVE_INTENSITY);

            // metalness
            const float *metalness = new float(arMaterial.GetMetalness());
            material->AddProperty(metalness, 1, AI_MATKEY_METALLIC_FACTOR);

            // roughness
            const float *roughness = new float(arMaterial.GetRoughness());
            material->AddProperty(roughness, 1, AI_MATKEY_ROUGHNESS_FACTOR);

            // opacity
            const float *opacity = new float(arMaterial.GetOpacity());
            material->AddProperty(opacity, 1, AI_MATKEY_OPACITY);

            // textures
            if(cmap.size() > 1) { //one color
                const int *uvwsrc = new int(0);
                material->AddProperty(uvwsrc, 1, AI_MATKEY_UVWSRC_AMBIENT(0));

                material->AddProperty(uvwsrc, 1, AI_MATKEY_UVWSRC_DIFFUSE(0));

                material->AddProperty(uvwsrc, 1, AI_MATKEY_UVWSRC_EMISSIVE(0));

                const aiString *texIndex = new aiString(aTextureName.substr(aTextureName.find_last_of("/\\") + 1));
                material->AddProperty(texIndex, AI_MATKEY_TEXTURE_DIFFUSE(0));

                material->AddProperty(texIndex, AI_MATKEY_TEXTURE_AMBIENT(0));

                material->AddProperty(texIndex, AI_MATKEY_TEXTURE_EMISSIVE(0));
            }
            // twosided
            const int *twosided = new int(1);
            material->AddProperty(twosided, 1, AI_MATKEY_TWOSIDED);

            // obj specific things
            const int *shading_model = new int(2);
            material->AddProperty(shading_model, 1, AI_MATKEY_SHADING_MODEL);

            const int *illum = new int(0);
            material->AddProperty(illum, 1, "$mat.illum", 4, 0);  // going around assimp system

            // TODO: Are the rest of these needed?

            const aiColor3D *specular_color = new aiColor3D(0, 0, 0);
            material->AddProperty(specular_color, 3, AI_MATKEY_COLOR_SPECULAR);

//            const aiColor3D *color_transparent = new aiColor3D(1, 1, 1);
//            material->AddProperty(color_transparent, 3, AI_MATKEY_COLOR_TRANSPARENT);

            const float *anisotrpy_factor = new float(0);
            material->AddProperty(anisotrpy_factor, 1, AI_MATKEY_ANISOTROPY_FACTOR);

            const float *refracti = new float(1);
            material->AddProperty(refracti, 1, AI_MATKEY_REFRACTI);

            return material;
        }



        aiScene *GenerateScene(vivid::CModel &model) {
            //setup scene
            auto *scene = new aiScene();
            scene->mFlags = AI_SCENE_FLAGS_ALLOW_SHARED;
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
                scene->mMaterials[i] = GenerateMaterial(meshes[i], texture_path, i);
                scene->mMeshes[i]->mMaterialIndex = i;
            }
            if(EmbeddedTexture){
                scene->mNumTextures = EmbeddedTextures.size();
                scene->mTextures = new aiTexture * [EmbeddedTextures.size()];
                for(int i = 0; i != EmbeddedTextures.size(); i++){scene->mTextures[i] = EmbeddedTextures[i];}
            }
            return scene;
        }

        aiAnimation *GenerateAnimation(CAnimation &arAnimation) {
            auto *anim = new aiAnimation;
            anim->mNumChannels = arAnimation.GetNumModels();
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
            auto *anim = new aiAnimation;
            anim->mNumChannels = arSMAnimation.GetNumModels();
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
                CPoint animation_value = arSMAnimation.GetFrames()[i].mMoveAnimation;
                MoveAnimation(anim->mChannels[i], (i + 1) * ticks_per_frame, &animation_value, i * ticks_per_frame);
                //Rotate
                animation_value = arSMAnimation.GetFrames()[i].mRotateAnimation;
                RotateAnimation(anim->mChannels[i], (i + 1) * ticks_per_frame, &animation_value, i * ticks_per_frame);
                //Scale
                animation_value = arSMAnimation.GetFrames()[i].mScaleAnimation;
                StopMotionScaleAnimation(anim->mChannels[i], (i + 1) * ticks_per_frame, &animation_value,
                                         i * ticks_per_frame);
            }
            return anim;
        }


        aiScene *GenerateAnimationScene(CAnimation &arAnimation) {
            //setup scene
            auto *scene = new aiScene();
            scene->mFlags = AI_SCENE_FLAGS_ALLOW_SHARED;
            scene->mRootNode = new aiNode();
            scene->mRootNode->mName = aiString("root_node");

            vector<vivid::CFrame> models = arAnimation.GetFrames();

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


            auto *SMAnimation = dynamic_cast<CStopMotionAnimation *>(&arAnimation);
            if (!SMAnimation) {
                scene->mAnimations[0] = GenerateAnimation(arAnimation);
            } else {
                scene->mAnimations[0] = GenerateStopMotionAnimation(*SMAnimation);
            }

            vector<vivid::CModelComponent> meshes;
            string texture_path;
            size_t mesh_counter = 0;
            for (int model_index = 0; model_index != models.size(); model_index++) {
                meshes = models[model_index].mModel.GetMeshes();
                center_node->mChildren[model_index] = GenerateNode("anim_node_" + to_string(model_index), mesh_counter,
                                                                   mesh_counter + meshes.size());
                center_node->mChildren[model_index]->mParent = center_node;
                for (auto & mesh : meshes) {
                    //handle mesh texture
                    texture_path = AddTexture(mesh.GetColorMap());
                    //setting up the mesh structure
                    scene->mMeshes[mesh_counter] = GenerateMesh(&mesh);
                    scene->mMaterials[mesh_counter] = GenerateMaterial(mesh, texture_path, mesh_counter);
                    scene->mMeshes[mesh_counter]->mMaterialIndex = mesh_counter;
                    mesh_counter++;
                }
            }
            if(EmbeddedTexture){
                scene->mNumTextures = EmbeddedTextures.size();
                scene->mTextures = new aiTexture * [EmbeddedTextures.size()];
                for(int i = 0; i != EmbeddedTextures.size(); i++){scene->mTextures[i] = EmbeddedTextures[i];}
            }
            return scene;
        }

    }

    namespace AssimpExport {

        CBlobData AssimpExporter(vivid::CModel &arModel, const std::string &arFileType) {
            Assimp::Exporter exp;
            EmbeddedTexture = true;

            aiScene *scene = GenerateScene(arModel);
            auto blob = exp.ExportToBlob(scene, RenameGLTF_GLB(arFileType),
                                   aiProcess_JoinIdenticalVertices | aiProcess_FixInfacingNormals | aiProcess_GenSmoothNormals);

            EmbeddedTextures.clear();
            EmbeddedTexture = false;
            TextureNameToIndex.clear();
            delete scene;
            
            if (!blob) {
                Log(LOG_ERROR, exp.GetErrorString());
            }
            // we copy all the blob data to a vivid const qualified struct so the blob pointer isn't freed by python's greedy GC
            return CBlobData::FormatExportDataBlob(blob);
        }

        CBlobData AnimationExporter(vivid::CAnimation &arAnimation, const std::string &arFileType) {
            Assimp::Exporter exp;
            EmbeddedTexture = true;

            aiScene *scene = GenerateAnimationScene(arAnimation);
            auto blob = exp.ExportToBlob(scene, RenameGLTF_GLB(arFileType),
                                         aiProcess_JoinIdenticalVertices | aiProcess_FixInfacingNormals | aiProcess_GenSmoothNormals);

            EmbeddedTextures.clear();
            EmbeddedTexture = false;
            TextureNameToIndex.clear();
            delete scene;

            if (!blob) {
                Log(LOG_ERROR, exp.GetErrorString());
            }

            return CBlobData::FormatExportDataBlob(blob);
        }

        void AssimpExporter(CModel &arModel, const std::string &arFileType, std::string aOutputPath) {
            Assimp::Exporter exp;
            OutputPath = aOutputPath;
            aiScene *scene = GenerateScene(arModel);

            //clean aOutputPath
            //if(aOutputPath.find('.')) aOutputPath = aOutputPath.substr(0, aOutputPath.find('.'));
            aOutputPath += FILE_FORMATS.at(arFileType);
            auto ret = exp.Export(scene, RenameGLTF_GLB(arFileType), aOutputPath,
                                  aiProcess_JoinIdenticalVertices | aiProcess_FixInfacingNormals | aiProcess_GenSmoothNormals);

            TextureNameToIndex.clear();
            OutputPath = "";
            delete scene;

            if (ret != aiReturn_SUCCESS) {
                Log(LOG_ERROR, exp.GetErrorString());
            }
        }

        void AnimationExporter(CAnimation &arAnimation, const std::string &arFileType, std::string aOutputPath) {
            Assimp::Exporter exp;
            OutputPath = aOutputPath;
            aiScene *scene = GenerateAnimationScene(arAnimation);

            //if(aOutputPath.find('.')) aOutputPath = aOutputPath.substr(0, aOutputPath.find('.'));
            aOutputPath += FILE_FORMATS.at(arFileType);
            auto ret = exp.Export(scene, RenameGLTF_GLB(arFileType), aOutputPath);
            
            TextureNameToIndex.clear();
            OutputPath = "";
            delete scene;
            
            if (ret != aiReturn_SUCCESS) {
                Log(LOG_ERROR, exp.GetErrorString());
            }
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


