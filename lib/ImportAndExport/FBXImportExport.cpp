#include "FBXImportExport.h"

using namespace vivid;

void vivid::FbfAnimNodes(std::vector<FbxNode*> FbxMeshNodes, string outputfile, float interval)          //animation based on nodes
{
	FbxManager* manager = FbxManager::Create();                                             //Creating the manager, the biggest container
	FbxScene* scene = FbxScene::Create(manager, "");                                //Creating the scene, this object will be the one exported
	FbxAnimStack* animStack = FbxAnimStack::Create(scene, "Animation stack");       //creating the animation stack, has to have a name to be recognized by software
	FbxAnimLayer* layer0 = FbxAnimLayer::Create(scene, "Base Layer");               //Animation layer
	animStack->AddMember(layer0);
	FbxTime time;
	FbxNode* SceneRootNode = scene->GetRootNode();
    // TODO Tomer where do you DELETE the pointers?

    for (auto l = 0; l < FbxMeshNodes.size(); l++)
    {
        SceneRootNode->AddChild(FbxMeshNodes[l]);
    }
	/* int a = 0;
	for (vector<FbxNode*>::iterator it = FbxMeshNodes.begin(); it != FbxMeshNodes.end(); ++it)
	{
		//FbxMeshNodes[a]->SetVisibility(false);
		
		a++;
	} */

	vector<int> keyIndexes;
	vector<FbxAnimCurve*> animCurves;
	animCurves.resize(FbxMeshNodes.size());
	keyIndexes.resize(FbxMeshNodes.size());
	for (auto i = 0; i < FbxMeshNodes.size(); i++)
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

            time.SetSecondDouble(interval * i + interval + 0.01);
            keyIndexes[i] = animCurves[i]->KeyAdd(time);
            animCurves[i]->KeySet(keyIndexes[i], time, 1.0, FbxAnimCurveDef::eInterpolationConstant);

            time.SetSecondDouble(interval * i + interval + 0.02);
            keyIndexes[i] = animCurves[i]->KeyAdd(time);
            animCurves[i]->KeySet(keyIndexes[i], time, 0.0, FbxAnimCurveDef::eInterpolationConstant);
        }
	}

	FbxSceneExport(scene, outputfile );
//	// Create an exporter.
//	FbxExporter* lExporter = FbxExporter::Create(manager, "");
//
//	// Initialize the exporter
//	bool lExportStatus = lExporter->Initialize(outputfile, -1, manager->GetIOSettings());
//
//	// Export the content of lScene to outputfile
//	lExporter->Export(scene);
}

void FbfAnimMeshes(std::vector<FbxMesh*> FbxMeshes, char* outputfile, float interval)
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
    for (auto it = FbxMeshes.begin(); it != FbxMeshes.end(); it++)
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
    for (auto it = FbxMeshes.begin(); it != FbxMeshes.end(); ++it)
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


void RotateAnim(CModel &arModel, float aLength, float aDuration, int aRotAxis, std::string &aOutputFilePath)
{
    // TODO clean code here. Too many param.
    FbxNode* CMeshNode = OneModelToFbx(arModel);
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
		 if (aRotAxis == 0) animCurve = CMeshNode->LclRotation.GetCurve(layer0, FBXSDK_CURVENODE_COMPONENT_X, true);
	else if (aRotAxis == 1) animCurve = CMeshNode->LclRotation.GetCurve(layer0, FBXSDK_CURVENODE_COMPONENT_Y, true);
	else if (aRotAxis == 2) animCurve = CMeshNode->LclRotation.GetCurve(layer0, FBXSDK_CURVENODE_COMPONENT_Z, true);


	if (animCurve)
	{
		animCurve->KeyModifyBegin();

		time.SetSecondDouble(0.0);
		keyIndex = animCurve->KeyAdd(time);
		animCurve->KeySet(keyIndex, time, 0.0, FbxAnimCurveDef::eInterpolationLinear);

		time.SetSecondDouble(aDuration);
		keyIndex = animCurve->KeyAdd(time);
		animCurve->KeySet(keyIndex, time, -aLength, FbxAnimCurveDef::eInterpolationLinear);

		animCurve->KeyModifyEnd();
	}

    FbxSceneExport(scene, aOutputFilePath);
//	// Create an exporter.
//	FbxExporter* lExporter = FbxExporter::Create(manager, "");
//
//	// Initialize the exporter
//	bool lExportStatus = lExporter->Initialize(outputfile, -1, manager->GetIOSettings());
//
//	// Export the content of lScene to outputfile
//	lExporter->Export(scene);
}

void Animate(std::vector<CModel> &arModels, double aInterval, std::string aOutputFilePath)
{
    vector<FbxNode*> nodes; //TODO (TOMER) iterator!!! for each loop or even by some vector command!
    for (int l = 0; l < arModels.size(); ++l)
    {
        nodes.push_back(OneModelToFbx(arModels[l]));
    }
    FbfAnimNodes(nodes, aOutputFilePath, aInterval);
}

