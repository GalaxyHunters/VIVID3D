#include "stdafx.h"
#include <vector>
#include <iostream>
#include <fbxsdk.h>
#include <fbxsdk\fileio\fbxiosettings.h>
#include <string>

using namespace std;

int KFAnim(vector<char*> inputfiles, char* outputfile, int interval){
	// Create the FBX SDK manager
	int i = 0;
	FbxManager* lSdkManager = FbxManager::Create();
	vector<FbxObject*> objList;
	vector<FbxAnimLayer*> animLayers;
	objList.resize(inputfiles.size());
	animLayers.resize(inputfiles.size());
	// Create an IOSettings object.
	FbxIOSettings * ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);

	// ... Configure the FbxIOSettings object ...

	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

	// Create a new scene so it can be populated by the imported file.
	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

	// Declare the path and filename of the file containing the scene.
	// In this case, we are assuming the file is in the same directory as the executable.s
	for (vector<char*>::iterator it = inputfiles.begin(); it != inputfiles.end(); ++it)
	{
		// Initialize the importer.
		bool lImportStatus = lImporter->Initialize(*it, -1, lSdkManager->GetIOSettings());

		if (!lImportStatus) {
			printf("Call to FbxImporter::Initialize() failed.\n");
			printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
			exit(-1);
		}

		objList[i] = FbxScene::Create(lSdkManager, "obj" + i);

		// Import the contents of the file into the scene.
		lImporter->Import((FbxScene*)objList[i]);

		animLayers[i] = FbxAnimLayer::Create(lScene, "layer");
		animLayers[i]->AddMember(objList[i]);
		i++;
	}	

	// The file has been imported; we can get rid of the importer.
	lImporter->Destroy();
	FbxAnimCurve* myAnimCurve = FbxAnimCurve::Create(lScene, "");
	FbxTime myTime;
	int myKeyIndex = 0;
	for (vector<FbxAnimLayer*>::iterator pLayer = animLayers.begin(); pLayer != animLayers.end(); ++pLayer)
	{
		// First the start keyframe
		
		(*pLayer)->AddMember(myAnimCurve);
		myAnimCurve->KeyModifyBegin();
		myTime.SetSecondDouble(0.0);             // Starting time
		myKeyIndex = myAnimCurve->KeyAdd(0); // Add the start key; returns 0

		myAnimCurve->KeySet(0,           // Set the zero’th key
			0,               // Starting time
			0.0,                 // Starting X value
			FbxAnimCurveDef::eInterpolationLinear);// Straight line between 2 points

		// Then the stop keyframe
		myTime.SetSecondDouble(20.0);
		myKeyIndex = myAnimCurve->KeyAdd(myTime);
		myAnimCurve->KeySet(myKeyIndex, myTime, 500.0, FbxAnimCurveDef::eInterpolationLinear);
		myAnimCurve->KeyModifyEnd();
	}

	// Create an exporter.
	FbxExporter* lExporter = FbxExporter::Create(lSdkManager, "");

	// Initialize the exporter
	bool lExportStatus = lExporter->Initialize(outputfile, -1, lSdkManager->GetIOSettings());

	if (!lExportStatus) {
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
		exit(-1);
	}

	// Export the content of lScene to outputfile
	lExporter->Export(lScene);

	system("pause");
	return 0;
}


int main(){
vector<char*> list;
list.push_back("C:\\Users\\zorik\\Documents\\alpha\\models\\sgal_a24_gas_1e26.obj");
list.push_back("C:\\Users\\zorik\\Documents\\alpha\\models\\sgal_a24_gas_1e23.obj");
list.push_back("C:\\Users\\zorik\\Documents\\alpha\\models\\sgal_a24_gas_1e24.obj");
list.push_back("C:\\Users\\zorik\\Documents\\alpha\\models\\sgal_a24_gas_1e25.obj");
KFAnim(list, "C:\\Users\\zorik\\Documents\\alpha\\models\\cube2.fbx", 5);
return 0;

} 
