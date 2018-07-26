#include "Point.h"

class Face{
	
private:
	Point * points; //holds this face points
	float quan; //holds the quen value of this face, this value will later be translated to the color of the face
	int c_points[2]; //holds the indexs for the two creating points of this face

public:
	Face(Point points[], float quan, int c_points[]);
	void remove_face(); //performs the facesub func for all is points, in order to delete himself from the record(mendatory if we want to delete him)
	float getQuan();
	void setQuan(float quan);
	Point getPoint(int index); // get the point in the given index(within this.points)
	int[] getC_points();//gets the creating points for this face
	~Face();
};