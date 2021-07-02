#include <vector>
#include "Utils/ReadBinFile.h"
#include "ModelBuilder/Model.h"
#include "ModelBuilder/Shapes.h"
#include "ImportAndExport/FBXImportExport.h"
#include "ModelBuilder/Surface.h"
#include "ModelBuilder/Point.h"


//// TODO TOMER YYYYYY!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include "../lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/Voronoi3D.hpp"
#include "../lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/simple_io.hpp"

#define BOX_SIZE 20
#define HEIGHT 10


using namespace vivid;
using namespace std;

const std::string DATA_MODEL_PATH  = "./tests/test_data/";
const std::string TEST_OUTPUT_PATH = "./tests/test_models/";


/* Test basic shapes creation, add them to a Model and export to OBJ. */
int RunBasicTests(){
    cout << "Basic Test:" << endl;

    CModel model;
    // Some 3D viewers are centering the 3D models and change direction. this sets the center
    // TODO (Tomer): what does "this sets the center" mean???
    model.AddMesh( CreateSphereMesh(10, 10, 0.1, std::vector<double>({0.0, 0.0, 0.0}),  5, 0.8, std::string("sphere1")) );
    model.AddMesh( CreateSphereMesh(3, 3, 0.1, vector<double>{0.0, 0.0, 1.0}, 0.01, 0.2, "sphere2") );
    model.AddMesh( CreateSphereMesh(3, 3, 0.1, vector<double>{1, 0, 0}, 30, 1, "sphere3") );
    auto arrow_x = CreateArrowMesh(0.15, 0.4, vector<double>{0,0,0}, vector<double>{0,5,0}, 0.8, 0.6, "arrowX");
    model.AddMesh(arrow_x);
    model.ExportToObj(TEST_OUTPUT_PATH + "/arrow_test"); // /testModels/

    return EXIT_SUCCESS;
}

/* Test surf functionality with cube */
int RunCubeSurfTests() { //Decimate isn't activated currently
    cout << "Cube Test:" << endl;

    vector<vector<double >> points; vector<coord_t> quan; vector<bool> mask;

    for (int i = 2; i > -4; i -= 2) { // make the vornoi input points, a 3d grid for all combination optionts for 2, 0, -2
        for (int j = 2; j > -4; j -= 2) {
            for (int k = 2; k > -4; k -= 2) {
                points.push_back( vector<double> {(double)i, (double)j, (double)k} );
                quan.push_back(0.5);
                mask.push_back(false);
                if (i == j && j == k && k == 0) {
                    mask.back() = true;
                }
            }
        }
    }
//    cout << quan.size() << " " << points.size() << endl; // TODO: Should've been assert!

    CSurface surf = CSurface(points, mask, quan, *min_element(quan.begin(), quan.end() ), *max_element(quan.begin(), quan.end()) );
    surf.SmoothSurf();
    CMesh mesh = surf.ToMesh("vivid_3d_obj", 1.0);
//    mesh.Decimation(0.5, 0.4);
    mesh.ExportToObj(TEST_OUTPUT_PATH + "/Cube");

    return EXIT_SUCCESS;
}

/* Test surf functionality by cubic 3D pyramid (with square base) */
int RunPyramidSurfTest(){
    cout << "pyramid Test:" << endl;

    vector<vector<double >> points;
    vector<bool> mask;
    vector<coord_t> quan;
    coord_t Vmin, Vmax;
    vector<double> temp;
//----------------------------------------------------------------------pyramid ---------------------------------------
    cout << "but why" << endl;
    int a = 0;
    for (int i = -BOX_SIZE; i < BOX_SIZE; i += 2) {
        for (int j = -BOX_SIZE; j < BOX_SIZE; j += 2) {
            for (int z = -BOX_SIZE; z < BOX_SIZE; z += 2) {
                temp = vector<double>(3);
    		    temp[0] = i ; temp[1] = j; temp[2] = z;
                points.push_back(temp);
                if (z >= 0 && HEIGHT >= z) {
                    a = HEIGHT - z;
                } else {
                    a = 0;
                }

                if ((z >= 0 && HEIGHT >= z) && (-a <= i && i <= a) && (-a <= j && j <= a)) {
                    mask.push_back(true);
                    quan.push_back(std::log(pow(10, a) + 0.00001));
                } else {
                    mask.push_back(false);
                    quan.push_back(std::log(0.000001));
                }
            }
        }
    }

    cout << quan.size() << " " << points.size() << endl;
    Vmax = 0 ; //*max_element(quan.begin(), quan.end());
    Vmin = 0 ;//*min_element(quan.begin(), quan.end());

    CSurface surf = CSurface(points, mask, quan, *min_element( quan.begin(), quan.end() ), *max_element( quan.begin(), quan.end()) );
    surf.SmoothSurf();
    CMesh mesh = surf.ToMesh("vivid_3d_obj", 1.0);
//    mesh.Decimation(0.5, 0.4);
    mesh.ExportToObj(TEST_OUTPUT_PATH + "/Cube");

    return EXIT_SUCCESS;

}

