
#ifndef COLORMAP
#define COLORMAP

#include <vector>
#include <iostream>

using namespace std;
class Color_t 
{
public:
	double R;
	double G;
	double B;

	inline Color_t(double R, double G, double B) : R(R), G(G), B(B) {}
};

class ColorMap
{
private:
	vector<Color_t> color_map;
public:
	inline Color_t GetColor(float val) { return color_map[int(val*76)]; }
	inline ColorMap() 
	{
		color_map = vector<Color_t>();
		color_map.push_back(Color_t(0.050383, 0.029803, 0.52797));
		color_map.push_back(Color_t(0.086222, 0.026125, 0.54266));
		color_map.push_back(Color_t(0.11512, 0.023556, 0.55547));
		color_map.push_back(Color_t(0.14861, 0.021154, 0.57056));
		color_map.push_back(Color_t(0.17157, 0.019706, 0.58081));
		color_map.push_back(Color_t(0.20045, 0.017902, 0.59336));
		color_map.push_back(Color_t(0.2212, 0.016497, 0.60208));
		color_map.push_back(Color_t(0.2414, 0.014979, 0.61026));
		color_map.push_back(Color_t(0.2677, 0.012716, 0.62035));
		color_map.push_back(Color_t(0.28708, 0.010855, 0.6273));
		color_map.push_back(Color_t(0.31254, 0.008239, 0.6357));
		color_map.push_back(Color_t(0.33143, 0.006261, 0.64132));
		color_map.push_back(Color_t(0.35015, 0.004382, 0.6463));
		color_map.push_back(Color_t(0.3749, 0.002245, 0.65188));
		color_map.push_back(Color_t(0.3933, 0.001114, 0.6552));
		color_map.push_back(Color_t(0.41764, 0.000564, 0.65839));
		color_map.push_back(Color_t(0.43573, 0.001127, 0.6598));
		color_map.push_back(Color_t(0.45962, 0.003574, 0.66028));
		color_map.push_back(Color_t(0.47734, 0.00698, 0.65955));
		color_map.push_back(Color_t(0.49488, 0.01199, 0.65787));
		color_map.push_back(Color_t(0.51793, 0.021563, 0.65411));
		color_map.push_back(Color_t(0.53495, 0.031217, 0.65016));
		color_map.push_back(Color_t(0.55724, 0.047331, 0.64344));
		color_map.push_back(Color_t(0.57363, 0.060028, 0.63735));
		color_map.push_back(Color_t(0.58972, 0.072878, 0.63041));
		color_map.push_back(Color_t(0.61067, 0.090204, 0.61995));
		color_map.push_back(Color_t(0.62599, 0.10331, 0.6113));
		color_map.push_back(Color_t(0.64587, 0.1209, 0.59887));
		color_map.push_back(Color_t(0.66037, 0.13414, 0.58897));
		color_map.push_back(Color_t(0.67452, 0.14742, 0.57869));
		color_map.push_back(Color_t(0.69284, 0.16514, 0.56452));
		color_map.push_back(Color_t(0.70618, 0.17844, 0.55366));
		color_map.push_back(Color_t(0.72344, 0.19616, 0.53898));
		color_map.push_back(Color_t(0.73602, 0.20944, 0.52791));
		color_map.push_back(Color_t(0.75231, 0.22713, 0.51315));
		color_map.push_back(Color_t(0.76419, 0.2404, 0.50213));
		color_map.push_back(Color_t(0.7758, 0.25366, 0.49117));
		color_map.push_back(Color_t(0.79085, 0.27135, 0.47671));
		color_map.push_back(Color_t(0.80185, 0.28463, 0.46597));
		color_map.push_back(Color_t(0.81614, 0.30237, 0.45182));
		color_map.push_back(Color_t(0.82659, 0.31571, 0.44132));
		color_map.push_back(Color_t(0.8368, 0.3291, 0.4309));
		color_map.push_back(Color_t(0.85007, 0.34705, 0.41715));
		color_map.push_back(Color_t(0.85975, 0.36059, 0.40692));
		color_map.push_back(Color_t(0.8723, 0.37877, 0.39336));
		color_map.push_back(Color_t(0.88144, 0.39253, 0.38323));
		color_map.push_back(Color_t(0.89325, 0.41105, 0.36977));
		color_map.push_back(Color_t(0.90181, 0.42509, 0.35969));
		color_map.push_back(Color_t(0.9101, 0.43927, 0.34961));
		color_map.push_back(Color_t(0.92071, 0.45842, 0.33617));
		color_map.push_back(Color_t(0.92833, 0.47297, 0.32607));
		color_map.push_back(Color_t(0.93799, 0.49267, 0.31257));
		color_map.push_back(Color_t(0.94484, 0.50766, 0.30243));
		color_map.push_back(Color_t(0.95134, 0.52285, 0.29228));
		color_map.push_back(Color_t(0.95942, 0.54343, 0.2787));
		color_map.push_back(Color_t(0.96502, 0.55912, 0.26851));
		color_map.push_back(Color_t(0.97184, 0.58038, 0.25493));
		color_map.push_back(Color_t(0.97643, 0.59659, 0.24477));
		color_map.push_back(Color_t(0.98056, 0.61304, 0.23465));
		color_map.push_back(Color_t(0.9853, 0.63533, 0.22126));
		color_map.push_back(Color_t(0.98826, 0.65233, 0.21136));
		color_map.push_back(Color_t(0.99137, 0.67536, 0.19845));
		color_map.push_back(Color_t(0.99303, 0.69291, 0.18908));
		color_map.push_back(Color_t(0.99432, 0.71668, 0.17721));
		color_map.push_back(Color_t(0.99456, 0.73479, 0.16894));
		color_map.push_back(Color_t(0.99414, 0.75314, 0.1614));
		color_map.push_back(Color_t(0.9925, 0.77797, 0.15285));
		color_map.push_back(Color_t(0.99044, 0.79686, 0.14787));
		color_map.push_back(Color_t(0.98651, 0.8224, 0.14356));
		color_map.push_back(Color_t(0.98265, 0.84181, 0.1423));
		color_map.push_back(Color_t(0.97799, 0.86143, 0.14281));
		color_map.push_back(Color_t(0.97053, 0.8879, 0.14592));
		color_map.push_back(Color_t(0.96402, 0.90795, 0.14937));
		color_map.push_back(Color_t(0.95429, 0.93491, 0.15292));
		color_map.push_back(Color_t(0.9466, 0.95519, 0.15033));
		color_map.push_back(Color_t(0.94002, 0.97516, 0.13133));
	}
};


#endif