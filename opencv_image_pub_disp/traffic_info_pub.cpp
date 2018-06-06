#include <ros/ros.h>
#include <visualization/Traffic_info.h>

int main(int argc, char** argv)
{
	ros::init(argc, argv, "traffic_info_pub");
	ros::NodeHandle n;
	ros::Publisher pub = n.advertise<visualization::Traffic_info>("traffic_info_pub", 1);
	ros::Rate loop_rate(10);
	int count = 0;

	while(ros::ok())
	{
		visualization::Traffic_info msg;
		if(count > 100 && count < 150)
		{
			msg.traffic_light_flag = 1;
			msg.traffic_light_color = 1;
			msg.speed_limit_flag = 1;
			msg.speed_limit = 80.0;
		}
		else if (count > 300 && count < 350)
		{
			msg.traffic_light_flag = 1;
			msg.traffic_light_color = 2;
			msg.speed_limit_flag = 0;
			msg.speed_limit = 80.0;
		}
		else if (count > 500 && count < 550)
		{
			msg.traffic_light_flag = 0;
			msg.traffic_light_color = 3;
			msg.speed_limit_flag = 1;
			msg.speed_limit = 80.0;
		}
		else
		{
			msg.traffic_light_flag = 0;
			msg.traffic_light_color = 3;
			msg.speed_limit_flag = 0;
			msg.speed_limit = 80.0;
						
		}
		if(count > 1000)
			count = 0;
		pub.publish(msg);
		ros::spinOnce();

		loop_rate.sleep();
		++count;
	}
	return 0;	
}	
