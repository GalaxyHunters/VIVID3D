#include <vector>
#include "Utils/ReadBinFile.h"
#include "ModelBuilder/Model.h"
#include "ModelBuilder/Shapes.h"
#include "ModelBuilder/SurfByFunc.h"
#include "ImportAndExport/FBXImportExport.h"
#include "ModelBuilder/Surface.h"
#include "ModelBuilder/Point.h"
#include "ModelBuilder/PointVector.h"
#define BOX_SIZE 30
#define HEIGHT 15

using namespace vivid;
using namespace std;

const std::string DATA_MODEL_PATH  = "./tests/test_data/";
const std::string TEST_OUTPUT_PATH = "./tests/test_models/";

/* Test vector<CPoint> */
int PointVectorTest()
{
    vector<CPoint> points;
    vector<vector<coord_t>> size = {{0,1,2}, {0,1,2}};
//    cout << points[0].X() << endl;

    return EXIT_SUCCESS;
}


/* Test basic shapes creation, add them to a Model and export to OBJ. */
int ShapesTest()
{
    cout << "Basic Test:" << endl;

    CModel model;
    // Some 3D viewers are centering the 3D models and change direction. this sets the center
    CMesh sphere = CreateSphereMesh(CPoint(0.0, 0.0, 0.0), 5, 20, 20, 0.1, 0.8, "sphere1" );
    CMesh arrow_x = CreateArrowMesh( CPoint(0,0,0), CPoint(0,5,0), 0.15, 0.4, 0.8, 0.6, "arrowX");
    CMesh box = CreateBoxMesh(CPoint(4,0,0), CPoint(5,3,4), 0.5, 0.4, "Box");
    CMesh cube = CreateCubeMesh(CPoint(-4,0,0), 3, 0.5, 0.4, "Cube");
    pair<CLines, CLines> grid = CreateGrid(100, 10);
    vector<CModelComponent> list = {sphere, arrow_x, box, cube, grid.first, grid.second};
    model.AddMeshes(list);
    model.ExportToObj(TEST_OUTPUT_PATH + "/Shapes"); // /test_models/

    return EXIT_SUCCESS;
}

coord_t TestFunc(const coord_t x, const coord_t y) {
    //return (7*x*y)/pow(2,x*x+y*y);
//    return 1./(y-x*x);
//    if ((4-x*x-y*y) >= -0.1 & (4-x*x-y*y) <= 0.01) {
//        return 0;
//    }
    return sqrt(4-x*x-y*y);
}
coord_t Sphere2Func(const coord_t x, const coord_t y) {

    return -sqrt(4-x*x-y*y);
}

int SurfByFuncTest()
{
    F3D_t func = TestFunc;
    CMesh mesh = SurfByFunc(func, 3, 1., "Test_Func");
    F3D_t func2 = Sphere2Func;
    CMesh mesh2 = SurfByFunc(func2, 3, 1., "Test_Func");
    CModel model = CModel({mesh, mesh2});
    model.ExportToObj(TEST_OUTPUT_PATH + "SurfByFunc");
    return EXIT_SUCCESS;
}

CPoint ParametricTest(const coord_t u, const coord_t v)
{
    coord_t x = sin(v);
    coord_t y = (2 + cos(v)) * sin(u);
    coord_t z = (2 + cos(v)) * cos(u);
//    coord_t x = v*cos(u);
//    coord_t y = v*sin(u);
//    coord_t z = v+sin(3*v)/3-4;
    return CPoint(x,y,z);
}

int ParametricSurfByFuncTest()
{
    FParametric_t func = ParametricTest;
    CMesh mesh = ParametricSurface(func, 50, 0, 2*M_PI, 0, 2*M_PI, 1., "Parametric");
    mesh.ExportToObj(TEST_OUTPUT_PATH + "ParametricTestAlt");
    return EXIT_SUCCESS;
}

/* Test surf functionality with cube */
int CubeSurfTests()
{
    cout << "Cube Test:" << endl;

    vector<vector<double >> points; vector<coord_t> quan; vector<bool> mask;

    for (int i = 2; i >= -2; i -= 2) { // make the vornoi input points, a 3d grid for all combination optionts for 2, 0, -2
        for (int j = 2; j >= -2; j -= 2) {
            for (int k = 2; k >= -2; k -= 2) {
                points.push_back( vector<double> {(double)i, (double)j, (double)k} );
                quan.push_back(j);
                mask.push_back(false);
                if (i == j && j == k && k == 0) {
                    mask.back() = true;
                }
            }
        }
    }
    CModel model;
    CSurface surf = CSurface(points, mask, quan, *min_element(quan.begin(), quan.end() ), *max_element(quan.begin(), quan.end()) );
    surf.CreateSurface();
//    cerr << "Initiating Copy Constructor" << endl;
//    CSurface surf_copy = CSurface(surf);
    CMesh mesh = surf.ToMesh("vivid_3d_obj", 1.0);
    mesh.SetClm("Red");
    //mesh.Reduce(0.3, 0.3);
    mesh.ExportToObj(TEST_OUTPUT_PATH + "/Cube");
    CMesh mesh2 = CreateBoxMesh(CPoint(4,0,0), CPoint(5,3,4), 0.5, 0.4, "Box");
    mesh2.SetClm("Green");
    CMesh mesh3 = CreateBoxMesh(CPoint(-4,0,0), CPoint(5,3,4), 0.5, 0.4, "Box");
    mesh3.SetClm("Blue");
    vector<CModelComponent> meshes = {mesh2, mesh3};
    model.AddMesh(mesh2);
    model.ExportToObj(TEST_OUTPUT_PATH + "/CubeShape");
    return EXIT_SUCCESS;
}

