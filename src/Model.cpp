#include "Model.h"


using namespace boost::algorithm;

CModel::CModel() { this->mMeshes = vector<CMesh>();}

CModel::CModel(vector<CMesh> aMeshes) {
	this->mMeshes = aMeshes;
}

CModel::CModel(vector<CSurf> aSurfs, string aLabel, cord_t aAlpha){
    this->mMeshes = vector<CMesh>();
    for (vector<CSurf>::iterator it = aSurfs.begin(); it != aSurfs.end(); it++){
        this->mMeshes.push_back((*it).ToMesh(aLabel, aAlpha));
    }
}

Color_t static Quan2Color(cord_t aQuan) { // calls function from ColorMap.h
	return GetColor(aQuan);
}

bool static CompareColor(Color_t aColor1, Color_t aColor2) {
	return (aColor1.R == aColor2.R && aColor1.G == aColor2.G && aColor1.B == aColor2.B);
}

bool static CompareQuan(CIndexedFace aFace1, CIndexedFace aFace2) {
	return (aFace1.GetColor() > aFace2.GetColor());
}
static bool(*CompFace)(CIndexedFace, CIndexedFace) = CompareQuan;


void CModel::WriteNewMtl(ofstream& aOBJFile, ofstream& aMTLFile, size_t * apMtlCounter, Color_t aColor, cord_t aAlpha)
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
	aOBJFile << "usemtl surf_" + int2str(*apMtlCounter) + "\n";
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
	aOBJFile << "o " + apMesh->GetLabel() + "\n";
	//cout << apMesh->GetPoints().begin() << endl;
    (*apMesh).GetPoints().begin();
	//write points to obj file
	vector<CPoint> Points = apMesh->GetPoints();
    vector<CIndexedFace> Faces = apMesh->GetFaces();
	for (vector<CPoint>::iterator it = Points.begin(); it != Points.end(); it++)
	{
		aOBJFile << "v " + to_string(it->GetX()) + " " + to_string(it->GetY()) + " " + to_string(it->GetZ()) + "\n";
	}
	//sort vecFaces by color
	sort(Faces.begin(), Faces.end(), CompFace); // NlogN
	//write faces to obj file + write colors to mtl file
	Color_t color = Quan2Color((apMesh->GetFaces())[0].GetColor());
	WriteNewMtl(aOBJFile, aMTLFile, apMtlCounter, color, apMesh->GetAlpha());
	for (vector<CIndexedFace>::iterator it = Faces.begin(); it != Faces.end(); it++)
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

void CModel::ExportToObj(string aOutput){
	if (ends_with(aOutput, ".obj")) { //check if the output file ends with .obj, and delete it if it does
		aOutput.erase(aOutput.length() - 4, 4);
	}

    string lines;
    #if defined(_WIN32)
	    lines = '\\';
    #elif defined(_linux_)
        lines = '/';
    #elif defined __APPLE__
        lines = '/';
    #endif

	ofstream OBJFile; // the obj file
	OBJFile.open(aOutput + ".obj");
	ofstream MTLObj; //the mtl file
	MTLObj.open(aOutput + ".mtl");
	string mtl = aOutput;
    while (mtl.find(lines) != string::npos) {
        mtl = mtl.substr(mtl.find(lines) + 1, string::npos);
	}
	//write obj file starter
	OBJFile << "# This 3D code was produced by Vivid \n\n\n";
	OBJFile << "mtllib " + mtl + ".mtl\n";
	
	size_t mtl_counter = 0; // will be used to count the newmtl
	size_t points_counter = 0; // will be used to count how many points the former obj wrote to the file
	for (vector<CMesh>::iterator ItMesh = this->mMeshes.begin(); ItMesh != this->mMeshes.end(); ItMesh++) 
	{
		WriteObj(OBJFile, MTLObj, &(*ItMesh), &mtl_counter, points_counter);
		points_counter += ItMesh->GetPoints().size();
	}
}

///////////////////////////////////////////////////export as texture/////////////////////////////////////////////////////////////////////////////////////////////



void CModel::WriteNewFaceTexture(ofstream &aOBJFile, size_t uv_count, CIndexedFace aFace){
    aOBJFile << "f ";
    vector<size_t> face_points = aFace.GetPoints();
    size_t VT = GetColorIndex( aFace.GetColor())+1+uv_count;
    for (vector<size_t>::iterator ItPoint = face_points.begin(); ItPoint != face_points.end(); ItPoint++)
    {
        aOBJFile << int2str(*ItPoint + 1) +"/" + int2str(VT) + " ";
    }
    aOBJFile << "\n";
}