/* Test the Elad Voronoi bug and the pointy faces bugs */
int RunMedicaneTests(){
    cout << "Medicane Test:" << endl;

    ModelData medicane = ReadBin(DATA_MODEL_PATH + "medicane.bin");
    ModelData medicaneNoise = ReadBin(DATA_MODEL_PATH + "medicane_noise.bin");

    CModel model;
    // Some 3D viewers are centering the 3D models and change direction. this sets the center
    model.AddMesh( CreateSphereMesh(10, 10, 0.1, vector<double>{0, 0, 0}, 5, 0.01, "sphere") );

    CSurface medicaneSurf = CSurface(medicane.points, medicane.mask, medicane.quan, medicane.quan[0], medicane.quan[0] );
    //medicaneSurf.SmoothSurf();
    CMesh medicaneMesh = medicaneSurf.ToMesh("medicane surf", 1.0);
    //medicaneMesh.Decimation(0.5, 0.4);
    model.AddMesh(medicaneMesh);
    model.ExportToObj(TEST_OUTPUT_PATH + "/MedicaneModelTest");
    return EXIT_SUCCESS;
}



int main(){
    int ret_value = EXIT_SUCCESS;

    ret_value = RunBasicTests();
    if ( EXIT_SUCCESS != RunBasicTests() ) return ret_value;
    cout << "Cube" << endl;
    ret_value = RunCubeSurfTests();
    if ( EXIT_SUCCESS != RunBasicTests() ) return ret_value;
    cout << "Pyramid" << endl;
    ret_value = RunPyramidSurfTest();
    if ( EXIT_SUCCESS != RunBasicTests() ) return ret_value;
    cout << "Medicane" << endl;
    ret_value = RunMedicaneTests();
    if ( EXIT_SUCCESS != RunBasicTests() ) return ret_value;

    return EXIT_SUCCESS;
}





// GARBAGE code of ADAM and NAFTALI #FIX IT!!! TODO TODO TODO
//  |
//  |
//  |
//  v

