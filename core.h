#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>
#include<string>
#include<algorithm>
#include<fstream>
#include<sstream>
#include<direct.h>
#include<cmath>
#include<opencv2/calib3d.hpp>
#include<algorithm>
using namespace cv;
using namespace std;
const Mat CAMERA_K = (Mat_<double>(3, 3) <<
	1769.321404, 0, 762.985097,
	0, 1768.461805, 572.3718881,
	0, 0, 1);
const Mat CAMERA_D = (Mat_<double>(1, 5) <<
	0.1301456438, -3.237695144,
	-0.0007849919824, 0.001116547072, 17.43296795);
// ---------- 装甲板真实物理尺寸（单位：米！必须用米，不能用毫米）----------
const double ARMOR_W = 0.135;  // 宽度 13.5厘米
const double ARMOR_H = 0.056;  // 高度 5.6厘米
// ---------- 视频回退参数（OpenCV读不到属性时用）----------
const int FALLBACK_W = 1280;
const int FALLBACK_H = 1024;
const double FALLBACK_FPS = 120.052976;
// ---------- 灯条筛选阈值（沿用你的代码，可微调）----------
const int minlightarea = 15;     // 灯条最小面积
const float minratio = 2.5f;      // 最小长宽比
const float maxratio = 9.0f;      // 最大长宽比
// ---------- 重投影误差阈值（大于这个值认为结果可疑）----------
const double deviation = 3.0;
// ---------- 3D模型点（装甲板坐标系，原点在中心，z=0）----------
// 顺序严格：左上、右上、右下、左下，和图像角点一一对应
const vector<Point3d> objectpoint = {
	{ -ARMOR_W / 2, -ARMOR_H / 2, 0 },
	{ ARMOR_W / 2, -ARMOR_H / 2, 0 },
	{ ARMOR_W / 2,  ARMOR_H / 2, 0 },
	{ -ARMOR_W / 2,  ARMOR_H / 2, 0 },
};
struct Light
{
	RotatedRect rotate_rect;
	Point2f center;
	float angle;
	float length;
	int color;
};
struct Armor
{
	int color;
	Rect2f bounding;
	Point2f center;
	Point2f corner[4];
	bool is_valid;
};
Mat ColorMask(const Mat& src, Scalar low, Scalar high);
vector<Light>findLight(Mat mask, int color);
vector<Armor>MatchArmor(vector<Light>& all_light);
Mat draw(const Mat& src, const vector<Armor>& armor_list,const vector<vector<Point2d>>& projectpoints, const vector<double>& distance, const vector<bool>pnpvalid);
vector<Armor>detect(const Mat& src_img, Mat& orangemask, Mat& bluemask);
double caldeviation(const vector<Point2d>& imgpt, const vector<Point2d>& solvept);