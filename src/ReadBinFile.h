
#ifndef READBINFILE
#define READBINFILE

#include "Point.h"
#include <vector>
#include <iostream>
#include "fstream"


using namespace std;

class ModelData {
public:
	vector<CPoint> points;
	vector<bool> mask;
	vector<float> quan;
	inline ModelData() {}
};

class ModelDataMultipleMasks {
public:
	vector<CPoint> points;
	vector<vector<bool> > mask;
	vector<float> quan;
	inline ModelDataMultipleMasks() {}
};

inline static ModelData ReadBin(string file_loc) {
	ifstream binFile(file_loc, ios::in | ios::binary);
	float x, y, z; //will hold the temporary x, y ,z values
	float temp_quan; //will hold the temporary color values
	bool temp_mask; //will hold the temporary mask values
	int length; //will hold the length of the input
	ModelData output = ModelData();

	binFile.read((char*)&length, 4);

	for (int i = 0; i < length; i++) { //read the x, y, z data of the points
		binFile.read((char*)&x, 4);
		binFile.read((char*)&y, 4);
		binFile.read((char*)&z, 4);
		output.points.push_back(CPoint(x, y, z));
	}
	for (int i = 0; i < length; i++) { //read the mask values
		binFile.read((char*)&temp_mask, 1);
		output.mask.push_back(temp_mask);
	}
	for (int i = 0; i < length; i++) { //read the quan values
		binFile.read((char*)&temp_quan, 4);
		output.quan.push_back(temp_quan);
	}
	binFile.close();
	return output;
}


inline static ModelDataMultipleMasks ReadBinMultipleMasks(string file_loc) {
	ifstream binFile(file_loc, ios::in | ios::binary);
	float x, y, z; //will hold the temporary x, y ,z values
	float temp_quan; //will hold the temporary color values
	bool temp_mask; //will hold the temporary mask values
	int length; //will hold the length of the input
	int maskNum; //will hold the number of masks to read
	ModelDataMultipleMasks output = ModelDataMultipleMasks();

	binFile.read((char*)&length, 4);

	for (int i = 0; i < length; i++) { //read the x, y, z data of the points
		binFile.read((char*)&x, 4);
		binFile.read((char*)&y, 4);
		binFile.read((char*)&z, 4);
		output.points.push_back(CPoint(x, y, z));
	}
	for (int i = 0; i < length; i++) { //read the quan values
		binFile.read((char*)&temp_quan, 4);
		output.quan.push_back(temp_quan);
	}

	//binFile.read((char*)&maskNum, 4);

	for (int m = 0; m < 5; m++) {
		output.mask.push_back(vector<bool>());
		for (int i = 0; i < length; i++) { //read the mask values
			binFile.read((char*)&temp_mask, 1);
			output.mask[m].push_back(temp_mask);
		}
	}
	binFile.close();
	return output;
}


#endif