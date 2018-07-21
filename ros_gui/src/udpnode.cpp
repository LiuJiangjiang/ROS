#include <ros/ros.h>
#include <ros/network.h>
#include <string>
#include <std_msgs/String.h>
#include <sstream>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>



#include "../include/ros_gui/qnode.hpp"
#include "../include/ros_gui/udpnode.hpp"
#include "../include/ros_gui/Ego.h"


namespace ros_gui
{
  unsigned int char2hex(unsigned char data)
  {
      unsigned int a;
      switch(data)
      {
          case '0':
          {
              a = 0;
              break;
          }

          case '1':
          {
              a = 1;
              break;
          }

          case '2':
          {
              a = 2;
              break;
          }

          case '3':
          {
              a = 3;
              break;
          }


          case '4':
          {
              a = 4;
              break;
          }

          case '5':
          {
              a = 5;
              break;
          }

          case '6':
          {
              a = 6;
              break;
          }

          case '7':
          {
              a = 7;
              break;
          }

          case '8':
          {
              a = 8;
              break;
          }

          case '9':
          {
              a = 9;
              break;
          }

          case 'a':
          {
              a = 10;
              break;
          }

          case 'b':
          {
              a = 11;
              break;
          }

          case 'c':
          {
              a = 12;
              break;
          }

          case 'd':
          {
              a = 13;
              break;
          }

          case 'e':
          {
              a = 14;
              break;
          }

          case 'f':
          {
              a = 15;
              break;
          }
      }
      return a;
  }

  unsigned char string2char(unsigned char* data_string)
  {
      unsigned char a = 0;
      unsigned char fou = 4;
      unsigned char fiv = 5;
      if(strlen((char*)data_string) == fou)
      {
          a = char2hex(data_string[2]);
      }
      else if(strlen((char*)data_string) == fiv)
      {
          a = 16*char2hex(data_string[2]) + char2hex(data_string[3]);
      }
      else
          printf("errrrrrrrrrrrrrrrrrrrrrrrrrror");
      return a;
  }



	UDPNode::UDPNode(int argc, char** argv):
		init_argc(argc),
    init_argv(argv)
		{}

	UDPNode::~UDPNode()
	{
		if(ros::isStarted())
		{
			ros::shutdown();
			ros::waitForShutdown();
		}
		wait();
	}

	bool UDPNode::init()
	{
		ros::init(init_argc, init_argv, "ros_gui");
		if(!ros::master::check())
		{
			return false;
		}
		ros::start();
		ros::NodeHandle n;
    udp_publisher     = n.advertise<visualization::Ego>("udp", 1000);
    udp_subscriber    = n.subscribe("chatter", 1000, &UDPNode::udp_callback, this);
		start();
    return true;
	}

	bool UDPNode::init(const std::string &master_url, const std::string &host_url)
	{
		std::map<std::string, std::string> remappings;
		remappings["__master"] = master_url;
		remappings["__hostname"] = host_url;
		ros::init(remappings, "ros_gui");
		if(!ros::master::check())
		{
			return false;
		}
		ros::start();
		ros::NodeHandle n;
    udp_publisher     = n.advertise<visualization::Ego>("udp", 1000);
    udp_subscriber    = n.subscribe("chatter", 1000, &UDPNode::udp_callback, this);
		start();
    return true;
	}

	void UDPNode::run()
	{
		ros::Rate loop_rate(1);
		int count = 0;
    int sockfd = socket(AF_INET, SOCK_DGRAM,0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1324);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

		while ( ros::ok() ) 
		{
        unsigned char buf[1442] = {0}, buffer[1442] = {0};
        char filename[500]={0}, index[5] = {0};
        if(count < 5006)
        {
            FILE *fp;
            char filename[300]={0}, index[5] = {0};
            strcpy(filename, "/home/jiangjiang/catkin_ws/src/socket_server_client/can_eth_data/can_eth_data");
            strcat(filename, "_");
            snprintf(index, sizeof(index), "%d", count);
            strcat(filename, index);
            strcat(filename, ".txt");
            fp = fopen(filename,"r");
            int j = 0;

            while(!feof(fp))
            {
                fgets((char*)buf, 1442, fp);
                buffer[j++] = string2char(buf);
            }
            fclose(fp);
        }
        else
            count = 0;
        count++;
        sendto(sockfd,&buffer,sizeof(buffer),0,(struct sockaddr*)&addr,sizeof(addr));

        std::ostringstream msg;
        msg << "I sent udp package: " << count;
        log_udp(Info,std::string(msg.str()));
        ros::spinOnce();
        loop_rate.sleep();

		}
		std::cout << "Ros shutdown, proceeding to close the gui." << std::endl;
    //Q_EMIT rosShutdown();
	}

  void UDPNode::udp_callback(const visualization::EgoConstPtr& submsg)
  {
    std::ostringstream msg;
    msg << "I heard: "<<submsg->lat;
    log_udp(Info,std::string(msg.str()));
  }

  void UDPNode::log_udp(const LogLevel &level, const std::string &msg)
	{
    logging_model_udp.insertRows(logging_model_udp.rowCount(),1);
		std::stringstream logging_model_msg;
		switch ( level ) 
		{
			case(Debug) : 
			{
					ROS_DEBUG_STREAM(msg);
					logging_model_msg << "[DEBUG] [" << ros::Time::now() << "]: " << msg;
					break;
			}
			case(Info) : 
			{
					ROS_INFO_STREAM(msg);
					logging_model_msg << "[INFO] [" << ros::Time::now() << "]: " << msg;
					break;
			}
			case(Warn) : 
			{
					ROS_WARN_STREAM(msg);
					logging_model_msg << "[INFO] [" << ros::Time::now() << "]: " << msg;
					break;
			}
			case(Error) : 
			{
					ROS_ERROR_STREAM(msg);
					logging_model_msg << "[ERROR] [" << ros::Time::now() << "]: " << msg;
					break;
			}
			case(Fatal) : 
			{
					ROS_FATAL_STREAM(msg);
					logging_model_msg << "[FATAL] [" << ros::Time::now() << "]: " << msg;
					break;
			}
		}
		QVariant new_row(QString(logging_model_msg.str().c_str()));
    logging_model_udp.setData(logging_model_udp.index(logging_model_udp.rowCount()-1),new_row);
    Q_EMIT loggingUpdated_udp();
	}
}
