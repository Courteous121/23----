#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <visualization_msgs/Marker.h>


using namespace cv;
using namespace std;

vector<Vec4i> hierarchy;
Mat imghsv, mask;
vector<vector<Point>> t1;

float line_orientation(Point A1, Point A2, Point B1, Point B2, int flag)
{
    if ((A2.x != A1.x) && (B2.x != B1.x))
    {
        float k1 = (A2.y - A1.y) / (A2.x - A1.x);
        float k2 = (B2.y - B1.y) / (B2.x - B1.x);
        float tan_k = 0;
        tan_k = (k2 - k1) / (1 + k2 * k1);
        float line_arctan = atan(tan_k);

        if (flag == 0)
        {
            return line_arctan;
        }
        else
        {
            return line_arctan * 180.0 / 3.1415926;
        }
    }
    else
        return 0;
}


int main(int argc, char** argv) {


    ros::init(argc, argv, "all_node");
    ros::NodeHandle nh;
    ros::Rate r(1);

    image_transport::ImageTransport it(nh);
    image_transport::Publisher pub = it.advertise("camera/image", 1);
    ros::Publisher marker_pub = nh.advertise<visualization_msgs::Marker>("visualization_marker", 1);
    uint32_t shape = visualization_msgs::Marker::CUBE;
	VideoCapture capture;
 visualization_msgs::Marker marker;
 marker.header.frame_id = "my_frame";
 marker.header.stamp = ros::Time::now();
 marker.ns = "basic_shapes";
    marker.id = 0;
marker.type = shape;
marker.action = visualization_msgs::Marker::ADD;
 marker.pose.position.x = 0;
    marker.pose.position.y = 0;
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;
marker.scale.x = 1.0;
    marker.scale.y = 1.0;
    marker.scale.z = 1.0;
marker.color.r = 0.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;
    marker.lifetime = ros::Duration();
    
while (marker_pub.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("Please create a subscriber to the marker");
      sleep(1);
    }
    marker_pub.publish(marker);
switch (shape)
    {
    
    case visualization_msgs::Marker::CYLINDER:
      shape = visualization_msgs::Marker::CUBE;
      break;
    }
r.sleep();


capture.open("/root/task3/1.avi"); 

	if (!capture.isOpened()) {
		printf("could not load video data...\n");
		return -1;
	}


	int frames = capture.get(CAP_PROP_FRAME_COUNT);//获取视频针数目(一帧就是一张图片)
	double fps = capture.get(CAP_PROP_FPS);//获取每针视频的频率
	// 获取帧的视频宽度，视频高度
	Size size = Size(capture.get(CAP_PROP_FRAME_WIDTH), capture.get(CAP_PROP_FRAME_HEIGHT));
	cout << frames << endl;
	cout << fps << endl;
	cout << size << endl;
	// 创建视频中每张图片对象
	Mat frame;


    sensor_msgs::ImagePtr msg;

    ros::Rate loop_rate(10);//以10ms间隔发送图片
    while (nh.ok()) {
        capture >> frame;  

         
cvtColor(frame, imghsv, COLOR_BGR2HSV);

        Scalar lower(0, 67, 66);
        Scalar upper(39, 219, 255);
        inRange(imghsv, lower, upper, mask);
        findContours(mask, t1, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
vector<RotatedRect> rectbox;
        for (int i = 0; i < t1.size(); i++)
        {
            RotatedRect rect = minAreaRect(t1[i]);
            Point2f vertices[4];
            rect.points(vertices);
            Rect brect = rect.boundingRect();
            double m5 = rect.size.width > rect.size.height ? rect.size.width : rect.size.height;
            double n5 = rect.size.width < rect.size.height ? rect.size.width : rect.size.height;
            double m = m5 / n5;
            double n = brect.height / brect.width;
            if (m > 3 && m < 8 && n > 0 && n < 6 && brect.area() > 50 && rect.size.area() > 50)
            {
         
                rectbox.push_back(rect);
            }
        }
	for (int i = 0; i < static_cast<int>(rectbox.size()) - 1; i++)
        {
            Point2f vertices1[4];
            Point2f vertices2[4];
            rectbox[i].points(vertices1);
            rectbox[i + 1].points(vertices2);

            double x1 = (vertices1[1].x + vertices1[2].x) / 2;
            double y1 = (vertices1[1].y + vertices1[2].y) / 2;
            double x2 = (vertices2[0].x + vertices2[3].x) / 2;
            double y2 = (vertices2[0].y + vertices2[3].y) / 2;
            double x3 = (vertices1[0].x + vertices1[3].x) / 2;
            double y3 = (vertices1[0].y + vertices1[3].y) / 2;
            double x4 = (vertices2[1].x + vertices2[2].x) / 2;
            double y4 = (vertices2[1].y + vertices2[2].y) / 2;

            float a = abs(line_orientation(Point(x1,y1), Point(x4,y4), Point(x4,y4), Point(x2,y2), 1));
            cout << a << endl;

            float m1 = rectbox[i].size.height > rectbox[i].size.width ? rectbox[i].size.height : rectbox[i].size.width;
            float n1 = sqrt(pow((vertices1[3].x - vertices2[3].x), 2) + pow((vertices1[3].y - vertices2[3].y), 2));
            float m = n1 / m1;
            float n = n1 * m1;
            double m2 = abs(vertices2[3].x - vertices1[1].x);
            double n2 = abs(vertices2[3].y - vertices1[1].y);
            double m0 = m2 / n2;
            if (m > 2 && m < 5 && n < 150000 && n > 200 && m0 < 10 )
            {

                // rectangle(img, vertices1[1], vertices2[3], Scalar(255, 255, 255), 2);
                line(frame, vertices1[1], vertices2[3], Scalar(0, 0, 255), 1);
                line(frame, vertices1[3], vertices2[1], Scalar(0, 0, 255), 1);
                double x1 = (vertices1[1].x + vertices2[3].x) / 2;
                double y1 = (vertices1[1].y + vertices2[3].y) / 2;
                //circle(img, Point(x1, y1), 5, Scalar(0, 255, 255), -1);


            }
msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();


pub.publish(msg);

        ROS_INFO("runnning!");
        ros::spinOnce();  
        loop_rate.sleep();//与ros::Rate loop_rate相对应,休息10ms
    }
}
}

