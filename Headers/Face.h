#include "Point.h"

class Face
{
private:
	Point * points; //holds this face points
	float quan; //holds the quen value of this face, this value will later be translated to the color of the face
	int c_points[2]; //holds the indexs for the two creating points of this face

public:
	Face(Point points[], float quan, int c_points[]);
	void removeFace(); //performs the facesub func for all is points, in order to delete himself from the record(mendatory if we want to delete him)
	float getQuan();
	void setQuan(float quen);
	Point getPoint(int index); // get the point in the given index(within this.points)
	int** getCpoints();//gets the creating points for this face
	~Face();
};