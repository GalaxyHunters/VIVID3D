#include <vector>
#include "Utils/ReadBinFile.h"
#include "ModelBuilder/Model.h"
#include "ModelBuilder/Shapes.h"
#include "ModelBuilder/SurfByFunc.h"
#include "ModelBuilder/VoronoiVolume.h"
#include "Point.h"
#include "AssimpImportExport.h"
#include "DataToImage.h"
#include "Model.h"
#include "PointCloud.h"
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/types.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

constexpr int BOX_SIZE = 30;
constexpr int HEIGHT   = 15;

using namespace vivid;
using namespace std;

const std::string DATA_MODEL_PATH  = "./tests/test_data/";
const std::string TEST_OUTPUT_PATH = "./tests/test_models/";


/* Test basic shapes creation, add them to a Model and export to OBJ. */
int ShapesTest()
{
    Log(LOG_INFO, "Basic Test:");

    CModel model;
    // Some 3D viewers are centering the 3D models and change direction. this sets the center
    CMesh sphere = CreateSphereMesh(CPoint(0.0, 0.0, 0.0), 5, 20, 20, "purple", 0.8, "sphere1" );
    CMesh arrow_x = CreateArrowMesh( CPoint(0,0,0), CPoint(0,5,0), 0.15, 0.1, "green", 0.6, "arrowX");
    CMesh box = CreateBoxMesh(CPoint(4,0,0), CPoint(5,3,4), "red", 0.4, "Box");
    CMesh cube = CreateCubeMesh(CPoint(-4,0,0), 3, "yellow", 0.4, "Cube");
    CMesh cube1 = CreateCubeMesh(CPoint(-4,0,0), 3, "yellow", 0.4, "Cube1");
    model.AddMeshes({cube, cube1});
    pair<CLines, CLines> grid = CreateGrid(100, 10);
    vector<CModelComponent> list = {sphere, arrow_x, box, cube, grid.first, grid.second};
//    model.AddMeshes(list);
    model.Export(TEST_OUTPUT_PATH + "Shapes"); // /test_models/

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
    //Cylinder:
    coord_t x = cos(u);
    coord_t y = sin(u);
    coord_t z = v;
    // cone:
//    coord_t x = u*cos(v);
//    coord_t y = u*sin(v);
//    coord_t z = u;
    // Moebuis:
//    coord_t x = (1+0.5*v*cos(u*.5))*cos(u);
//    coord_t y = (1+0.5*v*cos(u*.5))*sin(u);
//    coord_t z = v*0.5*sin(u*0.5);
    // Torus:
//    coord_t x = sin(v);
//    coord_t y = (2 + cos(v)) * sin(u);
//    coord_t z = (2 + cos(v)) * cos(u);
    // Sphere:
//    coord_t x = v*cos(u);
//    coord_t y = v*sin(u);
//    coord_t z = v+sin(3*v)/3-4;
    return {x,y,z};
}

int ParametricSurfByFuncTest()
{
    FParametric_t func = ParametricTest;
    CMesh mesh = ParametricSurface(func, 7, 0, 2*M_PI, -1, 2,  1., "Parametric");
    mesh.ExportToObj(TEST_OUTPUT_PATH + "ParametricTestAlt");
    return EXIT_SUCCESS;
}

/* Test surf functionality with cube */
int CubeSurfTests()
{
    Log(LOG_INFO, "Cube Test:");

    vector<CPoint> points; vector<normal_float> quan; vector<bool> mask;

    for (int i = 2; i >= -2; i -= 2) { // make the vornoi input points, a 3d grid for all combination optionts for 2, 0, -2
        for (int j = 2; j >= -2; j -= 2) {
            for (int k = 2; k >= -2; k -= 2) {
                points.push_back( {(double)i, (double)j, (double)k} );
                quan.push_back(j);
                mask.push_back(false);
                if (i == j && j == k && k == 0) {
                    mask.back() = true;
                }
            }
        }
    }
    CModel model;
    CVoronoiVolume volume = CVoronoiVolume(points, quan, *min_element(quan.begin(), quan.end() ), *max_element(quan.begin(), quan.end()) );
    volume.CreateSurface();
//    cerr << "Initiating Copy Constructor");
//    CVoronoiVolume surf_copy = CVoronoiVolume(surf);
    CMesh mesh = volume.MaskMesh(mask,"vivid_3d_obj", 1.0);
    mesh.SetColor("Red");
    //mesh.Reduce(0.3, 0.3);
    mesh.ExportToObj(TEST_OUTPUT_PATH + "/Cube");
    CMesh mesh2 = CreateBoxMesh(CPoint(4,0,0), CPoint(5,3,4), "green", 0.4, "Box");
    CMesh mesh3 = CreateBoxMesh(CPoint(-4,0,0), CPoint(5,3,4), "blue", 0.4, "Box");
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
        CMesh box = CreateCubeMesh(loc[i]*3,0.5,colors[i],1,colors[i]);
        model.AddMesh(box);
    }
    model.ExportToObj(TEST_OUTPUT_PATH + "/ColorsTest");
    return EXIT_SUCCESS;
}


