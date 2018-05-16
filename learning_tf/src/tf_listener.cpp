#include<ros/ros.h>
#include<geometry_msgs/PointStamped.h>
#include<tf/transform_listener.h>

void transformPoint(const tf::TransformListener &listener)
{
	geometry_msgs::PointStamped radar_coord;
	radar_coord.header.frame_id="corner_radar_LF";  //choose the propriate coordinate systems.
	radar_coord.header.stamp = ros::Time();
	//corner_radar_LF.point.x= rand()%5;
	//corner_radar_LF.point.y= rand()%5;
	//corner_radar_LF.point.z= rand()%5;
	radar_coord.point.x= 1;
	radar_coord.point.y= 2;
	radar_coord.point.z= 3;
	try
	{
		geometry_msgs::PointStamped vehicle_point;
		listener.transformPoint("vehicle",radar_coord,vehicle_point);

		ROS_INFO("RRradar_coord: (%.6f, %.6f. %.6f) -----> vehicle: (%.6f, %.6f, %.6f) at time %.6f",
		radar_coord.point.x, radar_coord.point.y, radar_coord.point.z,
		vehicle_point.point.x, vehicle_point.point.y, vehicle_point.point.z, vehicle_point.header.stamp.toSec());
	}
	catch(tf::TransformException& ex)
	{
		ROS_ERROR("Received an exception trying to transform a point from \"RRradar_coord\" to \"vehicle\": %s", ex.what());
	}
}

int main(int argc,char **argv)
{
	ros::init(argc,argv,"tf_listener");
	ros::NodeHandle n;
	tf::TransformListener listener(ros::Duration(10));
	ros::Timer timer=n.createTimer(ros::Duration(1.0),boost::bind(&transformPoint,boost::ref(listener)));
	ros::spin();
	return 0;
}