#include "Model.h"


using namespace boost::algorithm;

CModel::CModel() {}

CModel::CModel(vector<CMesh> aMeshes) {
	this->mMeshes = aMeshes;
}

Color_t static Quan2Color(float aQuan) {
	ColorMap output;
	return output.GetColor(aQuan);
}

bool static CompareColor(Color_t aColor1, Color_t aColor2) {
	return (aColor1.R == aColor2.R && aColor1.G == aColor2.G && aColor1.B == aColor2.B);
}

bool static CompareQuan(CIndexedFace aFace1, CIndexedFace aFace2) {
	return (aFace1.GetColor() > aFace2.GetColor());
}
static bool(*CompFace)(CIndexedFace, CIndexedFace) = CompareQuan;


void CModel::WriteNewMtl(ofstream& aOBJFile, ofstream& aMTLFile, size_t * apMtlCounter, Color_t aColor, float aAlpha)
{
	aMTLFile << "newmtl surf_" + int2str(*apMtlCounter) + "\n" + \
		"Ns 96.078\n" + \
		"Ka 1.000 1.000 1.000 \n" + \
		"Kd " + to_string(aColor.R) + " " + to_string(aColor.G) + " " + to_string(aColor.B) + "\n" + \
		"Ks 0.000 0.000 0.000\n" + \
		"Ni 1.000000\n" + \
		"d " + to_string(aAlpha) + "\n" + \
		"illum 0\n" + \
		"em 0.000000\n\n\n";
	aOBJFile << "usemtl surf_" + int2str(*apMtlCounter) + "/n";
}

void CModel::WriteNewFace(ofstream& aOBJFile, CIndexedFace aFace) 
{
	aOBJFile << "f ";
	vector<size_t> face_points = aFace.GetPoints();
	for (vector<size_t>::iterator ItPoint = face_points.begin(); ItPoint != face_points.end(); ItPoint++)
	{
		aOBJFile << int2str(*ItPoint + 1) + " ";
	}
	aOBJFile << "\n";
}

void CModel::WriteObj(ofstream& aOBJFile, ofstream& aMTLFile, CMesh * apMesh, size_t * apMtlCounter, size_t aPointsCounter) 
{
	aOBJFile << "o" + apMesh->GetLabel() + "/n";
	//write points to obj file
	for (vector<CPoint>::iterator it = apMesh->GetPoints().begin(); it != apMesh->GetPoints().end(); it++) 
	{
		aOBJFile << "v " + to_string(it->GetX()) + " " + to_string(it->GetY()) + " " + to_string(it->GetZ()) + "\n";
	}
	//sort vecFaces by color
	sort(apMesh->GetFaces().begin(), apMesh->GetFaces().end(), CompFace); // NlogN
	//write faces to obj file + write colors to mtl file
	Color_t color = Quan2Color(apMesh->GetFaces()[0].GetColor());
	WriteNewMtl(aOBJFile, aMTLFile, apMtlCounter, color, apMesh->GetAlpha());
	for (vector<CIndexedFace>::iterator it = apMesh->GetFaces().begin(); it != apMesh->GetFaces().end(); it++) 
	{
		if (CompareColor(color, Quan2Color(it->GetColor()))) //if true write the face to the obj file
		{
			WriteNewFace(aOBJFile, *it);
		}
		else // we reached a new color, and we need to write it in mtl before using it
		{
			color = Quan2Color(it->GetColor());
			(*apMtlCounter)++;
			WriteNewMtl(aOBJFile, aMTLFile, apMtlCounter, color, apMesh->GetAlpha());
			// now we can write the face in the obj file
			WriteNewFace(aOBJFile, *it);
		}
	}
}

void CModel::operator<<(string aOutput) {
	if (ends_with(aOutput, ".obj")) { //check if the output file ends with .obj, and delete it if it does
		aOutput.erase(aOutput.length() - 4, 4);
	}
	ofstream OBJFile; // the obj file
	OBJFile.open(aOutput + ".obj");
	ofstream MTLObj; //the mtl file
	MTLObj.open(aOutput + ".mtl");
	string mtl = aOutput;
	while (mtl.find('\\') != string::npos) {
		mtl = mtl.substr(mtl.find('\\'), string::npos);
	}
	//write obj file starter
	OBJFile << "# This 3D code was produced by Vivid /n/n/n";
	OBJFile << "mtllib" + mtl + "/n";
	
	size_t mtl_counter = 0; // will be used to count the newmtl
	size_t points_counter = 0; // will be used to count how many points the former obj wrote to the file
	for (vector<CMesh>::iterator ItMesh = this->mMeshes.begin(); ItMesh != this->mMeshes.end(); ItMesh++) 
	{
		WriteObj(OBJFile, MTLObj, &(*ItMesh), &mtl_counter, points_counter);
		points_counter += ItMesh->GetPoints().size();
	}
}

void CModel::AddMesh(CMesh aMesh) {
	this->mMeshes.push_back(aMesh);
}


CModel::~CModel(){}