/* Test surf functionality by cubic 3D pyramid (with square base) */
int PyramidSmoothTest()
{
    Log(LOG_INFO, "Pyramid Test:");

    vector<CPoint> points;
    vector<bool> mask;
    vector<normal_float> quan;
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

    CPointCloud pyramid_points = CPointCloud(points, quan, 0, 0, 1.0, "");
//    CVoronoiVolume smooth1 = CVoronoiVolume(points, mask, quan, Vmin, Vmax);
//    smooth1.CreateSurface();
    //CVoronoiVolume surf_copy = CVoronoiVolume(smooth1);
    CVoronoiVolume volume = pyramid_points.CreateVoronoiVolume();
    CMesh mesh = volume.MaskMesh(mask, "2", 1);
    cout << "second mesh" << endl;
    CMesh fullMesh = volume.ToMesh("3", 0.1);
    cout << "model" << endl;
//    fullMesh.Export(TEST_OUTPUT_PATH + "PyramidVoronoiVolumeTest", "obj");
    CModel model;
    model.AddMesh(mesh);
//    model.AddMesh(mesh);
    model.AddMesh(fullMesh);
    //mesh1.Reduce(0.3, 0.5);
//    anim1.SetScaleAnim(0,CPoint(3,3,3));
//    anim1.SetMoveAnim(0, CPoint(50, -50, 20));
    cout << "export" << endl;
    model.Export(TEST_OUTPUT_PATH + "PyramidVoronoiVolumeTest", "obj");
//    mesh1.ExportToObj(TEST_OUTPUT_PATH + "PyramidVivid");
//    CVoronoiVolume smooth3 = CVoronoiVolume(points, mask, quan, *min_element( quan.begin(), quan.end() ), *max_element( quan.begin(), quan.end()) );
//    smooth3.CreateSurface();
//    CMesh mesh3 = smooth3.ToMesh("vivid_3d_obj", 1.0);
//    mesh3.LaplacianSmooth(10, 0.7, 0);
//    mesh3.LaplacianSmooth(150, 0.25, 0.7);
//    mesh3.Reduce(0.25, 0.7);
//    mesh3.ExportToObj(TEST_OUTPUT_PATH + "/PyramidLaplacianSmooth_HC_reduce");
//    CModel model = CModel({mesh3, mesh1});
//    model.ExportToObj(TEST_OUTPUT_PATH + "/SmoothReduceComparison");
//    CVoronoiVolume smooth3 = CVoronoiVolume(points, mask, quan, *min_element( quan.begin(), quan.end() ), *max_element( quan.begin(), quan.end()) );
//    smooth3.CreateSurface();
//    smooth3.Smooth(3);
//    CMesh mesh3 = smooth3.ToMesh("vivid_3d_obj", 1.0);
//    mesh3.Reduce(0.4, 0.3);
//    mesh3.ExportToObj(TEST_OUTPUT_PATH + "/PyramidSmooth3");
//    CVoronoiVolume smooth6 = CVoronoiVolume(points, mask, quan, *min_element( quan.begin(), quan.end() ), *max_element( quan.begin(), quan.end()) );
//    smooth6.CreateSurface();
//    smooth6.Smooth(6);
//    CMesh mesh6 = smooth6.ToMesh("vivid_3d_obj", 1.0);
//    mesh6.Reduce(0.25, 0.3);
//    mesh6.ExportToObj(TEST_OUTPUT_PATH + "/PyramidSmooth6");
//    CVoronoiVolume smooth8 = CVoronoiVolume(points, mask, quan, *min_element( quan.begin(), quan.end() ), *max_element( quan.begin(), quan.end()) );
//    smooth8.CreateSurface();
//    smooth8.Smooth(8);
//    CMesh mesh8 = smooth8.ToMesh("vivid_3d_obj", 1.0);
//    mesh8.Reduce(0.2, 0.3);
//    mesh8.ExportToObj(TEST_OUTPUT_PATH + "/PyramidSmooth8");
    return EXIT_SUCCESS;

}

