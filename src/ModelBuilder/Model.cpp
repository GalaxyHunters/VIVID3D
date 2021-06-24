#include "Model.h"

using namespace vivid;

using namespace boost::algorithm;
using namespace std;

CModel::CModel(vector<CSurf> aSurfs, string aLabel, coord_t aAlpha){
    mMeshes = vector<CMesh>();
    for (auto it = aSurfs.begin(); it != aSurfs.end(); it++){
        mMeshes.push_back((*it).ToMesh(aLabel, aAlpha));
    }
}

color_t static Quan2Color(coord_t aQuan) { // calls function from ColorMap.h
	return GetColor(aQuan);
}

bool static CompareColor(color_t aColor1, color_t aColor2) {
	return (aColor1.R == aColor2.R && aColor1.G == aColor2.G && aColor1.B == aColor2.B);
}

bool static CompareQuan(CIndexedFace aFace1, CIndexedFace aFace2) {
	return (aFace1.GetColor() > aFace2.GetColor());
}
static bool(*CompFace)(CIndexedFace, CIndexedFace) = CompareQuan;


void CModel::WriteNewMtl(ofstream& aOBJFile, ofstream& aMTLFile, size_t * apMtlCounter, color_t aColor, coord_t aAlpha)
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

    aOBJFile.flush();
    aMTLFile.flush();
}

void CModel::WriteNewFace(ofstream& aOBJFile, CIndexedFace aFace, size_t aPointsCounter)
{
	aOBJFile << "f ";
	vector<size_t> face_points = aFace.GetPoints();
	for (vector<size_t>::iterator ItPoint = face_points.begin(); ItPoint != face_points.end(); ItPoint++)
	{
		aOBJFile << int2str(*ItPoint + 1 + aPointsCounter) + " ";
	}
	aOBJFile << "\n";
}

