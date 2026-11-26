#include"core.h"
int main()
{
    string video_path = "C:/Users/omen/Desktop/opencvtest/outpost.avi";
    VideoCapture cap(video_path);
    if (!cap.isOpened())
    {
        cout << "错误：打不开视频文件！请检查路径：" << video_path << endl;
        return -1;
    }

    int frame_w = (int)cap.get(CAP_PROP_FRAME_WIDTH);
    int frame_h = (int)cap.get(CAP_PROP_FRAME_HEIGHT);
    double fps = cap.get(CAP_PROP_FPS);
    int total_frames = (int)cap.get(CAP_PROP_FRAME_COUNT);

    if (frame_w <= 0 || frame_h <= 0)
    {
        cout << "[警告] 读取画面尺寸失败，使用回退值 1280×1024" << endl;
        frame_w = FALLBACK_W;
        frame_h = FALLBACK_H;
    }
    if (fps < 1)
    {
        cout << "[警告] 读取帧率失败，使用回退值 " << FALLBACK_FPS << endl;
        fps = FALLBACK_FPS;
    }

    cout << "=== 视频信息 ===" << endl;
    cout << "宽度：" << frame_w << "  高度：" << frame_h << endl;
    cout << "帧率：" << fps << "  总帧数：" << total_frames << endl;

    if (frame_w != 1280 || frame_h != 1024)
    {
        cout << "错误：视频分辨率不是1280×1024，和相机标定参数不匹配！停止运行" << endl;
        return -1;
    }
    VideoWriter writer;
    bool write_ok = writer.open("result_video.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'),fps, Size(frame_w, frame_h));

    if (!write_ok)
    {
        cout << "警告：输出视频创建失败！" << endl;
    }
    
    ofstream csv_file("frame_data.csv");

    csv_file << "frameid,times,candidateid,"
        << "detectvalid,pnpvalid,"
        << "tx_m,ty_m,tz_m,distance_m,"
        << "rx_rad,ry_rad,rz_rad,reprojection_error_px" << endl;
    Mat frame;
    int frameid = 0;
    int count = 0;
    while (cap.read(frame))
    {
        if (frame.empty())
        {
            break;
        }
        double time = (double)frameid / fps;
        cout << "处理第" << frameid << "帧，时间:" << time << "s" << endl;
        Mat orangemask, bluemask;
        vector<Armor>armorlist = detect(frame, orangemask, bluemask);
        sort(armorlist.begin(), armorlist.end(), [](const Armor& a, const Armor& b)
            {
                if (a.center.x - b.center.x > 0)
                {
                    return a.center.x < b.center.x;
                }
                else
                {
                    return a.center.y < b.center.y;
                }
            });
        vector<vector<Point2d>>allprojectpts;
        vector<double>alldistance;
        vector<bool> allpnpvalid;
        for (int j = 0;j < armorlist.size();j++)
        {
            const Armor& armor = armorlist[j];
            bool detectvalid = armor.is_valid;
            bool pnpvalid = false;
            string tx_str = "", ty_str = "", tz_str = "", dist_str = "";
            string rx_str = "", ry_str = "", rz_str = "", err_str = "";
            Mat rvec, tvec;         
            double distance = 0.0;  
            double error = 0.0;
            vector<Point2d> projpts; 
            if (detectvalid)
            {
                vector<Point2d> imagepts;
                for (int i = 0; i < 4; i++)
                {
                    imagepts.push_back(Point2d(armor.corner[i].x, armor.corner[i].y));
                }
                pnpvalid = solvePnP(objectpoint, imagepts, CAMERA_K, CAMERA_D, rvec, tvec);
                if (pnpvalid)
                {
                    double tx = tvec.at<double>(0, 0);
                    double ty = tvec.at<double>(1, 0);
                    double tz = tvec.at<double>(2, 0);
                    distance = sqrt(tx * tx + ty * ty + tz * tz);
                    projectPoints(objectpoint, rvec, tvec, CAMERA_K, CAMERA_D, projpts);
                    error = caldeviation(imagepts, projpts);
                    tx_str = to_string(tx);
                    ty_str = to_string(ty);
                    tz_str = to_string(tz);
                    dist_str = to_string(distance);
                    rx_str = to_string(rvec.at<double>(0, 0));
                    ry_str = to_string(rvec.at<double>(1, 0));
                    rz_str = to_string(rvec.at<double>(2, 0));
                    err_str = to_string(error);
                }
            }
            allprojectpts.push_back(projpts);
            alldistance.push_back(distance);
            allpnpvalid.push_back(pnpvalid);
         
            csv_file << frameid << "," << time << "," << j << ","
                << (detectvalid ? 1 : 0) << "," << (pnpvalid ? 1 : 0) << ","
                << tx_str << "," << ty_str << "," << tz_str << "," << dist_str << ","
                << rx_str << "," << ry_str << "," << rz_str << "," << err_str << endl;
        }
        Mat drawframe = draw(frame, armorlist, allprojectpts, alldistance, allpnpvalid);
        if (write_ok)
            writer.write(drawframe);
        
        if (count % 120 == 0)
        {
            imwrite("debug_frame_" + to_string(frameid) + ".png", drawframe);
        }
        count++; 
        frameid++; 
    
    }
    cap.release();      
    writer.release();   
    csv_file.close();   
    
    cout << endl << "=== 全部处理完成！===" << endl;
    cout << "输出文件：" << endl;
    cout << "1. 标注视频：result_video.avi" << endl;
    cout << "2. 逐帧数据：frame_data.csv" << endl;
    cout << "3. 调试图片：debug_frame_xxx.png" << endl;
    return 0; 
}