#include "core.h"
Mat draw(const Mat& src, const vector<Armor>& armor_list,const vector<vector<Point2d>>&projectpoints,const vector<double>&distance,const vector<bool>pnpvalid)

{
    Mat res_img = src.clone();
    for (int k =0;k<armor_list.size();k++)
    {
        const Armor ar = armor_list[k];
        if (!ar.is_valid) continue;
        
        line(res_img, ar.corner[0], ar.corner[1], Scalar(0, 0, 255), 2);
        line(res_img, ar.corner[1], ar.corner[2], Scalar(0, 0, 255), 2);
        line(res_img, ar.corner[2], ar.corner[3], Scalar(0, 0, 255), 2);
        line(res_img, ar.corner[3], ar.corner[0], Scalar(0, 0, 255), 2);
        
        circle(res_img, ar.center, 4, Scalar(255, 0, 0), -1);
        
        for (int i = 0;i < 4;i++)
        {
            circle(res_img, ar.corner[i], 3, Scalar(0, 255, 255), -1);
            putText(res_img, to_string(i), ar.corner[i],
                FONT_HERSHEY_SIMPLEX, 0.45, Scalar(0, 255, 255), 1);
        }
        if (pnpvalid[k])
        {
            for (int j = 0; j < 4; j++)
            {
                circle(res_img, projectpoints[k][j], 3, Scalar(0, 0, 255), -1);
            }
            string text = "id:" + to_string(k) + " dist:" +
                to_string(distance[k]).substr(0, 4) + "m";
            putText(res_img, text, Point(20, 40 + k * 35), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 255, 0), 2);
        }
    }
    return res_img;
}