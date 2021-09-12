#include "FbxCode.h"
#include "DataToImage.h"
#include "Decimate.h"

using namespace vivid;
using namespace std;

// TODO: Is this even worth refactoring if Assimp will let us do this in a second?
void FbxSceneExport(FbxScene* scene, const string& outputfile)
{
    FbxManager* manager = FbxManager::Create();
    FbxExporter* lExporter = FbxExporter::Create(manager, "exporter");
    bool lExportStatus = lExporter->Initialize(outputfile.c_str(), -1, manager->GetIOSettings());
    lExporter->Export(scene);
}


void FbxMeshExport(FbxMesh* CMesh, char* output)
{
    FbxManager* manager = FbxManager::Create();
    FbxScene* scene = FbxScene::Create(manager, "");
    FbxNode* CMeshNode = FbxNode::Create(manager, "");
    CMeshNode->AddNodeAttribute(CMesh);
    FbxNode* sceneRootNode = scene->GetRootNode();
    sceneRootNode->AddChild(CMeshNode);
    FbxExporter* exporter = FbxExporter::Create(manager, "");
    exporter->Initialize(output, -1, manager->GetIOSettings());
    exporter->Export(scene);
}

void FbxNodeExport(FbxNode* Node, char* output)
{
    FbxManager* manager = FbxManager::Create();
    FbxScene* scene = FbxScene::Create(manager, "");
    FbxNode* sNode = scene->GetRootNode();
    sNode->AddChild(Node);
    FbxExporter* exporter = FbxExporter::Create(manager, "");
    exporter->Initialize(output, -1, manager->GetIOSettings());
    exporter->Export(scene);
}


vector<FbxDouble3> MeshToFbxMaterials(CModelComponent mesh)
{
    //colors.clear();
    vector<CFace> faces = mesh.GetFaces();
    vector<FbxDouble3> colors;

    for (int l = 0; l < faces.size(); ++l)
    {
//        vector<Color_t> MeshColors;
//        cout << faces[l].GetColor() << endl;
//        MeshColors.GetColor(0.3);
        colors.push_back(FbxDouble3(mesh.GetClm().GetColor(faces[l].GetQuan()).R, mesh.GetClm().GetColor(faces[l].GetQuan()).G, mesh.GetClm().GetColor(faces[l].GetQuan()).B));
    }

    return colors;
}

FbxNode* OneMeshToFbx(CModelComponent CMesh)
{
    FbxManager* manager = FbxManager::Create();
    FbxMesh* FMesh = FbxMesh::Create(manager, "mesh");
    FbxNode* Node = FbxNode::Create(manager, "node");
    vector<FbxDouble3> ColorValues = MeshToFbxMaterials(CMesh);
    vector<FbxSurfacePhong*> FbxMaterials(ColorValues.size());

    vector<CPoint> CMeshPoints = CMesh.GetPoints();
    vector<CFace> CMeshFaces = CMesh.GetFaces();

    FbxGeometryElementMaterial* MaterialElement = FMesh->CreateElementMaterial();
    MaterialElement->SetMappingMode(FbxGeometryElement::eByPolygon);
    MaterialElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);

    FMesh->InitControlPoints(CMeshPoints.size());
    FbxVector4* FMeshCP = FMesh->GetControlPoints();

    for (int l = 0; l < CMeshPoints.size(); ++l)
    {
        FMeshCP[l].Set(CMeshPoints[l].X(), CMeshPoints[l].Y(), CMeshPoints[l].Z());
    }

    for (int i1 = 0; i1 < ColorValues.size(); ++i1)
    {
        FbxMaterials[i1] = FbxSurfacePhong::Create(manager, "color");
        FbxMaterials[i1]->Diffuse.Set(ColorValues[i1]);
        FbxMaterials[i1]->TransparencyFactor.Set(CMesh.GetAlpha());
        Node->AddMaterial(FbxMaterials[i1]);
    }

    for (int k1 = 0; k1 < CMeshFaces.size(); ++k1)
    {
        vector<size_t> FacePoints = CMeshFaces[k1].GetPoints();

        FMesh->BeginPolygon(k1); //was j

        for (int m1 = 0; m1 < FacePoints.size(); ++m1)
        {
            FMesh->AddPolygon(FacePoints[m1]);
        }
        //l++; //was j
        FMesh->EndPolygon();
    }

    Node->SetNodeAttribute(FMesh);
    return Node;
}

FbxNode* OneModelToFbx(CModel Model)
{
    FbxManager* manager = FbxManager::Create();
    vector<CModelComponent> Meshes = Model.GetMeshes();
    vector<FbxNode*> Nodes(Meshes.size());
    FbxNode* ParentNode = FbxNode::Create(manager, "ParentNode");
    for (int l = 0; l < Meshes.size(); ++l)
    {
        Nodes[l] = OneMeshToFbx(Meshes[l]);
        ParentNode->AddChild(Nodes[l]);
    }
    return ParentNode;
}

int GetCPSize(CModelComponent mesh)
{
    int CPSize = 0;
    vector<CFace> faces = mesh.GetFaces();
    for (int l = 0; l < faces.size(); ++l)
    {
        CPSize = CPSize + faces[l].GetPoints().size();
    }
    return CPSize;
}
//


