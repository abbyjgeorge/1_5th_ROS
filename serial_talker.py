#!/usr/bin/env python

import rospy
from keyboard_teleop.msg import drive_values
from keyboard_teleop.msg import drive_param

pub = rospy.Publisher('drive_pwm', drive_values, queue_size=10)

def arduino_map(x, in_min, in_max, out_min, out_max):
	return (x - in_min) * (out_max - out_min) // (in_max - in_min) + out_min

def callback(data):
	throttle = data.throttle
	steer = data.steer
	print("Throttle: ",throttle,"Steer: ",steer)
	pwm1 = arduino_map(throttle,-50,50,6554,13108);
	pwm2 = arduino_map(steer,-50,50,6554,13108);
	msg = drive_values()
	msg.pwm_drive = pwm1
	msg.pwm_angle = pwm2
	pub.publish(msg)

if __name__ == '__main__':
	print "Serial talker initialized"
	rospy.init_node('serial_talker', anonymous=True)
	rospy.Subscriber("drive_parameters", drive_param, callback)
	rospy.spin()

