#include "ObjImportExport.h"
#include <boost/algorithm/string/predicate.hpp>
#include "DataToImage.h"
#include "AssimpImportExport.h"

#include <string>

//TODO (TOMER) this file has many repetitions inside. This spaghetti code should be rewritten

// GENERAL MESSAGE:
//TODO MAGIC STRINGS, SHOULD BE included in a special define header
//TODO also:  to_string(aColor.R), int2str(*apMtlCounter) is a very bad practice. CPP is not python. and file formatting should be exact

using namespace std;

namespace vivid
{  // TODO temp fix most of the functions here should be rewritten or become out of scope

    map<FACE_TYPE, char> ELEMENT_TYPE {{POLYGONS, 'f'}, {TRIANGLES, 'f'}, {LINES, 'l'}, {POINTS, 'p'}};

    color_t static Quan2Color(CColorMap aClm, coord_t aQuan) { // calls function from ColorMap.h
        return aClm.GetColor(aQuan);
    }

    bool static CompareColor(color_t aColor1, color_t aColor2) {
        return (aColor1[0] == aColor2[0] && aColor1[1] == aColor2[1] && aColor1[2] == aColor2[2]);
    }

    bool static CompareQuan(CFace aFace1, CFace aFace2) {
        return (aFace1.GetUVcoord() > aFace2.GetUVcoord());
    }
    static bool(*CompFace)(CFace, CFace) = CompareQuan;


    void WriteNewMtl(ofstream& aOBJFile, ofstream& aMTLFile, size_t * apMtlCounter, color_t aColor, coord_t aAlpha)
    {
        // TODO talk to tomer about line endings, unix vs windows.
        aMTLFile << "newmtl surf_%07d" << (*apMtlCounter) <<endl;
        aMTLFile << "Ns 96.078\nKa 1.000 1.000 1.000\n";
        aMTLFile << "Kd " + to_string(aColor[0]) + " " + to_string(aColor[1]) + " " + to_string(aColor[2]) + "\n";
        aMTLFile << "Ks 0.000 0.000 0.000\n";
        aMTLFile << "Ni 1.000000\n";
        aMTLFile << "d " + to_string(aAlpha) + "\n";
        aMTLFile << "illum 0\n";
        aMTLFile << "em 0.000000\n\n\n";
        aMTLFile.flush();

        aOBJFile << "usemtl surf_" << (*apMtlCounter) << "\n";
        aOBJFile.flush();

    }

    void WriteNewFace(ofstream& aOBJFile, CFace aFace, size_t aPointsCounter, char aWriteType)
    {
        aOBJFile << aWriteType << ' ';
        vector<size_t> face_points = aFace.GetPoints();
        for (vector<size_t>::iterator ItPoint = face_points.begin(); ItPoint != face_points.end(); ItPoint++)
        {
            aOBJFile << to_string(*ItPoint + 1 + aPointsCounter) + " "; //TODO formatting
        }
        aOBJFile << "\n";
    }

    void WriteNewPoint(ofstream& aOBJFile, CPoint aPoint)
    {
        aOBJFile << "v " << to_string(aPoint.X()) + " " + to_string(aPoint.Y()) + " " + to_string(aPoint.Z()) + "\n";
    }

    void WriteObj(ofstream& aOBJFile, ofstream& aMTLFile, CModelComponent * apMesh, size_t * apMtlCounter, size_t aPointsCounter)
    {
        aOBJFile << "o " + apMesh->GetLabel() + "\n";
        //cout << apMesh->GetPoints().begin() << endl;
        (*apMesh).GetPoints().begin();
        //write points to obj file
        CPoint temp_point = CPoint(0, 0, 0);
        vector<CPoint> Points = apMesh->GetPoints();
        vector<CFace> Faces = apMesh->GetFaces();
        for (auto it = Points.begin(); it != Points.end(); it++)
        {
            temp_point = *it; // + apMesh->getCenVector() ; //add the CenVector to return model to the original centralization.
            WriteNewPoint(aOBJFile, temp_point);; // writes the point in obj file as vertex
        }
        aOBJFile.flush();

        //sort vecFaces by color
        sort(Faces.begin(), Faces.end(), CompFace); // NlogN
        //write faces to obj file + write colors to mtl file
        color_t color = Quan2Color(apMesh->GetColorMap(), (apMesh->GetFaces())[0].GetUVcoord());
        char write_type = ELEMENT_TYPE[apMesh->GetObjType()];
        coord_t alpha = apMesh->GetOpacity();
        WriteNewMtl(aOBJFile, aMTLFile, apMtlCounter, color, alpha);
        for (auto it = Faces.begin(); it != Faces.end(); it++)
        {

            if (CompareColor(color, Quan2Color(apMesh->GetColorMap(), it->GetUVcoord()))) //if true write the face to the obj file
            {
                WriteNewFace(aOBJFile, *it, aPointsCounter, write_type);
            }
            else // we reached a new color, and we need to write it in mtl before using it
            {
                color = Quan2Color(apMesh->GetColorMap(), it->GetUVcoord());
                (*apMtlCounter)++;
                WriteNewMtl(aOBJFile, aMTLFile, apMtlCounter, color, alpha);
                // now we can write the face in the obj file
                WriteNewFace(aOBJFile, *it, aPointsCounter, write_type);
            }
        }
    }

