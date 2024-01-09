#!/usr/bin/env python
# 05/07
#pwm values were found from visual calibration of steering servo
import rospy
from keyboard_teleop.msg import drive_values
from sensor_msgs.msg import Joy

throttle = 0;
steer = 0;
pwm1 = 0; #add mid value
pwm2 = 0; 

pub = rospy.Publisher('drive_pwm', drive_values, queue_size=10)

joystick_throttle = 0
joystick_reverse = 0
joystick_steer = 0

def arduino_map(x, in_min, in_max, out_min, out_max):
	return (x - in_min) * (out_max - out_min) // (in_max - in_min) + out_min

def joy_callback(data):
	global joystick_throttle
	global joystick_reverse
	global joystick_steer
	joystick_throttle = data.axes[5]
	joystick_reverse = data.axes[2]
	joystick_steer = data.axes[0]
	print "joystick_throttle: ", joystick_throttle

def drive_pwm(event):
	global throttle
	global steer
	global pwm1
	global pwm2
	global pub
	if joystick_throttle <= 0.98 and joystick_reverse > 0.98  and throttle < 50: 
		throttle = (1 - joystick_throttle)*25
		pwm1 = arduino_map(throttle,-50,50,7196,11466)
		steer = -joystick_steer*50
		pwm2 = arduino_map(steer,-50,50,7196,11466)

	elif joystick_throttle > 0.98 and joystick_reverse <= 0.98 and throttle > -50:
		throttle = -(1 - joystick_reverse)*25
		pwm1 = arduino_map(throttle,-50,50,7196,11466)
		steer = -joystick_steer*50
		pwm2 = arduino_map(steer,-50,50,7196,11466)

	else:
		steer = -joystick_steer*50
		throttle = 0
		pwm1 = arduino_map(throttle,-50,50,7196,11466)
		pwm2 = arduino_map(steer,-50,50,7196,11466)			


	msg = drive_values()
	msg.pwm_drive = pwm1
	msg.pwm_angle = pwm2
	print msg.pwm_drive
	pub.publish(msg)

if __name__ == '__main__':
	rospy.init_node('keyboard_teleop', anonymous=True)
	rospy.Timer(rospy.Duration(0.01),drive_pwm)
	rospy.Subscriber("joy", Joy, joy_callback)
	rospy.spin()