void CModel::WriteObj(ofstream& aOBJFile, ofstream& aMTLFile, CMesh * apMesh, size_t * apMtlCounter, size_t aPointsCounter)
{
	aOBJFile << "o " + apMesh->GetLabel() + "\n";
	//cout << apMesh->GetPoints().begin() << endl;
    (*apMesh).GetPoints().begin();
	//write points to obj file
	CPoint temp_point = CPoint(0, 0, 0);
	vector<CPoint> Points = apMesh->GetPoints();
    vector<CIndexedFace> Faces = apMesh->GetFaces();
	for (auto it = Points.begin(); it != Points.end(); it++)
	{
        temp_point = *it + apMesh->getCenVector() ; //add the CenVector to return model to the original centralization.
        aOBJFile << temp_point; // writes the point in obj file as vertex
	}
    aOBJFile.flush();

	//sort vecFaces by color
	sort(Faces.begin(), Faces.end(), CompFace); // NlogN
	//write faces to obj file + write colors to mtl file
	color_t color = Quan2Color((apMesh->GetFaces())[0].GetColor());
	WriteNewMtl(aOBJFile, aMTLFile, apMtlCounter, color, apMesh->GetAlpha());
	for (auto it = Faces.begin(); it != Faces.end(); it++)
	{
		if (CompareColor(color, Quan2Color(it->GetColor()))) //if true write the face to the obj file
		{
			WriteNewFace(aOBJFile, *it, aPointsCounter);
		}
		else // we reached a new color, and we need to write it in mtl before using it
		{
			color = Quan2Color(it->GetColor());
			(*apMtlCounter)++;
			WriteNewMtl(aOBJFile, aMTLFile, apMtlCounter, color, apMesh->GetAlpha());
			// now we can write the face in the obj file
			WriteNewFace(aOBJFile, *it, aPointsCounter);
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
    #elif defined(__linux__)
        lines = '/';
    #elif defined __APPLE__
        lines = '/';
    #endif

	ofstream OBJFile (aOutput + ".obj"); // obj file  //TODO naming
	ofstream MTLObj  (aOutput + ".mtl");  // mtl file
//  TODO if (OBJFile.is_open())  MTLObj.is_open()  https://www.cplusplus.com/doc/tutorial/files/

	string mtl = aOutput;
    while (mtl.find(lines) != string::npos) {
        mtl = mtl.substr(mtl.find(lines) + 1, string::npos);
	}
	//write obj file starter
	OBJFile << "# This 3D code was produced by Vivid \n\n\n";
	OBJFile << "mtllib " + mtl + ".mtl\n";
	
	size_t mtl_counter = 0; // will be used to count the newmtl
	size_t points_counter = 0; // will be used to count how many points the former obj wrote to the file
	for (auto it = mMeshes.begin(); it != mMeshes.end(); it++)
	{
//	    cout << it->GetLabel() << endl;
		WriteObj(OBJFile, MTLObj, &(*it), &mtl_counter, points_counter);
		points_counter += it->GetPoints().size();
		mtl_counter = mtl_counter + 1;
	}
    OBJFile.flush();
    MTLObj.flush();

}


void CModel::WriteNewFaceTexture(ofstream &aOBJFile, CIndexedFace aFace, size_t aPointsCounter) {
    aOBJFile << "f ";
    vector<size_t> face_points = aFace.GetPoints();
    size_t VT = GetColorIndex( aFace.GetColor())+1;
    for (auto ItPoint = face_points.begin(); ItPoint != face_points.end(); ItPoint++)
    {
        aOBJFile << int2str(*ItPoint + 1 + aPointsCounter) +"/" + int2str(VT) + " ";
    }
    aOBJFile << "\n";
}

void CModel::WriteMtlTexture(ofstream& aOBJFile, ofstream& aMTLFile, size_t * mtl_counter, string aTextureName, coord_t aAlpha) {
        aMTLFile << "newmtl texture_" + int2str(*mtl_counter) + "\n" + \
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

        aOBJFile << "usemtl texture_" + int2str(*mtl_counter) + "\n";
}

void CModel::WriteObjTexture(ofstream &aOBJFile, ofstream &aMTLFile, CMesh *aMesh, size_t * mtl_counter, string aTextureName,
                             coord_t aTextureSize, size_t aPointsCounter) {
    aOBJFile << "o " + aMesh->GetLabel() + "\n";
    //write points to obj file
    for (auto it = aMesh->GetPoints().begin(); it != aMesh->GetPoints().end(); it++)
    {
        aOBJFile << "v " + to_string(it->GetX()) + " " + to_string(it->GetY()) + " " + to_string(it->GetZ()) + "\n";
    }
    //write uv cordinates
    for(size_t i = aTextureSize; i > 0; i--){
        aOBJFile << "vt 0 " + to_string(i/aTextureSize) + "\n";
    }
    //write faces
    WriteMtlTexture(aOBJFile, aMTLFile,  mtl_counter, aTextureName, aMesh->GetAlpha());
    for (auto it = aMesh->GetFaces().begin(); it != aMesh->GetFaces().end(); it++){
        WriteNewFaceTexture(aOBJFile, *it, aPointsCounter);
    }
}

void CModel::ExportToObjTexture(string aOutput) {
    if (ends_with(aOutput, ".obj")) { //check if the output file ends with .obj, and delete it if it does
        aOutput.erase(aOutput.length() - 4, 4);
    }
    string lines;
    #if defined(_WIN32)                                     //TODO this is ugly, change this
        lines = '\\';
    #elif defined(__linux__)
        lines = '/';
        #elif defined(__APPLE__)
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

    size_t mtl_counter = 0; // will be used to count the newmtl
    size_t points_counter = 0; // will be used to count how many points the former obj wrote to the file
    for (auto ItMesh = mMeshes.begin(); ItMesh != mMeshes.end(); ++ItMesh)
    {
//	    cout << ItMesh->GetLabel() << endl;
        WriteObj(o, m, &(*ItMesh), &mtl_counter, points_counter);
        points_counter += ItMesh->GetPoints().size();
        mtl_counter = mtl_counter + 1;
    }

    o.close();
    m.close();
}


void CModel::AddMesh(CMesh aMesh) {
	mMeshes.push_back(aMesh);
}

void CModel::AddSurf(CSurf aSurf, string aLabel, coord_t aAlpha){
    mMeshes.push_back(aSurf.ToMesh(aLabel, aAlpha));
}


CModel::~CModel() = default;