/* Test Colormap functionality by many cubemeshes */
int ColorMapTest()
{
    CModel model;
    vector<string> colors = {"Red", "Blue", "Green", "Purple", "Yellow", "Cyan", "White", "Black"};
    vector<CPoint> loc = {{0,0,0}, {1,0,0}, {2,0,0},{0,0,1},{1,0,1},{2,0,1},{0,0,2},{1,0,2}};
    for (int i = 0; i < colors.size(); i++) {
        CMesh box = CreateCubeMesh(loc[i]*3,0.5,1,1,colors[i]);
        box.SetClm(colors[i]);
        model.AddMesh(box);
    }
    model.ExportToObj(TEST_OUTPUT_PATH + "/ColorsTest");
    return EXIT_SUCCESS;
}


/* Test surf functionality by cubic 3D pyramid (with square base) */
int PyramidSmoothTest()
{
    cout << "Pyramid Test:" << endl;

    vector<vector<double >> points;
    vector<bool> mask;
    vector<coord_t> quan;
    coord_t Vmin, Vmax;
    vector<double> temp;
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

    CSurface smooth1 = CSurface(points, mask, quan, *min_element( quan.begin(), quan.end() ), *max_element( quan.begin(), quan.end()) );
    smooth1.CreateSurface();
    //CSurface surf_copy = CSurface(smooth1);
    CMesh mesh1 = smooth1.ToMesh("vivid_3d_obj", .5);
    //mesh1.Reduce(0.3, 0.5);
//    mesh1.ExportToObj(TEST_OUTPUT_PATH + "/Pyramid");
    CSurface smooth3 = CSurface(points, mask, quan, *min_element( quan.begin(), quan.end() ), *max_element( quan.begin(), quan.end()) );
    smooth3.CreateSurface();
    CMesh mesh3 = smooth3.ToMesh("vivid_3d_obj", 1.0);
    mesh3.LaplacianSmooth(10, 0.7, 0);
    mesh3.LaplacianSmooth(150, 0.25, 0.7);
    mesh3.Reduce(0.25, 0.7);
    mesh3.ExportToObj(TEST_OUTPUT_PATH + "/PyramidLaplacianSmooth_HC_reduce");
    CModel model = CModel({mesh3, mesh1});
    model.ExportToObj(TEST_OUTPUT_PATH + "/SmoothReduceComparison");
//    CSurface smooth3 = CSurface(points, mask, quan, *min_element( quan.begin(), quan.end() ), *max_element( quan.begin(), quan.end()) );
//    smooth3.CreateSurface();
//    smooth3.Smooth(3);
//    CMesh mesh3 = smooth3.ToMesh("vivid_3d_obj", 1.0);
//    mesh3.Reduce(0.4, 0.3);
//    mesh3.ExportToObj(TEST_OUTPUT_PATH + "/PyramidSmooth3");
//    CSurface smooth6 = CSurface(points, mask, quan, *min_element( quan.begin(), quan.end() ), *max_element( quan.begin(), quan.end()) );
//    smooth6.CreateSurface();
//    smooth6.Smooth(6);
//    CMesh mesh6 = smooth6.ToMesh("vivid_3d_obj", 1.0);
//    mesh6.Reduce(0.25, 0.3);
//    mesh6.ExportToObj(TEST_OUTPUT_PATH + "/PyramidSmooth6");
//    CSurface smooth8 = CSurface(points, mask, quan, *min_element( quan.begin(), quan.end() ), *max_element( quan.begin(), quan.end()) );
//    smooth8.CreateSurface();
//    smooth8.Smooth(8);
//    CMesh mesh8 = smooth8.ToMesh("vivid_3d_obj", 1.0);
//    mesh8.Reduce(0.2, 0.3);
//    mesh8.ExportToObj(TEST_OUTPUT_PATH + "/PyramidSmooth8");
    return EXIT_SUCCESS;

}

