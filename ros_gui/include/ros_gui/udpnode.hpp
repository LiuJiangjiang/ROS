/**
 * @file /include/ros_gui/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef ros_gui_UDPNODE_HPP_
#define ros_gui_UDPNODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ros/ros.h>
#include <string>
#include <QThread>
#include <QStringListModel>
#include "Ego.h"


namespace ros_gui
{
	class UDPNode : public QThread
	{
		Q_OBJECT
	public:
		UDPNode(int argc, char** argv);
		virtual ~UDPNode();
		bool init();
		bool init(const std::string &master_url, const std::string &host_url);
    void run();
    enum LogLevel
		{
      Debug,
      Info,
      Warn,
      Error,
      Fatal
		};

    QStringListModel* loggingModel() {return &logging_model;}
    QStringListModel* loggingModel_udp() {return &logging_model_udp;}
    //void log(const LogLevel &level, const std::string &msg);
    void log_udp(const LogLevel &level, const std::string &msg);
     void udp_callback(const visualization::EgoConstPtr& submsg);

	Q_SIGNALS:
		void loggingUpdated();
    void rosShutdown();
    void loggingUpdated_udp();
  private:
  		int init_argc;
  		char** init_argv;
  		ros::Publisher udp_publisher;
      ros::Subscriber udp_subscriber;
  		QStringListModel logging_model;
      QStringListModel logging_model_udp;
	};
}

#endif /*ros_gui_UDPNODE_HPP_ */