void CreateTexture(FbxScene* scene, FbxMesh* FMesh, CModelComponent mesh, double AlphaFactor, const string& outputpath){

    //checks if a texture png file already exists, if not, it creates the texture file
//    const char *TexturePath;
    if (CheckTexture(string(outputpath) + string("_texture.png")) == false) {
        cout << "no texture found, creating texture" << endl;
        vector<unsigned char> texture;
        texture = mesh.GetClm().GetColorTexture();
        encodePNG( string(outputpath + "_texture.png"), texture, 1, texture.size() / 4);
//        TexturePath = ();
        cout << "texture creation done" << endl;
    }
    else {
        cout << "texture found, skipping texture creation" << endl;
    }

    FbxSurfacePhong* Material = FbxSurfacePhong::Create(scene, "material");
    FbxNode* Node = FMesh->GetNode();
    FbxNode* sNode = scene->GetRootNode();
    sNode->AddChild(Node);
    if (Node){
        cout << "mesh node found" << endl;
        Material->TransparencyFactor.Set(AlphaFactor);
        Node->AddMaterial(Material);
    }
    else {
        cout << "mesh isnt connected to node" << endl;
    }

    FbxFileTexture* Texture = FbxFileTexture::Create(scene,"Diffuse Texture");
    Texture->SetFileName((string(outputpath + "_texture.png")).c_str()); // Resource file is in current directory.
    Texture->SetTextureUse(FbxTexture::eStandard);
    Texture->SetMappingType(FbxTexture::eUV);
    Texture->SetMaterialUse(FbxFileTexture::eModelMaterial);
    Texture->SetSwapUV(false);
    Texture->SetTranslation(0.0, 0.0);
    Texture->SetScale(0.0, 1.0);                                                         //Notice that this means there is 1 possible value of x, and that is 0. basically, instead of using a 2d uv map, we are using a one dimensional line of color. we do this because we require static colors, not complicated textures.
    Texture->SetRotation(0.0, 0.0);

    if (Material){
        cout << "material found" << endl;
        Material->Diffuse.ConnectSrcObject(Texture);
    }
    else {
        cout << "Material isnt found" << endl;
    }
}

FbxNode* OneMeshToFbxTextures(CModelComponent mesh, FbxScene* scene, const string& outputpath)
{
    //Initialize node + mesh and connect them
    FbxNode* Node = FbxNode::Create(scene, "convee");
    FbxMesh* FMesh = FbxMesh::Create(scene, "convee");
    Node->SetNodeAttribute(FMesh);

    //Create control points for the mesh
    vector<CPoint> CPoints = mesh.GetPoints();
    vector<CFace> CFaces = mesh.GetFaces();
    //int PointsSize = CPoints.size();                                        //maybe useless, for memory purposes
    int FacesSize = CFaces.size();
    FMesh->InitControlPoints(GetCPSize(mesh));                              //Initialize the control point (aka vertex) array of the FBX mesh
    FbxVector4* FMeshCP = FMesh->GetControlPoints();                                 //putting the array into a workable variable

    for (int l = 0; l < CPoints.size(); ++l)
    {
        FMeshCP[l].Set(CPoints[l].X(), CPoints[l].Y(), CPoints[l].Z());
    }


    //create UV map

        //create the actual object
    FbxGeometryElementUV* UVMap = FMesh->CreateElementUV( "DiffuseUV");
    //K_ASSERT( lUVDiffuseElement != NULL);
    UVMap->SetMappingMode(FbxGeometryElement::eByPolygonVertex);
    UVMap->SetReferenceMode(FbxGeometryElement::eIndexToDirect);

        //add values to it (see old function)
    for (float l = 75; l > 0; l--){
        UVMap->GetDirectArray().Add(FbxVector2(0, l/75));                           //This loop adds all possible colors from the texture to the UV map vertice array
        //This is basically ColorMap's ClmPlasma map, except its saved in a UV index way
    }
    UVMap->GetIndexArray().SetCount(GetCPSize(mesh));

    //create faces for the mesh
    for (int k1 = 0; k1 < CFaces.size(); ++k1){
        vector<size_t> FacePoints = CFaces[k1].GetPoints();
        int FaceColorIndex = mesh.GetClm().GetColorIndex(CFaces[k1].GetQuan());
        FMesh->BeginPolygon(-1, -1, false); //was j

        for (int m1 = 0; m1 < FacePoints.size(); ++m1)
        {
            FMesh->AddPolygon(FacePoints[m1], FaceColorIndex);
        }
        //l++; //was j
        FMesh->EndPolygon();
    }


//    c = 0;
//    for (int k1 = 0; k1 < FacesSize; ++k1)
//    {
//        int CurrentFS = CFaces[k1].GetPoints().size();
//        int FaceColorIndex = GetColorIndex(CFaces[k1].GetColor());
//
//        FMesh->BeginPolygon(-1, -1, false);
//
//        for (int l = c; l < c + CurrentFS; ++l)
//        {
//            FMesh->AddPolygon(l);                                               //TODO i think this might be wrong, check
//            UVMap->GetIndexArray().SetAt(l, FaceColorIndex);
//        }
//
//        FMesh->EndPolygon();
//        c = c + CurrentFS;
//    }

    //set node shading mode (literally 1 line)
    Node->SetShadingMode(FbxNode::eTextureShading);

    //create texture using function
    CreateTexture(scene, FMesh, mesh, 0.8, outputpath);

    //pray to the gods of fbx sdk that this shit works

    return Node;
}


FbxNode* OneModelToFbxTextures(CModel model, FbxScene* scene, const string& outputpath){
    FbxNode* ParentNode = FbxNode::Create(scene, "model");
    vector<CModelComponent> meshes = model.GetMeshes();
    for (int l = 0; l < meshes.size(); ++l) {
        ParentNode->AddChild(OneMeshToFbxTextures(meshes[l], scene, outputpath));
    }
    return ParentNode;
}