///*int main ()
//{
//    ModelData medicane = ReadBin( DATA_MODEL_PATH + "medicane.bin");
//
//    size_t const N = medicane.points.size ();
//    std :: vector <double> x (N);//= read_vector ("c: /sim_data/x.txt");
//    std :: vector <double> y (N);//= read_vector ("c: /sim_data/y.txt");
//    std :: vector <double> z (N);//= read_vector ("c: /sim_data/z.txt");
//
//    std :: vector <Vector3D> points (N);
//    //vector<CPoint> aInputPoints (N);
//    for (size_t i = 0; i < N; ++i) {
//        x[i] = medicane.points[i][0];
//        y[i] = medicane.points[i][1];
//        z[i] = medicane.points[i][2];
//        points[i].Set (x[i], y[i], z[i]);
////        aInputPoints[i].SetX(x[i]);
////        aInputPoints[i].SetY(y[i]);
////        aInputPoints[i].SetZ(z[i]);
//
//static bool ComparePoint(CPoint &aPoint1, CPoint &aPoint2) {
//    CPoint zeroPoint(0, 0, 0);
//    double dis1 = aPoint1.CalcDistance(zeroPoint);
//    double dis2 = aPoint2.CalcDistance(zeroPoint);
//    return (dis2 > dis1);
//}
//
//static double FindBoxR(vector<CPoint>& aInputPoints) {
//    CPoint zeroPoint(0, 0, 0);
//    CPoint box_r = *max_element(aInputPoints.begin(), aInputPoints.end(), *ComparePoint);
//    return box_r.CalcDistance(zeroPoint);
//}
//
//
//int main(){
//
//
//
//    CModel model;
//    // Some 3D viewers are centering the 3D models and change direction. this sets the center
//    model.AddMesh( CreateSphereMesh(10, 10, 0.1, vector<double>{0, 0, 0}, 5, 0.8, "sphere1") );
//    model.AddMesh( CreateSphereMesh(3, 3, 0.1, vector<double>{0, 0, 1}, 0.01, 0.2, "sphere2") );
//    model.AddMesh( CreateSphereMesh(3, 3, 0.1, vector<double>{1, 0, 0}, 30, 1, "sphere3") );
////
//    auto arrow_x = CreateArrowMesh(0.15, 0.4, vector<double>{0,0,0}, vector<double>{0,5,0}, 0.8, 0.6, "arrowX");
//    model.AddMesh(arrow_x);
//    model.ExportToObj("./TestModels/arrow_test"); // /testModels/
//
////
//    // Pyramid Surf
//    vector<double> temp;
//    vector<vector<double >> points;
//    vector<coord_t> quan;
//    vector<bool> mask;
///*
//    for (int i = 2; i > -4; i -= 2) { // make the vornoi input points, a 3d grid for all combination optionts for 2, 0, -2
//        for (int j = 2; j > -4; j -= 2) {
//            for (int z = 2; z > -4; z -= 2) {
//                temp = vector<double>(3);
//                temp[0] = i ; temp[1] = j; temp[2] = z;
//                points.push_back(temp);
//                quan.push_back(0.5);
//                mask.push_back(false);
//                if (i == j && j == z && z == 0) {
//                    mask.back() = true;
//                }
//            }
//        }
//    }
//    */
////------------------------------------------------------------------------------------pyramid -------------------------------------------------------------------------------------------
//cout << "but why" << endl;
//int a = 0;
//for (int i = -BOX_SIZE; i < BOX_SIZE; i += 2) {
//for (int j = -BOX_SIZE; j < BOX_SIZE; j += 2) {
//for (int z = -BOX_SIZE; z < BOX_SIZE; z += 2) {
//temp = vector<double>(3);
//temp[0] = i ; temp[1] = j; temp[2] = z;
//points.push_back(temp);
//if (z >= 0 && HEIGHT >= z) {
//a = HEIGHT - z;
//} else {
//a = 0;
//}
//
//if ((z >= 0 && HEIGHT >= z) && (-a <= i && i <= a) && (-a <= j && j <= a)) {
//mask.push_back(true);
//quan.push_back(std::log(pow(10, a) + 0.00001));
//} else {
//mask.push_back(false);
//quan.push_back(std::log(0.000001));
//}
//}
//}
//
//}
//
//cout << quan.size() << " " << points.size() << endl;
//coord_t Vmax = *max_element(quan.begin(), quan.end());
//coord_t Vmin = *min_element(quan.begin(), quan.end());
//cout << quan.size() << " " << points.size() << endl;
////--------------------------------------------------------------------run cube/pyramid -----------------------------------------------------------------------
//try {
//CSurf surf = CSurf(points, mask, quan, Vmin, Vmax);
//surf.SmoothSurf();
//CMesh mesh = surf.ToMesh("vivid_3d_obj", 1.0);
////        mesh.Decimation(0.5, 0.4);
//CModel model1 = CModel(mesh);
////model1.ExportToObj("./TestModels/Pyramid_model");
//mesh.ExportToObj("./TestModels/Pyramid_mesh", 0);
//}
//
///*    cout << "Center: " << endl;
//    cout << CenPoint3D.x << endl;
//    cout << CenPoint3D.y << endl;
//    cout << CenPoint3D.z << endl;
//    cout << "points[0] after center" << endl;
//    cout << points[0].x << endl;
//    cout << points[0].y << endl;
//    cout << points[0].z << endl;
//    //our box version:
//    //double boxR = FindBoxR(aInputPoints);
//
//    double const xmin1 = * (std :: min_element (x.begin (), x.end ()));
//    double const ymin1 = * (std :: min_element (y.begin (), y.end ()));
//    double const zmin1 = * (std :: min_element (z.begin (), z.end ()));
//    double const xmax1 = * (std :: max_element (x.begin (), x.end ()));
//    double const ymax1 = * (std :: max_element (y.begin (), y.end ()));
//    double const zmax1 = * (std :: max_element (z.begin (), z.end ()));
//
//    Vector3D ll (xmin1 - (xmax1 - xmin1) * 0.01, ymin1 - (ymax1 - ymin1) * 0.01, zmin1 - (zmax1 - zmin1) * 0.01);
//    Vector3D ur (xmax1 + (xmax1 - xmin1) * 0.01, ymax1 + (ymax1 - ymin1) * 0.01, zmax1 + (zmax1 - zmin1) * 0.01);
//
//    //aInputPoints.clear();
//    //aInputPoints.shrink_to_fit();
//    Voronoi3D tess (ll, ur);
//    //Voronoi3D tess (Vector3D(-boxR, -boxR, -boxR), Vector3D(boxR, boxR, boxR));
//    tess.Build (points);  */
//
//
//catch (exception &e) { cout << e.what() << endl;}
//
//return EXIT_SUCCESS;
//
//return 0;
//}*/
//
//
//
//
//
//
////int main ()
////{
////    ModelData medicane = ReadBin( DATA_MODEL_PATH + "medicane.bin");
////
////    size_t const N = medicane.points.size ();
////    std :: vector <double> x (N);//= read_vector ("c: /sim_data/x.txt");
////    std :: vector <double> y (N);//= read_vector ("c: /sim_data/y.txt");
////    std :: vector <double> z (N);//= read_vector ("c: /sim_data/z.txt");
////
////    std :: vector <Vector3D> points (N);
////    //vector<CPoint> aInputPoints (N);
////    for (size_t i = 0; i < N; ++i) {
////        x[i] = medicane.points[i][0];
////        y[i] = medicane.points[i][1];
////        z[i] = medicane.points[i][2];
////        points[i].Set (x[i], y[i], z[i]);
////        /*aInputPoints[i].SetX(x[i]);
////        aInputPoints[i].SetY(y[i]);
////        aInputPoints[i].SetZ(z[i]);*/
////    }
////
////    double const xmin = * (std :: min_element (x.begin (), x.end ()));
////    double const ymin = * (std :: min_element (y.begin (), y.end ()));
////    double const zmin = * (std :: min_element (z.begin (), z.end ()));
////    double const xmax = * (std :: max_element (x.begin (), x.end ()));
////    double const ymax = * (std :: max_element (y.begin (), y.end ()));
////    double const zmax = * (std :: max_element (z.begin (), z.end ()));
////
////    // Elad box version 1, superior to our box. why * 0.01 -> slight box expansion.
////    //Vector3D ll (xmin - (xmax - xmin) * 0.01, ymin - (ymax - ymin) * 0.01, zmin - (zmax - zmin) * 0.01);
////    //Vector3D ur (xmax + (xmax - xmin) * 0.01, ymax + (ymax - ymin) * 0.01, zmax + (zmax - zmin) * 0.01);
////    cout << "points[0] before center" << endl;
////    cout << points[0].x << endl;
////    cout << points[0].y << endl;
////    cout << points[0].z << endl;
////    //CPoint CenPoint((xmax + xmin)/2, (ymax + ymin)/2, (zmax + zmin)/2);
////    Vector3D const CenPoint3D = Vector3D((xmax + xmin)/2, (ymax + ymin)/2, (zmax + zmin)/2);
////    for (size_t i = 0; i< N; ++i){
////        x[i] -= CenPoint3D.x;
////        y[i] -= CenPoint3D.y;
////        z[i] -= CenPoint3D.z;
////
////        points[i].operator-=(CenPoint3D);
////        //aInputPoints[i].operator-=(CenPoint);
////    }
////    cout << "Center: " << endl;
////    cout << CenPoint3D.x << endl;
////    cout << CenPoint3D.y << endl;
////    cout << CenPoint3D.z << endl;
////    cout << "points[0] after center" << endl;
////    cout << points[0].x << endl;
////    cout << points[0].y << endl;
////    cout << points[0].z << endl;
////    //our box version:
////    //double boxR = FindBoxR(aInputPoints);
////
////    double const xmin1 = * (std :: min_element (x.begin (), x.end ()));
////    double const ymin1 = * (std :: min_element (y.begin (), y.end ()));
////    double const zmin1 = * (std :: min_element (z.begin (), z.end ()));
////    double const xmax1 = * (std :: max_element (x.begin (), x.end ()));
////    double const ymax1 = * (std :: max_element (y.begin (), y.end ()));
////    double const zmax1 = * (std :: max_element (z.begin (), z.end ()));
////
////    Vector3D ll (xmin1 - (xmax1 - xmin1) * 0.01, ymin1 - (ymax1 - ymin1) * 0.01, zmin1 - (zmax1 - zmin1) * 0.01);
////    Vector3D ur (xmax1 + (xmax1 - xmin1) * 0.01, ymax1 + (ymax1 - ymin1) * 0.01, zmax1 + (zmax1 - zmin1) * 0.01);
////
////    //aInputPoints.clear();
////    //aInputPoints.shrink_to_fit();
////    Voronoi3D tess (ll, ur);
////    //Voronoi3D tess (Vector3D(-boxR, -boxR, -boxR), Vector3D(boxR, boxR, boxR));
////    tess.Build (points);
////    return 0;
////}


