#include "Mesh.h"
#include <stdio.h>



using namespace boost::algorithm;


// TODO: to place the methods in order and in substructure

CMesh::~CMesh() {}

CMesh::CMesh(const CMesh &mesh){
    mAlpha = mesh.mAlpha;
    mFaces = mesh.mFaces;
    mLabel = mesh.mLabel;
    mPoints = mesh.mPoints;
    mCenVector = mesh.mCenVector;

}


CMesh::CMesh(vector<CPoint> aPoints, vector<CIndexedFace> aFaces, string aLabel, coord_t aAlpha, CPoint aCenVector) : mPoints(aPoints), mFaces(aFaces), mLabel(aLabel), mAlpha(aAlpha), mCenVector(aCenVector) {}

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


void CMesh::WriteNewMtl(ofstream& aOBJFile, ofstream& aMTLFile, size_t * apMtlCounter, color_t aColor, coord_t aAlpha)
{
	aMTLFile << "newmtl surf_" + to_string(*apMtlCounter) + "\n" + \
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

void CMesh::WriteNewFace(ofstream& aOBJFile, CIndexedFace aFace) 
{
	aOBJFile << "f ";
	vector<size_t> face_points = aFace.GetPoints();
	for (vector<size_t>::iterator ItPoint = face_points.begin(); ItPoint != face_points.end(); ItPoint++)
	{
		aOBJFile << int2str(*ItPoint + 1) + " ";
	}
	aOBJFile << "\n";
}

void CMesh::WriteObj(ofstream& aOBJFile, ofstream& aMTLFile, size_t * apMtlCounter) 
{
	aOBJFile << "o " + mLabel + "\n";
	//write points to obj file
	CPoint temp_point = CPoint(0, 0, 0);
	for (vector<CPoint>::iterator it = mPoints.begin(); it != mPoints.end(); it++)
	{
        temp_point = *it + mCenVector ; //sdd the CenVector to return model to the original centralization.
        aOBJFile << temp_point; // writes the point in obj file as vertex
	}
	//sort vecFaces by color
	sort(mFaces.begin(), mFaces.end(), CompFace); // NlogN
	//write faces to obj file + write colors to mtl file
	color_t color = Quan2Color(mFaces[0].GetColor());
	WriteNewMtl(aOBJFile, aMTLFile, apMtlCounter, color, mAlpha);
	for (vector<CIndexedFace>::iterator it = mFaces.begin(); it != mFaces.end(); it++)
	{
		if (CompareColor(color, Quan2Color(it->GetColor()))) //if true write the face to the obj file
		{
			WriteNewFace(aOBJFile, *it);
		}
		else // we reached a new color, and we need to write it in mtl before using it
		{
			color = Quan2Color(it->GetColor());
			(*apMtlCounter)++;
			WriteNewMtl(aOBJFile, aMTLFile, apMtlCounter, color, mAlpha);
			// now we can write the face in the obj file
			WriteNewFace(aOBJFile, *it);
		}
	}
}

void CMesh::ExportToObj(string aOutput){
	if (ends_with(aOutput, ".obj")) { //check if the output file ends with .obj, and delete it if it does
		aOutput.erase(aOutput.length() - 4, 4);
	}
	string lines;
    #if defined(_WIN32)
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
	//write obj file starter
	o << "# This 3D code was produced by Vivid \n\n\n";
	o << "mtllib " + mtl + "\n";

	size_t mtl_counter = 0;
	WriteObj(o, m, &mtl_counter);
	o.close();
	m.close();
}

void CMesh::WriteNewFaceTexture(ofstream &aOBJFile, CIndexedFace aFace){
    aOBJFile << "f ";
    vector<size_t> face_points = aFace.GetPoints();
    size_t VT = GetColorIndex( aFace.GetColor())+1;
    for (vector<size_t>::iterator ItPoint = face_points.begin(); ItPoint != face_points.end(); ItPoint++)
    {
        aOBJFile << int2str(*ItPoint + 1) +"/" + int2str(VT) + " ";
    }
    aOBJFile << "\n";
}

void CMesh::WriteMtlTexture(ofstream &aOBJFile, ofstream &aMTLFile, string aTextureName, coord_t aAlpha){
    aMTLFile << "newmtl texture_0\n" \
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

    aOBJFile << "usemtl texture_0\n";
}

void CMesh::WriteObjTexture(ofstream &aOBJFile, ofstream &aMTLFile, string aTextureName, coord_t aTextureSize){
    aOBJFile << "o " + mLabel + "\n";
    CPoint temp_point = CPoint(0, 0, 0);
    //write points to obj file
    for (vector<CPoint>::iterator it = mPoints.begin(); it != mPoints.end(); it++)
    {
        temp_point = *it + mCenVector ; //sdd the CenVector to return model to the original centralization.
        aOBJFile << temp_point; // writes the point in obj file as vertex
    }
    //write uv cordinates
    for(size_t i = aTextureSize; i > 0; i--){
        aOBJFile << "vt 0 " + to_string(i/aTextureSize) + "\n";
    }
    //write faces
    WriteMtlTexture(aOBJFile, aMTLFile, aTextureName, mAlpha);
    for (vector<CIndexedFace>::iterator it = mFaces.begin(); it != mFaces.end(); it++){
        WriteNewFaceTexture(aOBJFile, *it);
    }
}

void CMesh::ExportToObjTexture(string aOutput){
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

    WriteObjTexture(o, m, mtl + "_texture.png", texture.size()/4);

    o.close();
    m.close();
}

void CMesh::Decimation(coord_t aVerticlePercent, coord_t aMaxError)
{
    //check input valdilty
    if( aVerticlePercent < 0 || aVerticlePercent > 1){
        throw "input error, DecimationPercent must be between 0 and 1";
    }
    if( aMaxError < 0 || aMaxError > 1){
        throw "input error, MaxError must be between 0 and 1";
    }

	//call decimation from External
	int targetVerticesN = int(aVerticlePercent * mPoints.size());
	int targetTrianglesN = int(aVerticlePercent * mFaces.size());
	pair<vector<CPoint>, vector<CIndexedFace> > temp = DecimateMesh(mPoints, GetFacesAsTriangles(), targetVerticesN, targetTrianglesN, aMaxError);
	mPoints = get<0>(temp);
	mFaces = get<1>(temp);
}

vector<CIndexedFace> CMesh::GetFacesAsTriangles() {
    vector<CIndexedFace> aTriangles = vector<CIndexedFace>();
	for (vector<CIndexedFace>::iterator fIt = mFaces.begin(); fIt != mFaces.end(); fIt++) {
		for (size_t i = 1; i < fIt->GetPoints().size()-1; i++) { // go over all the vertices from 1 to n-1 and connect them with vertice 0 to create triangles 
            aTriangles.push_back(CIndexedFace((*fIt)[0], (*fIt)[i], (*fIt)[i + 1], fIt->GetColor()));
		}
	}
	return aTriangles;
}
//geters seters

string CMesh::GetLabel() { return mLabel; }
coord_t CMesh::GetAlpha() { return mAlpha; }
vector<CPoint> CMesh::GetPoints() { return mPoints; }
vector<CIndexedFace> CMesh::GetFaces() { return mFaces; }
void CMesh::SetFaces(vector<CIndexedFace> aFaces) { mFaces = aFaces; }
void CMesh::SetPoints(vector<CPoint> aPoints) { mPoints = aPoints; }
void CMesh::SetLabel(string aLabel) { mLabel = aLabel; }
void CMesh::SetAlpha(coord_t aAlpha){
    //check input valdilty
    if(aAlpha > 1 || aAlpha < 0){
        throw "Alpha must be between 0 and 1";
    }
    mAlpha = aAlpha;
}
void CMesh::setCenVector(const CPoint &vector){ mCenVector = vector; }
CPoint CMesh::getCenVector() { return mCenVector; }





void CMesh::TransformMesh(coord_t const aTrans[3][3]){

    double px,py,pz;
    for (vector<CPoint>::iterator it = mPoints.begin(); it != mPoints.end(); it++)
    {

        px=it->GetX(); py=it->GetY(); pz=it->GetZ();

        it->SetX( aTrans[0][0]*px + aTrans[0][1]*py + aTrans[0][2]*pz );
        it->SetY( aTrans[1][0]*px + aTrans[1][1]*py + aTrans[1][2]*pz );
        it->SetZ( aTrans[2][0]*px + aTrans[2][1]*py + aTrans[2][2]*pz );

        // We should change all to vectorized operators.
        // May the god of compilers forgive us all for our sins.
    }
}

void CMesh::RotatewMesh(CPoint aNormVec, double aRadAngel){
    // Trig operations are expansive
    auto cos_a = cos(aRadAngel);
    auto sin_a = sin(aRadAngel);
    // auto one_min_cos_a = 1-cos_a; for optimization it's better but it's less readable...
    auto nx = aNormVec.GetX();
    auto ny = aNormVec.GetY();
    auto nz = aNormVec.GetZ();

    coord_t const rotation_mat[3][3] = {
            cos_a + nx*nx*(1-cos_a),        nx*ny*(1-cos_a) - nz*sin_a,     nx*nz*(1-cos_a) + ny*sin_a,

            ny*nx*(1-cos_a) + nz*sin_a,     cos_a + ny*ny*(1-cos_a),        ny*nz*(1-cos_a) - nx*sin_a,

            nz*nx*(1-cos_a) - ny*sin_a,     nz*ny*(1-cos_a) + nx*sin_a,     cos_a + nz*nz*(1-cos_a),
    };
    TransformMesh(rotation_mat);
}

void CMesh::MoveMesh(CPoint aDirectionVec){
    auto x_movement = aDirectionVec.GetX();
    auto y_movement = aDirectionVec.GetY();
    auto z_movement = aDirectionVec.GetZ();
    for (vector<CPoint>::iterator it = mPoints.begin(); it != this->mPoints.end(); it++)
    {
        it->SetX(x_movement+it->GetX());
        it->SetY(y_movement+it->GetY());
        it->SetZ(z_movement+it->GetZ());
        // The only reason I let it pass like that, is because this is an inline code.
        // We should change all to vectorized operators.
        // May the god of compilers forgive us all for our sins.
    }
}

void CMesh::ScaleMesh(CPoint aScaleVec){
    auto x_scale = aScaleVec.GetX();
    auto y_scale = aScaleVec.GetY();
    auto z_scale = aScaleVec.GetZ();
    for (vector<CPoint>::iterator it = this->mPoints.begin(); it != this->mPoints.end(); it++)
    {
        it->SetX(x_scale*it->GetX());
        it->SetY(y_scale*it->GetY());
        it->SetZ(z_scale*it->GetZ());
        // The only reason I let it pass like that, is because this is an inline code.
        // We should change all to vectorized operators.
        // May the god of compilers forgive us all for our sins.
    }
}