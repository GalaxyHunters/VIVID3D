//#include "stdafx.h"

#include <string>
#include "Animation.h"
#include "ModelBuilder/Model.h"
#include "Utils/ReadBinFile.h"
#include "ModelBuilder/Surf.h"





using namespace std;


void FbfAnimNodes(vector<FbxNode*> FbxMeshNodes, char* outputfile, float interval)          //animation based on nodes
{
	FbxManager* manager = FbxManager::Create();                                             //Creating the manager, the biggest container
	FbxScene* scene = FbxScene::Create(manager, "");                                //Creating the scene, this object will be the one exported
	FbxAnimStack* animStack = FbxAnimStack::Create(scene, "Animation stack");       //creating the animation stack, has to have a name to be recognized by software
	FbxAnimLayer* layer0 = FbxAnimLayer::Create(scene, "Base Layer");               //Animation layer
	animStack->AddMember(layer0);
	FbxTime time;
	FbxNode* SceneRootNode = scene->GetRootNode();

    for (int l = 0; l < FbxMeshNodes.size(); ++l)
    {
        SceneRootNode->AddChild(FbxMeshNodes[l]);
    }
	/* int a = 0;
	for (vector<FbxNode*>::iterator it = FbxMeshNodes.begin(); it != FbxMeshNodes.end(); ++it)
	{
		//FbxMeshNodes[a]->SetVisibility(false);
		
		a++;
	} */

	int i = 0;
	vector<int> keyIndexes;
	vector<FbxAnimCurve*> animCurves;
	animCurves.resize(FbxMeshNodes.size());
	keyIndexes.resize(FbxMeshNodes.size());
	for (vector<FbxNode*>::iterator it = FbxMeshNodes.begin(); it != FbxMeshNodes.end(); ++it)
	{
		animCurves[i] = NULL;
		keyIndexes[i] = 0;
		animCurves[i] = FbxMeshNodes[i]->Visibility.GetCurve(layer0, true);
        if (animCurves[i])
        {
            animCurves[i]->KeyModifyBegin();
            time.SetSecondDouble(interval * i);
            keyIndexes[i] = animCurves[i]->KeyAdd(time);
            animCurves[i]->KeySet(keyIndexes[i], time, 0.0, FbxAnimCurveDef::eInterpolationConstant);

            time.SetSecondDouble(interval * i + interval + 0.02);
            keyIndexes[i] = animCurves[i]->KeyAdd(time);
            animCurves[i]->KeySet(keyIndexes[i], time, 1.0, FbxAnimCurveDef::eInterpolationConstant);

            time.SetSecondDouble(interval * i + interval + 0.01);
            keyIndexes[i] = animCurves[i]->KeyAdd(time);
            animCurves[i]->KeySet(keyIndexes[i], time, 0.0, FbxAnimCurveDef::eInterpolationConstant);
        }
        i++;
	}

	// Create an exporter.
	FbxExporter* lExporter = FbxExporter::Create(manager, "");

	// Initialize the exporter
	bool lExportStatus = lExporter->Initialize(outputfile, -1, manager->GetIOSettings());

	// Export the content of lScene to outputfile
	lExporter->Export(scene);
}