//struct CData{
//    vector<vector<float>> mPoints;
//    vector<bool> mMask;
//    vector<float> mQuan;
//};
//
//CData readCSV(const string &strPath2Dataset, bool noise = false)
//{
//    ifstream csvFile_1;
//    ifstream csvFile_2;
//    string medicane_xyz;
//    string medicane_mask;
//
//    if (noise) {
//        medicane_xyz  = "/noise_medicane_xyz.csv";
//        medicane_mask = "/noise_medicane_mask.csv";
//    }
//    else {
//        medicane_xyz  = "/medicane_xyz.csv";
//        medicane_mask = "/medicane_mask.csv";
//    }
//    string strPathCSVFile_1 = strPath2Dataset + medicane_xyz;
//    string strPathCSVFile_2 = strPath2Dataset + medicane_mask;
//    csvFile_1.open(strPathCSVFile_1.c_str());
//    csvFile_2.open(strPathCSVFile_2.c_str());
//    if (!csvFile_1.is_open())
//    {
//        cout << "Path Wrong!!!!" << endl;
//        exit(EXIT_FAILURE);
//    }
//    vector<int> px_Index;
//    vector<double> temp;
//    // vector<float> px_X;
//    // vector<float> px_Y;
//    // vector<float> px_Z;
//    vector<bool> px_Mask;
//    vector<vector<double>> points;
//
//
//    string line;
//    vector <string> vec;
//    getline(csvFile_1, line); // skip the 1st line
//
//    istringstream iss();
//    while (getline(csvFile_1,line))
//    {
//        if (line.empty()) // skip empty lines:
//        {
//            //cout << "empty line!" << endl;
//            continue;
//        }
//
//        iss = istringstream(line);
//        string lineStream;
//        vector <double> row;
//
//        while (getline(iss, lineStream, ','))
//        {
//            row.push_back(stold(lineStream,&sz)); // convert to double
//        }
//
//        px_Index.push_back(row[0]);
//
//
//        temp = vector<double>(3);
//        temp[0] = row[1] ; temp[1] = row[2]; temp[2] = row[3];
//        points.push_back(temp);
//
//        quan.push_back(0.5);
//    }
//
//    while (getline(csvFile_2, line))
//    {
//        if (line.empty()) // skip empty lines:
//        {
//            //cout << "empty line!" << endl;
//            continue;
//        }
//
//        istringstream iss(line);
//        string lineStream;
//
//        while (getline(iss, lineStream, ','))
//        {
//            px_Mask.push_back(stoi(lineStream, &sz)); // convert to int (bool)
//        }
//    }
//    CData data_bin = {.points=points, .mask=px_Mask, .quan=quan};
//    return data_bin;
//}
//
//
//
//int main(){
//
////    f = open('my_file', 'w+b')
////    byte_arr = [120, 3, 255, 0, 100]
////    binary_format = bytearray(byte_arr)
////    f.write(binary_format)
////    f.close()
//
//    // Import unfiltered data as csv, parse into several vectors
//    auto data_og = readCSV(DATA_MODEL_PATH, false);
//    // Import data with added noise as csv, parse into several vectors
//    auto data_noise = readCSV(DATA_MODEL_PATH, true);
//
//    CModel model;
//    // Some 3D viewers are centering the 3D models and change direction. this sets the center
//    model.AddMesh( CreateSphereMesh(10, 10, 0.1, vector<double>{0, 0, 0}, 5, 0.8, "sphere") );
//
//    CSurf medicaneSurf = CSurf(data_og.mPoints, data_og.mMask, data_og.mQuan, 0.f, 1.f);
//    medicaneSurf.smoothSurf();
//    CMesh medicaneMesh = medicaneSurf.ToMesh("vivid_3d_obj", 1.0);
//    medicaneMesh.Decimation(0.5, 0.4);
//    model.AddMesh(medicaneMesh);
//    model.ExportToObj("MedicaneModelTest");
//
//    return 0;
//}
//