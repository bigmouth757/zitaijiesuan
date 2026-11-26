#include"core.h"
double caldeviation(const vector<Point2d>& imgpt, const vector<Point2d>& solvept)
{
	double sumerror = 0;
	for (int i = 0; i<4 ;i++)
	{
		double dx = imgpt[i].x - solvept[i].x;
		double dy = imgpt[i].y - solvept[i].y;
		sumerror += sqrt(dx * dx + dy * dy);
	}
	return sumerror / 4;
}