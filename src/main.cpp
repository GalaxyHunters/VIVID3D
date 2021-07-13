#include <vector>
#include "Utils/ReadBinFile.h"
#include "ModelBuilder/Model.h"
#include "ModelBuilder/Shapes.h"
#include "ImportAndExport/FBXImportExport.h"
#include "ModelBuilder/Surface.h"
#include "ModelBuilder/Point.h"

//// TODO TOMER YYYYYY!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include "../lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/simple_io.hpp"

#define BOX_SIZE 50
#define HEIGHT 25


using namespace vivid;
using namespace std;

const std::string DATA_MODEL_PATH  = "./tests/test_data/";
const std::string TEST_OUTPUT_PATH = "./tests/test_models/";


/* Test basic shapes creation, add them to a Model and export to OBJ. */
int ShapesTest(){
    cout << "Basic Test:" << endl;

    CModel model;
    // Some 3D viewers are centering the 3D models and change direction. this sets the center
    // TODO (Tomer): what does "this sets the center" mean???
    model.AddMesh( CreateSphereMesh(CPoint(0.0, 0.0, 0.0), 5, 20, 20, 0.1, 0.8, "sphere1" ));
    auto arrow_x = CreateArrowMesh( CPoint(0,0,0), CPoint(0,5,0), 0.15, 0.4, 0.8, 0.6, "arrowX");
    model.AddMesh(arrow_x);
    model.AddMesh(CreateBoxMesh(CPoint(4,0,0), CPoint(5,3,4), 0.5, 0.4, "Box"));
    model.AddMesh(CreateCubeMesh(CPoint(-4,0,0), 3, 0.5, 0.4, "Cube"));
    model.ExportToObj(TEST_OUTPUT_PATH + "/Shapes"); // /test_models/

    return EXIT_SUCCESS;
}

/* Test surf functionality with cube */
// Seems to be broken.
int CubeSurfTests() { //Reduce isn't activated currently
    cout << "Cube Test:" << endl;

    vector<vector<double >> points; vector<coord_t> quan; vector<bool> mask;

    for (int i = 2; i >= -2; i -= 2) { // make the vornoi input points, a 3d grid for all combination optionts for 2, 0, -2
        for (int j = 2; j >= -2; j -= 2) {
            for (int k = 2; k >= -2; k -= 2) {
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
    surf.CreateSurface();
    //surf.Smooth();
    CMesh mesh = surf.ToMesh("vivid_3d_obj", 1.0);
    //mesh.Reduce(0.3, 0.3);
    mesh.ExportToObj(TEST_OUTPUT_PATH + "/Cube");

    return EXIT_SUCCESS;
}

/* Test surf functionality by cubic 3D pyramid (with square base) */
int PyramidSurfTest(){
    cout << "Pyramid Test:" << endl;

    vector<vector<double >> points;
    vector<bool> mask;
    vector<coord_t> quan;
    coord_t Vmin, Vmax;
    vector<double> temp;
//----------------------------------------------------------------------pyramid ---------------------------------------
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

    Vmax = 0 ; //*max_element(quan.begin(), quan.end());
    Vmin = 0 ;//*min_element(quan.begin(), quan.end());

    CSurface surf = CSurface(points, mask, quan, *min_element( quan.begin(), quan.end() ), *max_element( quan.begin(), quan.end()) );
    surf.CreateSurface();
    surf.Smooth();
    surf.Smooth();
    CMesh mesh = surf.ToMesh("vivid_3d_obj", 1.0);
    mesh.Reduce(0.1, 0.25);
    mesh.ExportToObj(TEST_OUTPUT_PATH + "/Pyramid");

    return EXIT_SUCCESS;

}

/* Test the Elad RunVorn bug and the pointy faces bugs */
int RunBlackHoleTests(){
    cout << "Black Hole Test:" << endl;

    ModelData medicane = ReadBin(DATA_MODEL_PATH + "supernova-15.bin");

    CModel model;
    // Some 3D viewers are centering the 3D models and change direction. this sets the center
    //model.AddMesh( CreateSphereMesh(10, 10, 0.1, vector<double>{0, 0, 0}, 5, 0.01, "sphere") );

    CSurface medicaneSurf = CSurface(medicane.points, medicane.mask, medicane.quan, medicane.quan[0], medicane.quan[0] );
    medicaneSurf.CreateSurface();
    //medicaneSurf.Smooth();
    CMesh medicaneMesh = medicaneSurf.ToMesh("BlackHole surf", .7);
    medicaneMesh.Reduce(0.3, 0.25);
    model.AddMesh(medicaneMesh);
    model.ExportToObj(TEST_OUTPUT_PATH + "/Supernova200k");
    return EXIT_SUCCESS;
}

int main(){
    int ret_value = EXIT_SUCCESS;

    ret_value = ShapesTest();
    if ( EXIT_SUCCESS != ret_value ) return ret_value;
    cout << "Cube" << endl;
    ret_value = CubeSurfTests();
    if ( EXIT_SUCCESS != ret_value ) return ret_value;
    cout << "Pyramid" << endl;
    ret_value = PyramidSurfTest();
    if ( EXIT_SUCCESS != ret_value ) return ret_value;
    cout << "Black Hole" << endl;
    ret_value = RunBlackHoleTests();
    if ( EXIT_SUCCESS != ret_value ) return ret_value;

    return EXIT_SUCCESS;
}