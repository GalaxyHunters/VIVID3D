#include "ModelBuilder/Surf.h" // imports Vornoi3D.hpp
#include <vector>
#include "Utils/ReadBinFile.h"
#include "ModelBuilder/Model.h"
#include "Utils/Shapes.h"
#include "AnimationBuilder/Animation.h"
#define BOX_SIZE 20
#define HEIGHT 10
using namespace std;

//////--------------------------------------------------------------test AniVIVID--------------------------------------------------------
////int main()
///
////    CModel ROTATE;
////    ROTATE.AddMesh(Shapes::CreateCubeMesh(10, 10, 10, 0.3, 0.7, vector<double>{0,0,0}));
////    RotateAnim(ROTATE, 1000, 10, 1, "D:\\Documents\\Alpha\\Models\\Random\\CheckMerge.fbx");
////}
////
//int main()
//{
//    cout << "bru" << endl;
//    CMesh cube = CreateCubeMesh(10, 10, 10, 0.5, 0.5, vector<double>{0,0,0});
//    cube.ExportToObjTexture("../test_models/cube1");
//    return 0;
//}

////----------------------------------------------------------test arrow
//int main(){
//    CMesh arrow = CreateArrowMesh2(10, 0.1, 0.5, 0.8, 0.3, vector<double>{0,0,0}, vector<double>{0,1,1});
//    arrow.ExportToObj("/home/zohar/Documents/Vivid/test_models/arrow_test");
//    return 0;
//}


////------------------------------------------------------------test sphere and ellipsoid
int main(){
    CMesh sphere = CreateSphereMesh(10, 10, 1, vector<double>{1, 1, 1}, 0.2, 0.8, "sphere");
    sphere.ExportToObj("/home/zohar/Documents/Vivid/test_models/sphere_test");
    CMesh ellips = CreateEllipsoidMesh(10, 10,  vector<double>{4,1,1}, vector<double>{2,3,6}, vector<double>{-1,2,0}, vector<double>{2,1,0}, vector<double>{0,0,1}, 0.7, 1, "ellips1");
    ellips.ExportToObj("/home/zohar/Documents/Vivid/test_models/ellips_test");
    CModel model;
    model.AddMesh(sphere);
    model.AddMesh(ellips);

    model.ExportToObj("/home/zohar/Documents/Vivid/test_models/model_test");
}



//int main() {
//    std::cout << "starting program" << endl;
//    vector<vector<double >> points;
//    vector<bool> mask;
//    vector<coord_t> quan;
//    coord_t Vmin, Vmax;
//
////----------------------------------------------------------------------pyramid
//
//    size_t a = 0;
//    for (size_t i = -BOX_SIZE; i < BOX_SIZE; i += 2) {
//        for (size_t j = -BOX_SIZE; j < BOX_SIZE; j += 2) {
//            for (size_t z = -BOX_SIZE; z < BOX_SIZE; z += 2) {
//                points.push_back(vector<double>{(double)i, (double)j, (double)z});
//                if (z >= 0 && HEIGHT >= z) {
//                    a = HEIGHT - z;
//                } else {
//                    a = 0;
//                }
//
//                if ((z >= 0 && HEIGHT >= z) && (-a <= i && i <= a) && (-a <= j && j <= a)) {
//                    mask.push_back(true);
//                    quan.push_back(std::log(pow(10, a) + 0.0001));
//                } else {
//                    mask.push_back(false);
//                    quan.push_back(std::log(0.00001));
//                }
//            }
//        }
//    }
//
//    Vmax = *max_element(quan.begin(), quan.end());
//    Vmin = *min_element(quan.begin(), quan.end());
//
//    //----------------------------------------------------cube
//    //for (int i = 2; i > -4; i -= 2) { // make the vornoi input points, a 3d grid for all combination optionts for 2, 0, -2
//    //	for (int j = 2; j > -4; j -= 2) {
//    //		for (int z = 2; z > -4; z -= 2) {
//    //			points.push_back(Point(i, j, z));
//    //			quan.push_back(0);
//    //			mask.push_back(false);
//    //			if (i == j && j == z && z == 0) {
//    //				mask.back() = true;
//    //			}
//    //		}
//    //	}
//    //}
//    //Vmax = *max_element(quan.begin(), quan.end());
//    //Vmin = *min_element(quan.begin(), quan.end());
//
////--------------------------------------------------------------------run cube/pyramid -----------------------------------------------------------------------
//    CSurf surf = CSurf(points, mask, quan, Vmin, Vmax);
//    surf.SmoothSurf();
//    CMesh mesh = surf.ToMesh("vivid_3d_obj", 1.0);
//    mesh.Decimation(0.5, 0.4);
//    mesh.ExportToObjTexture("../temp/Pyramid.obj");
//}
//	//CModel model = CModel(vector<CMesh>{mesh});
//	//model.ExportToObj("..\\test_models\\testCode_CModel");
//	//cout << "blalala";
	//----------------------------------------------------test read bin file -----------------------------------------------------------------------------
	//ModelData temp = ReadBin("D:\\alpa\\bin_files\\gal_07_0.2Rvie.bin");
	//Vmax = *max_element(temp.quan.begin(), temp.quan.end());
	//Vmin = *min_element(temp.quan.begin(), temp.quan.end());

	//Surf surf = surf.createSurf(temp.points, temp.mask, temp.quan, Vmin, Vmax);
	//surf.smoothSurf();
	//surf.exportToObj("D:\\alpa\\models\\check_code", "vivid_3d_obj", 1.0);

	//------------------------------------------------------print gal_07 Rvir_1.5------------------------------------------------------------------------------
	//ModelDataMultipleMasks temp = ReadBinMultipleMasks("D:\\alpa\\bin_files\\gal_07_1.5Rvir_--1e-25.bin");

	//Vmax = 18.4206808;
	//Vmin = 5.70378245;

	////Vmax = *max_element(temp.quan.begin(), temp.quan.end());
	////Vmin = *min_element(temp.quan.begin(), temp.quan.end());

	//vector<Surf> surfacesHigh = Surf::createSurf(temp.points, temp.mask, temp.quan, Vmin, Vmax);

	////temp = ReadBinMultipleMasks("D:\\alpa\\bin_files\\gal_07_1.5Rvir_++1e-26.bin");

	////vector<Surf> surfacesLow = Surf::createSurf(temp.points, temp.mask, temp.quan, Vmin, Vmax);

	//vector<string> labels(surfacesHigh.size());
	//labels.clear();
	//labels.push_back("1e-25"); labels.push_back("1e-24");
	//labels.push_back("1e-23"); labels.push_back("1e-22"); labels.push_back("1e-21");
	////for (size_t i = 0; i < surfacesHigh.size(); i++) {
	////	surfacesHigh[i].exportToObj("D:\\alpa\\models\\gal_07_fullRvir_noSmooth\\gal_07_" + labels[i+5], "gal_07_" + labels[i+5], 0.35 + ((i+5)*0.05));
	////	//surfacesLow[i].exportToObj("D:\\alpa\\models\\gal_07_fullRvir_noSmooth\\gal_07_" + labels[i], "gal_07" + labels[i], 0.35 + ((i)*0.05));
	////}
	//for (size_t i = 0; i < surfacesHigh.size(); i++) {
	//	surfacesHigh[i].smoothSurf();
	//	surfacesHigh[i].exportToObj("D:\\alpa\\models\\gal_07_fullRvir_smooth\\gal_07_lowColor_" + labels[i], "gal_07_" + labels[i], 1);
	//}