void FbfAnimMeshes(vector<FbxMesh*> FbxMeshes, char* outputfile, float interval)
{
    FbxManager* manager = FbxManager::Create();
    FbxScene* scene = FbxScene::Create(manager, "");
    FbxAnimStack* animStack = FbxAnimStack::Create(scene, "Animation stack");
    FbxAnimLayer* layer0 = FbxAnimLayer::Create(scene, "Base Layer");
    animStack->AddMember(layer0);
    FbxNode* sceneRootNode = scene->GetRootNode();
    FbxTime time;
    vector<FbxNode*> animNodes;
    animNodes.resize(FbxMeshes.size());
    int a = 0;
    for (vector<FbxMesh*>::iterator it = FbxMeshes.begin(); it != FbxMeshes.end(); ++it)
    {
        animNodes[a] = FbxNode::Create(scene, "");		//creates a node to connect to the CMesh (for later use).
        animNodes[a]->SetNodeAttribute(FbxMeshes[a]);							//set the node atrribute to CMesh. this lets the node know that he is connected to a CMesh.
        sceneRootNode->AddChild(animNodes[a]);
        a++;
    }

    int i = 0;
    vector<int> keyIndexes;
    vector<FbxAnimCurve*> animCurves;
    animCurves.resize(FbxMeshes.size());
    keyIndexes.resize(FbxMeshes.size());
    for (vector<FbxMesh*>::iterator it = FbxMeshes.begin(); it != FbxMeshes.end(); ++it)
    {
        animCurves[i] = NULL;
        keyIndexes[i] = 0;
        animCurves[i] = animNodes[i]->Visibility.GetCurve(layer0, true);
        if (animCurves[i])
        {
            animCurves[i]->KeyModifyBegin();
            time.SetSecondDouble(interval * i);
            keyIndexes[i] = animCurves[i]->KeyAdd(time);
            animCurves[i]->KeySet(keyIndexes[i], time, 0.0, FbxAnimCurveDef::eInterpolationConstant);

            time.SetSecondDouble(interval * i + interval + 0.02);
            keyIndexes[i] = animCurves[i]->KeyAdd(time);
            animCurves[i]->KeySet(keyIndexes[i], time, 1.0, FbxAnimCurveDef::eInterpolationConstant);

            time.SetSecondDouble(interval * i + interval + 0.01);
            keyIndexes[i] = animCurves[i]->KeyAdd(time);
            animCurves[i]->KeySet(keyIndexes[i], time, 0.0, FbxAnimCurveDef::eInterpolationConstant);
        }
        i++;
    }

    // Create an exporter.
    FbxExporter* lExporter = FbxExporter::Create(manager, "");

    // Initialize the exporter
    bool lExportStatus = lExporter->Initialize(outputfile, -1, manager->GetIOSettings());

    // Export the content of lScene to outputfile
    lExporter->Export(scene);
}

void RotateAnim(CModel model, float length, float duration, int RotAxis, char* outputfile)
{
    FbxNode* CMeshNode = OneModelToFbx(model);
	FbxManager* manager = FbxManager::Create();
	FbxScene* scene = FbxScene::Create(manager, "scene");
	FbxAnimStack* animStack = FbxAnimStack::Create(scene, "animation stack");
	FbxAnimLayer* layer0 = FbxAnimLayer::Create(scene, "layer 0");
	animStack->AddMember(layer0);
	FbxTime time;
	FbxAnimCurveKey animCurveKey;
	FbxNode* sceneRootNode = scene->GetRootNode();
	sceneRootNode->AddChild(CMeshNode);
	FbxAnimCurve* animCurve = NULL;
	int keyIndex = 0; 


	CMeshNode->LclRotation.GetCurveNode(layer0, true);
		 if (RotAxis == 0) animCurve = CMeshNode->LclRotation.GetCurve(layer0, FBXSDK_CURVENODE_COMPONENT_X, true);
	else if (RotAxis == 1) animCurve = CMeshNode->LclRotation.GetCurve(layer0, FBXSDK_CURVENODE_COMPONENT_Y, true);
	else if (RotAxis == 2) animCurve = CMeshNode->LclRotation.GetCurve(layer0, FBXSDK_CURVENODE_COMPONENT_Z, true);



	if (animCurve)
	{
		animCurve->KeyModifyBegin();

		time.SetSecondDouble(0.0);
		keyIndex = animCurve->KeyAdd(time);
		animCurve->KeySet(keyIndex, time, 0.0, FbxAnimCurveDef::eInterpolationLinear);

		time.SetSecondDouble(duration);
		keyIndex = animCurve->KeyAdd(time);
		animCurve->KeySet(keyIndex, time, -length, FbxAnimCurveDef::eInterpolationLinear);

		animCurve->KeyModifyEnd();
	}
	// Create an exporter.
	FbxExporter* lExporter = FbxExporter::Create(manager, "");

	// Initialize the exporter
	bool lExportStatus = lExporter->Initialize(outputfile, -1, manager->GetIOSettings());

	// Export the content of lScene to outputfile
	lExporter->Export(scene);
}

