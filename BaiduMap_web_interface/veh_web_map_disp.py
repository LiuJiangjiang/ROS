#!/usr/bin/env python

#from selenium import webdriver
#from selenium.webdriver.chrome.options import Options

from __future__ import print_function
import urllib

import rospy
import roslib
roslib.load_manifest("visualization")
import sys
import cv2
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError


from visualization.msg import Ego

ss = 'http://api.map.baidu.com/staticimage/v2?ak=hK1vUDiX7NVayGcV3SdK1QIY38bcfPhW&mcode=666666&center'
picName = '/home/jiangjiang/catkin_ws/src/visualization/pics/veh_loaction.png'

def veh_web_map_callback(data):
	lon = data.lon
	lat = data.lat
	print (lat)
	url = ss + str(lon) + ',' + str(lat) + '&width=300&height=200&zoom=15&scale=2&markers=' + str(lon) + ',' + str(lat) + '&markerStyles=l'
	urllib.urlretrieve(url,picName)
	web_map_pub = rospy.Publisher("disp_veh_positon_map", Image)
	img = cv2.imread(picName)
	web_map_pub.publish(CvBridge().cv2_to_imgmsg(img, "bgr8"))

def listener():
	rospy.init_node('veh_web_map_disp', anonymous = True)
	rospy.Subscriber("veh_positon", Ego, veh_web_map_callback)
	rospy.spin()

if __name__=='__main__':
	listener()

#url = 'http://api.map.baidu.com/staticimage/v2?ak=hK1vUDiX7NVayGcV3SdK1QIY38bcfPhW&mcode=666666&center116.403874,39.914888&width=300&height=200&zoom=15&scale=2&markers=116.403874,39.9148881&markerStyles=l'

##################### Methond 1 #####################
#picName = "hello.png"
#chrome_options = Options()
#chrome_options.add_argument('--headless')
#chrome_options.add_argument('--disable-gpu')
#driver = webdriver.Chrome(chrome_options=chrome_options)
#driver.get(url)
#driver.maximize_window()
#driver.save_screenshot(picName)
#driver.close()

##################### Methond 2 #####################
#urllib.urlretrieve(url,'test.png')
