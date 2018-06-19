/**********************************************************
*	FileName: get_veh_position.cpp
*	Author	: LJJ
*	Date	: 2018.06.12
*	Describe: ...
**********************************************************/
#include <ros/ros.h>
#include <visualization/Ego.h>

ros::Publisher veh_position_pub;

visualization::Ego msg;

int count = 0;

void get_veh_position()
{
	double lat[] = {40.1007, 20.1023, 30.1027, 40.1031, 30.1035};
	double lon[] = {136.287, 106.286, 126.286, 116.285, 110.285};


	if(count >=0 && count <=2000)
	{
		msg.lat = lat[0];
		msg.lon = lon[0];
	}
	else if(count > 2000 && count <= 4000)
	{
		msg.lat = lat[1];
		msg.lon = lon[1];
	}
	else if(count > 4000 && count <= 6000)
	{
		msg.lat = lat[2];
		msg.lon = lon[2];
	}
	else if(count > 6000 && count <=8000)
	{
		msg.lat = lat[3];
		msg.lon = lon[3];
	}
	else if(count > 8000 && count <= 10000)
	{
		msg.lat = lat[4];
		msg.lon = lon[4];
	}
	else
		count = 0;
	veh_position_pub.publish(msg);
	count++;
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "get_veh_position");
	ros::NodeHandle n;
	veh_position_pub 	= n.advertise<visualization::Ego>("veh_positon", 1);
	ros::Rate loop_rate(10);

	while(1)
	{
		get_veh_position();
		ros::spinOnce();
		loop_rate.sleep();
	}	
	return 0;	
}