void Animate(vector<CModel> models, double interval,  char* outputfile)
{
    vector<FbxNode*> nodes;
    for (int l = 0; l < models.size(); ++l)
    {
        nodes.push_back(OneModelToFbx(models[l]));
    }
    FbfAnimNodes(nodes, outputfile, interval);
}



//vector<FbxMesh*> ModelVecToAnimVec(vector<CModel> models)
//{
//	vector<FbxMesh*> fbxMeshes(models.size());
//	fbxMeshes.clear();
//
//	for (size_t i = 0; i < models.size(); i++)
//	{
//		fbxMeshes.push_back(OneModelToFbx(models[i]));
//	}
//
//	return fbxMeshes;
//}

//void CreateAnimFromBins(vector<string> inputFiles, char* outputfile, double interval, int MeshesInModel)
//{
//	vector<CModel> Models(inputFiles.size() / MeshesInModel);
//
//	for (size_t j = 0; j < inputFiles.size() / MeshesInModel; j++)
//	{
//		for (int i = 0; i < MeshesInModel; i++)
//		{
//			ModelData temp = ReadBin(inputFiles[i]);
//			cord_t Vmax = *max_element(temp.quan.begin(), temp.quan.end());
//			cord_t Vmin = *min_element(temp.quan.begin(), temp.quan.end());
//			
//			CSurf surf = surf.CreateSurf(temp.points, temp.mask, temp.quan, Vmin, Vmax);
//			surf.SmoothSurf();
//			CMesh mesh = surf.ToMesh("mesh" + to_string(i), 0.2);
//
//			Models[j].AddMesh(mesh);
//		}
//	}
//
//	vector<FbxMesh*> FbxMeshes = ModelVecToAnimVec(Models);
//	FbfAnimMeshes(FbxMeshes, outputfile, interval);
//}

//vector<string> loadFile(char* inputFile)
//{
//	ifstream file;
//	file.open(inputFile);
//	assert(file);
//
//	vector<string> fileContents;
//	copy(istream_iterator<string>(file), istream_iterator<string>(), back_inserter(fileContents));
//
//	return fileContents;
//}


//FbxMesh* ObjToFbxMesh(char* inputFile)
//{
//	FbxManager* manager = FbxManager::Create();
//	FbxMesh* CMesh = FbxMesh::Create(manager, "");
//	vector<string> fileContents = loadFile(inputFile);
//	int verticeAmount = 0;
//	int i = 0;
//
//	for (vector<string>::iterator letterIt = fileContents.begin(); letterIt != fileContents.end(); ++letterIt)
//	{
//		if (*letterIt == "v")
//		{
//			++verticeAmount;
//		}
//	}
//
//	vector<FbxVector4> fbxVertices(verticeAmount);
//	CMesh->InitControlPoints(verticeAmount);
//	FbxVector4* controlPoints = CMesh->GetControlPoints();
//	fbxVertices.clear();
//
//	for (vector<string>::iterator letterIt = fileContents.begin(); letterIt != fileContents.end(); ++letterIt)
//	{
//		if (*letterIt == "v")
//		{
//			++letterIt;
//			++letterIt;
//			controlPoints[i].Set(stod(*--letterIt), stod(*letterIt), stod(*++letterIt));
//		}
//
//		if (*letterIt == "f")
//		{
//			++letterIt;
//			CMesh->BeginPolygon();
//			while (*letterIt != "f")
//			{
//				CMesh->AddPolygon(stoi(*letterIt));
//				++letterIt;
//			}
//			CMesh->EndPolygon();
//		}
//	}
//
//	return CMesh;
//}


