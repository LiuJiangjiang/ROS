#include <ros/ros.h>  
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <visualization/Traffic_info.h>
#include <sstream>
using namespace cv;

/*int main(int argc, char** argv)  
{  
	ros::init(argc, argv, "img_pub");
	ros::NodeHandle n;
	image_transport::ImageTransport it(n);
	image_transport::Publisher pub = it.advertise("camera/image", 1);
	ros::Rate loop_rate(10);

	cv::Mat image = cv::imread("/home/jiangjiang/catkin_ws/src/visualization/src/Selection_004.png", CV_LOAD_IMAGE_ANYCOLOR);
	cv::Mat imageDst;
	int height = 150;
	int width = 120;
	resize(image, imageDst, Size(width,height));
	sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", imageDst).toImageMsg();

	while(1)
	{
		pub.publish(msg);
		ros::spinOnce();
		loop_rate.sleep();
	}
}*/

image_transport::Publisher traffic_light_pub;
image_transport::Publisher speed_limit_pub;
ros::Subscriber traffic_info_sub;

void traffic_info_callback(const visualization::Traffic_info::ConstPtr& msg)
{  
	std::stringstream ss;
	ss << "/home/jiangjiang/catkin_ws/src/visualization/mdl/";
	int height = 150;
	int width = 50;
	int height_sl = 150;
	int width_sl = 120;
	cv::Mat image, imageDst, image_sl, imageDst_sl; 
	//printf("%d\n",msg->traffic_light_flag);
	if(msg->traffic_light_flag == 1)
	{
		switch(msg->traffic_light_color)
		{
			case 1:
			{
				ss << "traffic_light/red.png";
				break;
			}
			case 2:
			{
				ss << "traffic_light/green.png";
				break;
			}
			case 3:
			{
				ss << "traffic_light/yellow.png";
				break;
			}
		}
		image = cv::imread(ss.str(), CV_LOAD_IMAGE_ANYCOLOR);
		resize(image, imageDst, Size(width,height));
		sensor_msgs::ImagePtr image_msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", imageDst).toImageMsg();
		traffic_light_pub.publish(image_msg);
	}
	else
	{
		ss << "traffic_light/none.png";
		image = cv::imread(ss.str(), CV_LOAD_IMAGE_ANYCOLOR);
		height = 150;
		width = 50;
		resize(image, imageDst, Size(width,height));
		sensor_msgs::ImagePtr image_msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", imageDst).toImageMsg();
		traffic_light_pub.publish(image_msg);
	}

	if(msg->speed_limit_flag == 1)
	{
		//printf("%f\n",msg->speed_limit);
		std::stringstream st;
		st << "/home/jiangjiang/catkin_ws/src/visualization/mdl/speed_limit/"<<(float)msg->speed_limit<<".png";
		image_sl = cv::imread(st.str(), CV_LOAD_IMAGE_ANYCOLOR);
		resize(image_sl, imageDst_sl, Size(width_sl,height_sl));
		sensor_msgs::ImagePtr image_msg_sl = cv_bridge::CvImage(std_msgs::Header(), "bgr8", imageDst_sl).toImageMsg();
		speed_limit_pub.publish(image_msg_sl);
	}
	else
	{
		std::stringstream st;
		st << "/home/jiangjiang/catkin_ws/src/visualization/mdl/speed_limit/"<<"none.png";
		image_sl = cv::imread(st.str(), CV_LOAD_IMAGE_ANYCOLOR);
		resize(image_sl, imageDst_sl, Size(width_sl,height_sl));
		sensor_msgs::ImagePtr image_msg_sl = cv_bridge::CvImage(std_msgs::Header(), "bgr8", imageDst_sl).toImageMsg();
		speed_limit_pub.publish(image_msg_sl);
	}
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "img_pub");
	ros::NodeHandle n;
	image_transport::ImageTransport it(n);
	traffic_light_pub = it.advertise("traffic_light", 1);
	speed_limit_pub	  = it.advertise("speed_limit", 1);
	//ros::Rate loop_rate(10);


	traffic_info_sub = n.subscribe("traffic_info_pub",10, traffic_info_callback);
	ros::spin();

}