int RunMedicaneTest()
{
    Log(LOG_INFO, "Medicane Test:");
    ModelData nova;
    Log(LOG_INFO, "Running VIVID");

    nova = ReadBin(DATA_MODEL_PATH + "medicane.bin");
    vector<CPoint> points (nova.points.size());
    for (int j = 0; j < nova.quan.size(); j++) {
        nova.quan[j] = 0.5;
        points[j] = nova.points[j];
    }
    CVoronoiVolume surf = CVoronoiVolume(points, nova.quan, -15.1, 1.51);
    surf.CreateSurface();
    //surf.Smooth(false, 1);
    Log(LOG_INFO, "Convert to Mesh");
    CMesh mesh = surf.MaskMesh(nova.mask,"SurfMedicane", 1);
    //mesh.SubdivideLargeFaces(4);
//        mesh.RemovePointyFaces(20);
    mesh.LaplacianSmooth(10, 0.7, 0);
//    mesh.LaplacianSmooth(50, 0.4, 0.7);
//    mesh.RemovePointyFaces();
//    mesh.Reduce(0.1, 0.8);
    //mesh.ExportToObj(TEST_OUTPUT_PATH + "/Supernova_" + to_string(i));

    mesh.ExportToObj(TEST_OUTPUT_PATH + "/MedicaneModel");
    return EXIT_SUCCESS;

}

void assimpTest(string file){
    using namespace  Assimp;
    Importer imp;
    Exporter exp;
    const aiScene *scene = imp.ReadFile(std::string(file),
                                        aiProcess_CalcTangentSpace |
                                        aiProcess_JoinIdenticalVertices |
                                        aiProcess_SortByPType);

    aiNodeAnim** channels = scene->mAnimations[0]->mChannels;

    for(int i = 0; i != scene->mAnimations[0]->mNumChannels; i++){
        cout << "animation channel " << i << endl;
        cout << "Position Keys: " << endl;
        for(int P_key = 0; P_key != channels[i]->mNumPositionKeys; P_key++){
            cout << "Time - " << channels[i]->mPositionKeys[P_key].mTime;
            cout << " Value - " << channels[i]->mPositionKeys[P_key].mValue.x << " ";
            cout << channels[i]->mPositionKeys[P_key].mValue.y << " ";
            cout << channels[i]->mPositionKeys[P_key].mValue.z << endl;
        }
        cout << "Rotation Keys: " << endl;
        for(int P_key = 0; P_key != channels[i]->mNumRotationKeys; P_key++){
            cout << "Time - " << channels[i]->mRotationKeys[P_key].mTime;
            cout << " Value - " << channels[i]->mRotationKeys[P_key].mValue.w << " ";
            cout << channels[i]->mRotationKeys[P_key].mValue.x << " ";
            cout << channels[i]->mRotationKeys[P_key].mValue.y << " ";
            cout << channels[i]->mRotationKeys[P_key].mValue.z << endl;
        }
        cout << "Scaling Keys: " << endl;
        for(int P_key = 0; P_key != channels[i]->mNumScalingKeys; P_key++){
            cout << "Time - " << channels[i]->mScalingKeys[P_key].mTime;
            cout << " Value - " << channels[i]->mScalingKeys[P_key].mValue.x << " ";
            cout << channels[i]->mScalingKeys[P_key].mValue.y << " ";
            cout << channels[i]->mScalingKeys[P_key].mValue.z << endl;
        }
    }

    aiMaterial* mat = scene->mMaterials[1];
    aiMaterialProperty prop;

//    //big printing of stuff
//    aiString matString;
//    aiColor3D color(0,0,0);
//    int num1;
//    float float1;
//    mat->Get(AI_MATKEY_NAME, matString);
//    cout << "AI_MATKEY_NAME     "<< matString.C_Str() << endl;
//
//    mat->Get(AI_MATKEY_SHADING_MODEL, num1);
//    cout << "AI_MATKEY_SHADING_MODEL     "<< num1 << endl;
//
//    mat->Get("$mat.illum", 3, 0, num1);
//    cout << "mat.illum     "<< num1 << endl;
//
//    mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
//    cout << "AI_MATKEY_COLOR_AMBIENT     ";
//    cout << color.r << color.g << color.b << endl;
//
//    mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
//    cout << "AI_MATKEY_COLOR_DIFFUSE     ";
//    cout << color.r << color.g << color.b << endl;
//
//    mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
//    cout << "AI_MATKEY_COLOR_SPECULAR     ";
//    cout << color.r << color.g << color.b << endl;
//
//    mat->Get(AI_MATKEY_COLOR_EMISSIVE, color);
//    cout << "AI_MATKEY_COLOR_EMISSIVE     ";
//    cout << color.r << color.g << color.b << endl;
//
//
//    mat->Get(AI_MATKEY_COLOR_TRANSPARENT, color);
//    cout << "AI_MATKEY_COLOR_TRANSPARENT     ";
//    cout << color.r << color.g << color.b << endl;
//
//    mat->Get(AI_MATKEY_SHININESS, float1);
//    cout << "AI_MATKEY_SHININESS     "<< float1 << endl;
//
//    mat->Get(AI_MATKEY_OPACITY, float1);
//    cout << "AI_MATKEY_OPACITY     "<< float1 << endl;
//
//    mat->Get(AI_MATKEY_ANISOTROPY_FACTOR, float1);
//    cout << "AI_MATKEY_ANISOTROPY_FACTOR     "<< float1 << endl;
//
//    mat->Get(AI_MATKEY_REFRACTI, float1);
//    cout << "AI_MATKEY_REFRACTI     "<< float1 << endl;
//
//    mat->Get(AI_MATKEY_UVWSRC(aiTextureType_DIFFUSE, 0), num1);
//    cout << "AI_MATKEY_UVWSRC     "<< num1 << endl;
//
//    mat->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), matString);
//    cout << "AI_MATKEY_TEXTURE     "<< matString.C_Str() << endl;

    if (exp.Export(scene, "gltf2", TEST_OUTPUT_PATH + "TestAssimpAnim.gltf") != AI_SUCCESS) {
        cerr << exp.GetErrorString() << endl;
    }
}

