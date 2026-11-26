#include"core.h"
vector<Armor>detect(const Mat& src_img, Mat& orangemask, Mat& bluemask)
{
    Scalar orangelow = Scalar(5, 70, 90);
    Scalar orangehigh = Scalar(40, 255, 255);
    Scalar bluelow = Scalar(95, 80, 80);
    Scalar bluehigh = Scalar(155, 255, 255);
    orangemask = ColorMask(src_img, orangelow, orangehigh);
    bluemask = ColorMask(src_img, bluelow, bluehigh);

    vector<Light> orange_bars = findLight(orangemask, 0);
    vector<Light> blue_bars = findLight(bluemask, 1);
    vector<Light> all_bars;
    all_bars.insert(all_bars.end(), orange_bars.begin(), orange_bars.end());
    all_bars.insert(all_bars.end(), blue_bars.begin(), blue_bars.end());
    vector<Armor> armors = MatchArmor(all_bars);
    //cout << "µÆÌõÊý" << all_bars.size() << endl;
    return armors;
}