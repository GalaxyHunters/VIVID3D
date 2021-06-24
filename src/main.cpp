#include <vector>
#include "Utils/ReadBinFile.h"
#include "ModelBuilder/Model.h"
#include "Utils/Shapes.h"
#include "AnimationBuilder/Animation.h"
#include "ModelBuilder/Surf.h"
#include "ModelBuilder/Point.h"

using namespace vivid;
using namespace std;

const std::string DATA_FOLDER_PATH = "./tests/test_data/";

/* Test basic shapes creation, add them to a Model and export to OBJ. */
int RunBasicTests(){
    CModel model;
    // Some 3D viewers are centering the 3D models and change direction. this sets the center
    // TODO (Tomer): what does "this sets the center" mean???
    model.AddMesh( CreateSphereMesh(10, 10, 0.1, vector<double>{0, 0, 0}, 5, 0.8, "sphere1") );
    model.AddMesh( CreateSphereMesh(3, 3, 0.1, vector<double>{0, 0, 1}, 0.01, 0.2, "sphere2") );
    model.AddMesh( CreateSphereMesh(3, 3, 0.1, vector<double>{1, 0, 0}, 30, 1, "sphere3") );
    auto arrow_x = CreateArrowMesh(0.15, 0.4, vector<double>{0,0,0}, vector<double>{0,5,0}, 0.8, 0.6, "arrowX");
    model.AddMesh(arrow_x);
    model.ExportToObj("./testModels/arrow_test"); // /testModels/

    return EXIT_SUCCESS;
}

/*
 * Test surf functionality by cubic 3D pyramid (with square base)
 */
int RunPyramidSurfTests() { //Decimate isn't activated currently
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

    CSurf surf = CSurf(points, mask, quan, *min_element( quan.begin(), quan.end() ), *max_element( quan.begin(), quan.end()) );
    surf.SmoothSurf();
    CMesh mesh = surf.ToMesh("vivid_3d_obj", 1.0);
//    mesh.Decimation(0.5, 0.4);
    mesh.ExportToObjTexture("./testModels/Pyramid");

    return EXIT_SUCCESS;
}

/* Test the Elad Voronoi bug and the pointy faces bugs */
int RunMedicaneTests(){
    ModelData medicane = ReadBin( DATA_FOLDER_PATH + "medicane.bin");
    ModelData medicaneNoise = ReadBin(DATA_FOLDER_PATH + "medicane_noise.bin");

    CModel model;
    // Some 3D viewers are centering the 3D models and change direction. this sets the center
    model.AddMesh( CreateSphereMesh(10, 10, 0.1, vector<double>{0, 0, 0}, 5, 0.8, "sphere") );

    CSurf medicaneSurf = CSurf( medicane.points, medicane.mask, medicane.quan, medicane.quan[0], medicane.quan[0] );
//    medicaneSurf.SmoothSurf();
//    CMesh medicaneMesh = medicaneSurf.ToMesh("medicane surf", 1.0);
//    medicaneMesh.Decimation(0.5, 0.4);
//    model.AddMesh(medicaneMesh);
//    model.ExportToObj("MedicaneModelTest");
    return EXIT_SUCCESS;
}



int main(){
    int ret_value = EXIT_SUCCESS;

    ret_value = RunBasicTests();
    if ( EXIT_SUCCESS != RunBasicTests() ) return ret_value;

    ret_value = RunPyramidSurfTests();
    if ( EXIT_SUCCESS != RunBasicTests() ) return ret_value;

    ret_value = RunMedicaneTests();
    if ( EXIT_SUCCESS != RunBasicTests() ) return ret_value;

    return EXIT_SUCCESS;
}


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
//    auto data_og = readCSV(DATA_FOLDER_PATH, false);
//    // Import data with added noise as csv, parse into several vectors
//    auto data_noise = readCSV(DATA_FOLDER_PATH, true);
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