#pragma once

class Animation{
    
public:
    Animation();
    int importer(vector<char*> inputFiles);
    int exportet(char* outputFile);
    int keyFrameAnimation(vector<char*> inputFiles, char* outputFile);
    int cameraRotation(double x, double y, double z, int deg);
    ~Animation();
private:
    FbxScene* scene;
}