int CubeAnimationTest()
{
    vector<CModel> models = {};
    vector<string> colors = {"Red", "Blue", "Green", "Purple", "Yellow", "Cyan", "White", "Black"};
    vector<CPoint> loc = {{0,0,0}, {1,0,0}, {2,0,0},{0,0,1},{1,0,1},{2,0,1},{0,0,2},{1,0,2}};
    for (int i = 0; i < colors.size(); i++) {
        CMesh box = CreateCubeMesh(loc[i]*3,0.5,colors[i],1,colors[i]);
        models.push_back((CModel(box)));
    }
    //CStopMotionAnimation anim = CStopMotionAnimation(models, 3);
    CAnimation anim = CAnimation(models);
    anim.SetDuration(150);
    anim.SetRotateAnim(0, CPoint(2.3,0,1));
    anim.SetRotateAnim(3, CPoint(0,2.3,1));
    anim.SetRotateAnim(2, CPoint(2.3,0,1));
    anim.SetRotateAnim(5, CPoint(2.3,2.3,0));
    anim.SetMoveAnim(2,CPoint(3,3,3));
    anim.SetMoveAnim(5,CPoint(3,3,3));
    anim.SetScaleAnim(6, CPoint(2,2,2));
    anim.SetScaleAnim(1, CPoint(5,0,0));
    CStopMotionAnimation SManim = CStopMotionAnimation(anim, 2);
    SManim.Export( TEST_OUTPUT_PATH + "/CubeAnimationTest", "gltf2");
    //AssimpExport::AnimationExporter(anim, "gltf2", TEST_OUTPUT_PATH + "/CubeAnimationTest");
    return EXIT_SUCCESS;
}
int main()
{
    ConfigLogging(true, LOG_DEBUG, false, "");
//    assimpTest(TEST_OUTPUT_PATH + "/sat_gal_anim.gltf");
    int ret_value = EXIT_SUCCESS;
//    Log(LOG_INFO, "MedicaneTestTest");
//    ret_value =RunMedicaneTest();
//    if ( EXIT_SUCCESS != ret_value ) return ret_value;
//    Log(LOG_INFO, "ParametricSurfByFuncTest");
//    ret_value = ParametricSurfByFuncTest();
//    if ( EXIT_SUCCESS != ret_value ) return ret_value;
    Log(LOG_INFO, "Testing All Shapes");
    ret_value = ShapesTest();
    if ( EXIT_SUCCESS != ret_value ) return ret_value;
//    Log(LOG_INFO, "Cube");
//    ret_value = CubeSurfTests();
//    if ( EXIT_SUCCESS != ret_value ) return ret_value;
//    Log(LOG_INFO, "Colors");
//    ret_value = ColorMapTest();
//    if ( EXIT_SUCCESS != ret_value) return ret_value;
//    Log(LOG_INFO, "Pyramid");
//    ret_value = PyramidSmoothTest();
//    if ( EXIT_SUCCESS != ret_value ) return ret_value;
//    cout << "Cube Animation" << endl;
//    ret_value = CubeAnimationTest();
//    if ( EXIT_SUCCESS != ret_value ) return ret_value;
//    ret_value = RemovePointyFacesTest();
//    if ( EXIT_SUCCESS != ret_value ) return ret_value;
    return EXIT_SUCCESS;
}