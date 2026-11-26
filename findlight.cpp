#include"core.h"
vector<Light>findLight(Mat mask, int color)
{
	vector <Light>lightlist;
	vector<vector<Point>>contours;
	vector<Vec4i> hierarchy;
	findContours(mask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//cout << "ÂÖÀªÊý" << contours.size() << endl;
	for (int i = 0;i < contours.size();i++)
	{

		double area = contourArea(contours[i]);
		if (area < 60)
			continue;
		RotatedRect r_rect = minAreaRect(contours[i]);
		Size2f size = r_rect.size;
		float w = size.width;
		float h = size.height;
		float longside = max(w, h);
		float shortside = min(w, h);
		float ratio = longside / shortside;
		if (ratio < 3.0f||ratio>8)
			continue;

		Light bar;
		bar.rotate_rect = r_rect;
		bar.center = r_rect.center;
		bar.angle = r_rect.angle;
		bar.length = longside;
		bar.color = color;
		lightlist.push_back(bar);

	}
	return lightlist;

}