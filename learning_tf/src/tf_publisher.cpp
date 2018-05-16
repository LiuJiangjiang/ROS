#include<ros/ros.h>

#include<tf/transform_broadcaster.h>

int main(int argc,char **argv)

{

	ros::init(argc,argv,"tf_publisher");

	ros::NodeHandle n;

	//发布频率

	ros::Rate r(100);

	//创建一个transform_broadcaster

	tf::TransformBroadcaster broadcaster_corner_radar_LF, broadcaster_corner_radar_RF, broadcaster_corner_radar_LR, broadcaster_corner_radar_RR;

	//创建一个transform对象

	tf::Transform transform_corner_radar_LF, transform_corner_radar_RF, transform_corner_radar_LR, transform_corner_radar_RR;

	//下面设置其位置以及旋转信息,

	transform_corner_radar_LF.setOrigin(tf::Vector3(0.1,0.0,0.2));
	transform_corner_radar_RF.setOrigin(tf::Vector3(0.1,0.0,0.3));
	transform_corner_radar_LR.setOrigin(tf::Vector3(0.1,0.0,0.4));
	transform_corner_radar_RR.setOrigin(tf::Vector3(0.1,0.0,0.5));

	//再创建一个四元组

	tf::Quaternion q_corner_radar_LF, q_corner_radar_RF, q_corner_radar_LR, q_corner_radar_RR;

	q_corner_radar_LF.setRPY(0.0,0.0,1.5707963267948966/2);//子框架--> 父框架, clockwise +, and randian
	q_corner_radar_RF.setRPY(0.0,0.0,0.0);
	q_corner_radar_LR.setRPY(0.0,0.0,0.0);
	q_corner_radar_RR.setRPY(0.0,0.0,0.0);

	transform_corner_radar_LF.setRotation(q_corner_radar_LF);//以上已经把位置描述设定好了
	transform_corner_radar_RF.setRotation(q_corner_radar_RF);
	transform_corner_radar_LR.setRotation(q_corner_radar_LR);
	transform_corner_radar_RR.setRotation(q_corner_radar_RR);

	while(ros::ok())

	{

		broadcaster_corner_radar_LF.sendTransform(tf::StampedTransform(transform_corner_radar_LF,ros::Time::now(),"vehicle","corner_radar_LF"));//父框架叫vehicle,子框架叫radar(LF, RF, LR, RR)
		broadcaster_corner_radar_RF.sendTransform(tf::StampedTransform(transform_corner_radar_RF,ros::Time::now(),"vehicle","corner_radar_RF"));
		broadcaster_corner_radar_LR.sendTransform(tf::StampedTransform(transform_corner_radar_LR,ros::Time::now(),"vehicle","corner_radar_LR"));
		broadcaster_corner_radar_RR.sendTransform(tf::StampedTransform(transform_corner_radar_RR,ros::Time::now(),"vehicle","corner_radar_RR"));

		r.sleep();
	}
	return 0;
}