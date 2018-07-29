class Point
{
private:
	float x;
	float y;
	float z;
	int index; // the index of the point when she is writen to the obj file
	int faces; // the number of faces isong this point(allows us to easly delete unused points)

public:
	Point(float x, float y, float z); // defines a new point (index = null, faces = 0)
	float find_dis(Point P); //find the distant between this point and the one given
	float getX();
	float getY();
	float getZ();
	float getIndex();
	float getFaces();
	void setX(int x);
	void setY(int y);
	void setZ(int z);
	void setIndex(int num);
	void facesAdd(); // add 1 to the amount of faces that use this point
	void facesSub(); // sub 1 fron the amount of faces that use this point
	bool isPointDel(); // return true if the point should be deleted, and otherwise it returns false
	~Point();
};
