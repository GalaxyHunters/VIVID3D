//
// Created by zorik on 15/09/2019.
//

#include "FbxFunc.h"

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
    FbxNode* sceneRootNode = scene->GetRootNode();
    sceneRootNode->AddChild(Node);
    FbxExporter* exporter = FbxExporter::Create(manager, "");
    exporter->Initialize(output, -1, manager->GetIOSettings());
    exporter->Export(scene);
}

int verticeCountModel(CModel model)
{
    vector<CMesh> CMeshes = model.GetMeshes();
    int verticesCount(0);
    for (int l = 0; l < CMeshes.size(); ++l)
    {
        verticesCount = verticesCount + CMeshes[l].GetPoints().size();
    }
    return verticesCount;
}

vector<FbxDouble3> MeshToFbxMaterials(CMesh mesh)
{
    //colors.clear();
    vector<CIndexedFace> faces = mesh.GetFaces();
    vector<FbxDouble3> colors;

    for (int l = 0; l < faces.size(); ++l)
    {
//        vector<color_t> MeshColors;
//        cout << faces[l].GetColor() << endl;
//        MeshColors.GetColor(0.3);
        colors.push_back(FbxDouble3(GetColor(faces[l].GetColor()).R, GetColor(faces[l].GetColor()).G, GetColor(faces[l].GetColor()).B));
    }

    return colors;
}

FbxNode* OneMeshToFbx(CMesh CMesh)
{
    FbxManager* manager = FbxManager::Create();
    FbxMesh* FMesh = FbxMesh::Create(manager, "mesh");
    FbxNode* Node = FbxNode::Create(manager, "node");
    vector<FbxDouble3> ColorValues = MeshToFbxMaterials(CMesh);
    vector<FbxSurfacePhong*> FbxMaterials(ColorValues.size());

    vector<CPoint> CMeshPoints = CMesh.GetPoints();
    vector<CIndexedFace> CMeshFaces = CMesh.GetFaces();

    FbxGeometryElementMaterial* MaterialElement = FMesh->CreateElementMaterial();
    MaterialElement->SetMappingMode(FbxGeometryElement::eByPolygon);
    MaterialElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);

    FMesh->InitControlPoints(CMeshPoints.size());
    FbxVector4* FMeshCP = FMesh->GetControlPoints();

    for (int l = 0; l < CMeshPoints.size(); ++l)
    {
        FMeshCP[l].Set(CMeshPoints[l].GetX(), CMeshPoints[l].GetY(), CMeshPoints[l].GetZ());
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
    vector<CMesh> Meshes = Model.GetMeshes();
    vector<FbxNode*> Nodes(Meshes.size());
    FbxNode* ParentNode = FbxNode::Create(manager, "ParentNode");
    for (int l = 0; l < Meshes.size(); ++l)
    {
        Nodes[l] = OneMeshToFbx(Meshes[l]);
        ParentNode->AddChild(Nodes[l]);
    }
    return ParentNode;
}