void RotateAnimTextures(CModel model, float length, float duration, int RotAxis, const string& outputfile){
    //Initialize the nodes for animation
    FbxManager* manager = FbxManager::Create();
    FbxScene* scene = FbxScene::Create(manager, "1");
    FbxNode* node = OneModelToFbxTextures(model, scene, outputfile);

    //Initialize animation variables and objects
        //Create animation stack and add layer to it, since we only have one type of animation (rotation) we only need one layer
    FbxAnimStack* AnimStack = FbxAnimStack::Create(scene, "animation stack");
    FbxAnimLayer* layer0 = FbxAnimLayer::Create(scene, "layer 0");
    AnimStack->AddMember(layer0);
        //Create variable to assign to animation stack that actually detail the animation itself
    FbxTime time;
    FbxAnimCurveKey AnimCurveKey;
    FbxAnimCurve* AnimCurve = NULL;
    int KeyIndex = 0;

    //Create animation information
        //We create a type of animation based on the variable RotAxis
    node->LclRotation.GetCurveNode(layer0, true);
    if (RotAxis == 0) AnimCurve = node->LclRotation.GetCurve(layer0, FBXSDK_CURVENODE_COMPONENT_X, true);
    else if (RotAxis == 1) AnimCurve = node->LclRotation.GetCurve(layer0, FBXSDK_CURVENODE_COMPONENT_Y, true);
    else if (RotAxis == 2) AnimCurve = node->LclRotation.GetCurve(layer0, FBXSDK_CURVENODE_COMPONENT_Z, true);
        //Create animation curve values
    if (AnimCurve)
    {
        AnimCurve->KeyModifyBegin();

        time.SetSecondDouble(0.0);
        KeyIndex = AnimCurve->KeyAdd(time);
        AnimCurve->KeySet(KeyIndex, time, 0.0, FbxAnimCurveDef::eInterpolationLinear);

        time.SetSecondDouble(duration);
        KeyIndex = AnimCurve->KeyAdd(time);
        AnimCurve->KeySet(KeyIndex, time, -length, FbxAnimCurveDef::eInterpolationLinear);

        AnimCurve->KeyModifyEnd();
    }
    else {
        cout << "RotAxis is probably defined poorly, it can be 0 for x axis, 1 for y axis and 2 for z axis." << endl;
        abort();
    }

    //Export the scene
    FbxSceneExport(scene, outputfile);
}


void AnimateTextures(std::vector<CModel> Models, double Interval, const string& OutputFile){
    //Initialize required container variables and some variables for later
    int ModelsSize = Models.size();
    FbxManager* manager = FbxManager::Create();
    FbxScene* scene = FbxScene::Create(manager, "scene");

    //Get the nodes from the models vector
    vector<FbxNode*> ModelNodes;
    for (int l = 0; l < Models.size(); ++l) {
        ModelNodes.push_back(OneModelToFbxTextures(Models[l], scene, OutputFile));
    }

    //Assign animation curves to each model
        //Initialize animation variables
    FbxAnimStack* AnimStack = FbxAnimStack::Create(scene, "Animation stack");       //creating the animation stack, has to have a name to be recognized by software
    FbxAnimLayer* layer0 = FbxAnimLayer::Create(scene, "Base Layer");               //Animation layer
    AnimStack->AddMember(layer0);
    FbxTime time;
    vector<int> KeyIndexes;
    vector<FbxAnimCurve*> AnimCurves;
    AnimCurves.resize(ModelsSize);
    KeyIndexes.resize(ModelsSize);
        //Create and assign the curves
    for (int i = 0; i < ModelsSize; ++i)
    {
        AnimCurves[i] = NULL;
        KeyIndexes[i] = 0;
        AnimCurves[i] = ModelNodes[i]->Visibility.GetCurve(layer0, true);
        if (AnimCurves[i])
        {
            AnimCurves[i]->KeyModifyBegin();
            time.SetSecondDouble(Interval * i);
            KeyIndexes[i] = AnimCurves[i]->KeyAdd(time);
            AnimCurves[i]->KeySet(KeyIndexes[i], time, 0.0, FbxAnimCurveDef::eInterpolationConstant);

            time.SetSecondDouble(Interval * (i + 1) + 0.01);
            KeyIndexes[i] = AnimCurves[i]->KeyAdd(time);
            AnimCurves[i]->KeySet(KeyIndexes[i], time, 1.0, FbxAnimCurveDef::eInterpolationConstant);

            time.SetSecondDouble(Interval * (i + 1) + 0.02);
            KeyIndexes[i] = AnimCurves[i]->KeyAdd(time);
            AnimCurves[i]->KeySet(KeyIndexes[i], time, 0.0, FbxAnimCurveDef::eInterpolationConstant);
        }
    }

    //Export scene
    FbxSceneExport(scene, std::string(OutputFile) );
}



//void AnimateTextures(vector<CModel> models, double interval,  char* outputfile){
//    FbxManager* manager = FbxManager::Create();
//    FbxScene* scene = FbxScene::Create(manager, "1");
//    FbxNode* SceneRootNode = scene->GetRootNode();
//    for (int l = 0; l < models.size(); ++l) {
//        SceneRootNode->AddChild(OneMeshToFbxTextures())
//
//    }
//}

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
//			CSurface surf = surf.CreateSurf(temp.points, temp.mask, temp.quan, Vmin, Vmax);
//			surf.Smooth();
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