void CModel::WriteMtlTexture(ofstream &aOBJFile, ofstream &aMTLFile, size_t & apMtlCounter, string aTextureName, cord_t aAlpha){
    aMTLFile << "newmtl texture_"+ apMtlCounter + "\n" \
		"Ns 96.078\n"  \
		"Ka 1.000 1.000 1.000 \n"  \
		"Kd 1.000 1.000 1.000 \n"  \
		"Ks 0.000 0.000 0.000\n" \
		"Ni 1.000000\n"  \
		"d " + to_string(aAlpha) + "\n" \
		"illum 0\n" \
		"em 0.000000\n" \
		"map_Ka " + aTextureName + "\n" \
        "map_Kd " + aTextureName + "\n\n\n";

    aOBJFile << "usemtl texture_0" + apMtlCounter + "\n";
}

void CModel::WriteObjTexture(ofstream &aOBJFile, ofstream &aMTLFile, CMesh * apMesh, size_t aPointsCounter, size_t &apMtlCounter, string aTextureName, cord_t aTextureSize){
    aOBJFile << "o " + apMesh->GetLabel() + "\n";
    //write points to obj file
    vector<CPoint> points = apMesh->GetPoints();
    vector<CIndexedFace> faces = apMesh->GetFaces();
    for (vector<CPoint>::iterator it = points.begin(); it != points.end(); it++)
    {
        aOBJFile << "v " + to_string(it->GetX()) + " " + to_string(it->GetY()) + " " + to_string(it->GetZ()) + "\n";
    }
    //write uv cordinates
    for(size_t i = aTextureSize; i > 0; i--){
        o << "vt 0 " + to_string(i/aTextureSize) + "\n";
    }
    //write faces
    WriteMtlTexture(aOBJFile, aMTLFile, &apMtlCounter, aTextureName, apMesh->GetAlpha());
    uv_count = apMtlCounter * aTextureSize;
    for (vector<CIndexedFace>::iterator it = faces.begin(); it != faces.end(); it++){
        WriteNewFaceTexture(aOBJFile, uv_count, *it);
    }
}

void CModel::ExportToObjTexture(string aOutput){
    if (ends_with(aOutput, ".obj")) { //check if the output file ends with .obj, and delete it if it does
        aOutput.erase(aOutput.length() - 4, 4);
    }
    string lines;
#if defined(_WIN32)
    lines = '\\';
#elif defined(_linux_)
    lines = '/';
#elif defined __APPLE__
    lines = '/';
#endif

    ofstream o; // the obj file
    o.open(aOutput + ".obj");
    ofstream m; //the mtl file
    m.open(aOutput + ".mtl");
    string mtl = aOutput + ".mtl";
    while (mtl.find(lines) != string::npos) {
        mtl = mtl.substr(mtl.find(lines) + 1, string::npos);
    }
    //write texture
    vector<unsigned char> texture;
    texture = GetColorTexture();
    encodePNG((aOutput + "_texture.png").c_str(), texture, 1, texture.size()/4);
    //write obj file starter
    o << "# This 3D code was produced by Vivid \n\n\n";
    o << "mtllib " + mtl + "\n";

    mtl.erase(mtl.length() - 4, 4);

    size_t points_counter = 0; // will be used to count how many points the former obj wrote to the file
    size_t mtl_counter = 0
    for (vector<CMesh>::iterator ItMesh = this->mMeshes.begin(); ItMesh != this->mMeshes.end(); ItMesh++) {
        this->WriteObjTexture(o, m, &(*ItMesh), points_counter, mtl_counter, mtl + "_texture.png", texture.size() / 4);
        points_counter += ItMesh->GetPoints().size();
        mtl_counter += 1;
    }

    o.close();
    m.close();
}

///////////////////////////////////////////////////////////////////

void CModel::AddMesh(CMesh aMesh) {
	this->mMeshes.push_back(aMesh);
}

void CModel::AddSurf(CSurf aSurf, string aLabel, cord_t aAlpha){
    this->mMeshes.push_back(aSurf.ToMesh(aLabel, aAlpha));
}


CModel::~CModel(){}