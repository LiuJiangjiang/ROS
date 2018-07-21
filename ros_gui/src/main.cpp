/**
 * @file /src/main.cpp
 *
 * @brief Qt based gui.
 *
 * @date November 2010
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui>
#include <QApplication>
#include "../include/ros_gui/main_window.hpp"
#include <QtWebKit/QWebView>

/*****************************************************************************
** Main
*****************************************************************************/

int main(int argc, char **argv) {

    QApplication app(argc, argv);
    ros_gui::MainWindow w(argc,argv);
    w.show();
    app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
    QWebView *view = new QWebView;
    view->setUrl(QUrl("file:///home/jiangjiang/catkin_ws/src/ros_gui/sources/baidumap_1.html"));
    view->resize(700,550);
    view->show();
    return app.exec();
}