//
////	//------------------------------------------------------------------------------------------------------------------------
////
////	Vmax = 18.4206808;
////	Vmin = 5.70378245;
////
////	ModelData temp2 = ReadBin("D:\\alpa\\bin_files\\gal_07_1.5Rvir_1e-26.bin");
////
////	Surf surf = Surf::createSurf(temp2.points, temp2.mask, temp2.quan, Vmin, Vmax);
////	//surf.smoothSurf();
////	surf.exportToObj("D:\\alpa\\models\\gal_07_fullRvir_smooth\\gal_07_lowColor_1e-26", "Vela_07_1e-26", 1);
////	cout << "1e-26 done" << endl;
////
////	temp2 = ReadBin("D :\\alpa\\bin_files\\gal_07_1.5Rvir_1e-27.bin");
////	surf = surf.createSurf(temp2.points, temp2.mask, temp2.quan, Vmin, Vmax);
////	//surf.smoothSurf();
////	surf.exportToObj("D:\\alpa\\models\\gal_07_fullRvir_smooth\\gal_07_lowColor_1e-27", "Vela_07_1e-27", 1);
////	cout << "1e-27 done" << endl;
////////////////////////////////////////////////////////////debug triangulation test///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	//string line;
//	//ifstream test_file("D:\\alpa\\models\\cow.obj",  ios::in);
//	//CMesh mesh;
//	//vector<CPoint> mesh_points;
//	//vector<CIndexedFace> mesh_faces;
//	//CPoint point;
//	//std::string::size_type sz;
//	//size_t x, y, z;
//	//while (getline(test_file, line))
//	//{
//	//	if (line.at(0) == 'v') {
//	//		line = line.substr(2);
//	//		point.SetX(std::stod(line, &sz));
//	//		point.SetY(std::stod(line = line.substr(sz), &sz));
//	//		point.SetZ(std::stod(line.substr(sz)));
//	//		mesh_points.push_back(CPoint(point));
//	//	}
//	//	if (line.at(0) == 'f') {
//	//		line = line.substr(2);
//	//		x = std::stoi(line, &sz) - 1;
//	//		y = std::stoi(line = line.substr(sz), &sz) - 1;
//	//		z = std::stoi(line.substr(sz)) - 1;
//	//		mesh_faces.push_back(CIndexedFace(x, y, z, 0));
//	//	}
//	//}
//	//test_file.close();
//	//mesh.SetAlpha(1);
//	//mesh.SetFaces(mesh_faces);
//	//mesh.SetLabel("test");
//	//mesh.SetPoints(mesh_points);
//	//mesh.Decimation(0.3, 0.01);
//	//mesh << ("D:\\alpa\\models\\testCode_decimation_cow.obj");
////}
//