/* Test the Elad RunVorn bug and the pointy faces bugs */
int RunSupernovaTests()
{
    cout << "Black Hole Test:" << endl;
//    vector<ModelData> data (7);
//    cout << "Loading Data" << endl;
////    data[0] = ReadBin(DATA_MODEL_PATH + "Supernova-0.bin");
//    data[0] = ReadBin(DATA_MODEL_PATH + "Supernova-1_5.bin");
//    data[1] = ReadBin(DATA_MODEL_PATH + "Supernova-2_5.bin");
//    data[2] = ReadBin(DATA_MODEL_PATH + "Supernova-6.bin");
//    data[4] = ReadBin(DATA_MODEL_PATH + "Supernova-7_5.bin");
//    data[3] = ReadBin(DATA_MODEL_PATH + "Supernova-10.bin");
//    data[6] = ReadBin(DATA_MODEL_PATH + "Supernova-15.bin");
    vector<string> sufix = {"0", "1_5", "2_5", "6", "7_5", "10", "15"};
    vector<coord_t> why = {0, 1.5, -2.5, -6, -7. - 10., -15.};
    ModelData nova;
    CModel nova_model;
    cout << "Running VIVID" << endl;
    
    for (int i = 0; i < 7; i++) {
        nova = ReadBin(DATA_MODEL_PATH + "Supernova-"+sufix[i]+".bin");
        for (int j = 0; j < nova.quan.size(); j++) {
            nova.quan[j] = why[i];
        }
        CSurface surf = CSurface(nova.points, nova.mask, nova.quan, -15.1, 1.51);
        surf.CreateSurface();
        //surf.Smooth(false, 1);
        cout << "Convert to Mesh" << endl;
        CMesh mesh = surf.ToMesh("Surf" + to_string(i), .7);
        //mesh.SubdivideLargeFaces(4);
//        mesh.RemovePointyFaces(20);
        mesh.CalculatePointsNeighbours();
        mesh.LaplacianSmooth(6, 0.7, 0);
        mesh.LaplacianSmooth(50, 0.4, 0.7);
        //mesh.RemovePointyFaces();
        mesh.Reduce(0.1, 0.8);
        //mesh.ExportToObj(TEST_OUTPUT_PATH + "/Supernova_" + to_string(i));
        nova_model.AddMesh(mesh);
    }
    nova_model.ExportToObj(TEST_OUTPUT_PATH + "/SupernovaModel");
    return EXIT_SUCCESS;
}

int RemovePointyFacesTest() {
    cout << "Remove Pointy Faces Test:" << endl;
    cout << "Loading Data" << endl;
////    data[0] = ReadBin(DATA_MODEL_PATH + "Supernova-0.bin");
//    data[0] = ReadBin(DATA_MODEL_PATH + "Supernova-1_5.bin");
//    data[1] = ReadBin(DATA_MODEL_PATH + "Supernova-2_5.bin");
//    data[2] = ReadBin(DATA_MODEL_PATH + "Supernova-6.bin");
//    data[4] = ReadBin(DATA_MODEL_PATH + "Supernova-7_5.bin");
    ModelData nova = ReadBin(DATA_MODEL_PATH + "Supernova-10.bin");
//    data[6] = ReadBin(DATA_MODEL_PATH + "Supernova-15.bin");

    for (int j = 0; j < nova.quan.size(); j++) {
        nova.quan[j] = 0;
    }
    CSurface surf = CSurface(nova.points, nova.mask, nova.quan, 0, 0);
    surf.CreateSurface();
    //surf.Smooth(false, 1);
    cout << "Convert to Mesh" << endl;
    CMesh mesh = surf.ToMesh("pointytest", .7);
    mesh.ExportToObj(TEST_OUTPUT_PATH + "/Supernova_no_processing");
    mesh.SubdivideLargeFaces();
    mesh.SubdivideLargeFaces();
    mesh.SubdivideLargeFaces();
    cout << mesh.GetFaces().size() << endl;
    mesh.RemovePointyFaces();
    mesh.LaplacianSmooth(20, 0.7, 0);
    mesh.ExportToObj(TEST_OUTPUT_PATH + "/Supernova_smooth_1");
    mesh.LaplacianSmooth(150, 0.25, 0.55);
    //mesh.RemoveLargeFaces(4);

    mesh.RemovePointyFaces(20);
    mesh.RemovePointyFaces(20);
    mesh.Reduce(0.25, .7);
    mesh.ExportToObj(TEST_OUTPUT_PATH + "/Supernova_smooth_reduce");
    return EXIT_SUCCESS;
}

int main()
{
    int ret_value = EXIT_SUCCESS;
//    cout << "ParametricSurfByFuncTest" << endl;
//    ret_value = ParametricSurfByFuncTest();
//    if ( EXIT_SUCCESS != ret_value ) return ret_value;
//    cout << "Testing All Shapes" << endl;
//    ret_value = ShapesTest();
//    if ( EXIT_SUCCESS != ret_value ) return ret_value;
//    cout << "Cube" << endl;
    ret_value = PointVectorTest();
    if ( EXIT_SUCCESS != ret_value ) return ret_value;
//    cout << "Colors" <<endl;
//    ret_value = ColorMapTest();
//    if ( EXIT_SUCCESS != ret_value) return ret_value;
//    cout << "Pyramid" << endl;
//    ret_value = PyramidSmoothTest();
//    if ( EXIT_SUCCESS != ret_value ) return ret_value;
//    ret_value = RemovePointyFacesTest();
//    if ( EXIT_SUCCESS != ret_value ) return ret_value;
    return EXIT_SUCCESS;
}