    void ExportToObjMaterial(CModel &aModel, string aOutput){
        if (boost::ends_with(aOutput, ".obj") ) { //check if the output file ends with .obj, and delete it if it does
            aOutput.erase(aOutput.length() - 4, 4);
        }

        string lines;
        //TODO BAD!!! Y NOT STD????
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
        vector<CModelComponent> meshes = aModel.GetMeshes();
        for (auto it = meshes.begin(); it != meshes.end(); it++)
        {
//	    cout << it->GetLabel() << endl;
            WriteObj(OBJFile, MTLObj, &(*it), &mtl_counter, points_counter);
            points_counter += it->GetPoints().size();
            mtl_counter = mtl_counter + 1;
        }
        OBJFile.flush();
        MTLObj.flush();

    }


    void WriteNewFaceTexture(ofstream &arOBJFile, const CModelComponent * apMesh, CFace aFace, size_t aPointsCounter) {
        arOBJFile << ELEMENT_TYPE[apMesh->GetObjType()] << ' ';
        vector<size_t> face_points = aFace.GetPoints();
        size_t VT = apMesh->GetColorMap().GetColorIndex(aFace.GetUVcoord()) + 1;
        for (auto ItPoint = face_points.begin(); ItPoint != face_points.end(); ItPoint++)
        {
            arOBJFile << to_string(*ItPoint + 1 + aPointsCounter) + "/" + to_string(VT) + " "; //TODO formatting
        }
        arOBJFile << "\n";
    }

    void WriteMtlTexture(ofstream& aOBJFile, ofstream& aMTLFile, size_t * mtl_counter, string aTextureName, coord_t aAlpha) {
        aMTLFile << "newmtl texture_" + to_string(*mtl_counter) + "\n" + \
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

        aOBJFile << "usemtl texture_" + to_string(*mtl_counter) + "\n";
    }

    void WriteObjTexture(ofstream &aOBJFile, ofstream &aMTLFile, CModelComponent *apMesh, size_t * mtl_counter, string aTextureName,
                         coord_t aTextureSize, size_t aPointsCounter) {
        aOBJFile << "o " + apMesh->GetLabel() + "\n";
        CPoint temp_point = CPoint(0, 0, 0);
        // CPoint cen_point = apMesh->getCenVector();
        vector<CPoint> points = apMesh->GetPoints();
        //write points to obj file
        for (auto it = points.begin(); it != points.end(); it++)
        {
            temp_point = *it; // + cen_point; //add the CenVector to return model to the original
            WriteNewPoint(aOBJFile, temp_point);
            //aOBJFile << "v " + to_string(it->GetX()) + " " + to_string(it->GetY()) + " " + to_string(it->GetZ()) + "\n";
        }
        //write uv cordinates
        for(size_t i = aTextureSize; i > 0; i--){
            aOBJFile << "vt 0 " + to_string(i/aTextureSize) + "\n";
        }
        //write faces
        WriteMtlTexture(aOBJFile, aMTLFile, mtl_counter, aTextureName, apMesh->GetOpacity());
        vector<CFace> faces = apMesh->GetFaces();
        for (vector<CFace>::iterator it = faces.begin(); it != faces.end(); it++){
            WriteNewFaceTexture(aOBJFile, apMesh, *it, aPointsCounter);
        }
    }

    void ExportToObjTexture(CModel &arModel, string aOutput) {
        if (boost::ends_with(aOutput, ".obj") )  { //check if the output file ends with .obj, and delete it if it does
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
        vector<CModelComponent> meshes = arModel.GetMeshes();
        vector<vector<unsigned char>> textures;
        for (auto it = meshes.begin(); it != meshes.end(); it++) {
            textures.push_back(it->GetColorMap().GetColorTexture());
            encodePNG(string(aOutput + it->GetColorMap().GetName() + "_texture.png"), textures.back(), 1, textures.back().size() / 4);
        }
        //write obj file starter
        o << "# This 3D code was produced by Vivid \n\n\n";
        o << "mtllib " + mtl + "\n";

        mtl.erase(mtl.length() - 4, 4);

        size_t mtl_counter = 0; // will be used to count the newmtl
        size_t points_counter = 0; // will be used to count how many points the former obj wrote to the file
        for (auto it = meshes.begin(); it != meshes.end(); ++it)
        {
//	    cout << it->GetLabel() << endl;
            WriteObjTexture(o, m, &(*it), &mtl_counter, mtl + it->GetColorMap().GetName() + "_texture.png",
                            textures[mtl_counter].size() / 4, points_counter);
            points_counter += it->GetPoints().size();
            mtl_counter = mtl_counter + 1;
        }

        o.close();
        m.close();
    }

    void OBJExporter(CModel &aModel,std::string aOutPutFile, bool WithTexture){
        if (WithTexture == 0){
            ExportToObjMaterial(aModel, aOutPutFile);
        }
        else{
            ExportToObjTexture(aModel, aOutPutFile);
        }
    }

} // namespace vivid