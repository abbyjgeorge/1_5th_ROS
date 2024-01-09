#!/usr/bin/env python
# 05/07
#pwm values were found from visual calibration of steering servo
import rospy
from keyboard_teleop.msg import drive_values
import curses

throttle = 0;
steer = 0;
pwm1 = 0; #add mid value
pwm2 = 0; 

stdscr = curses.initscr()
curses.cbreak()
stdscr.keypad(1)

pub = rospy.Publisher('drive_pwm', drive_values, queue_size=10)

def arduino_map(x, in_min, in_max, out_min, out_max):
	return (x - in_min) * (out_max - out_min) // (in_max - in_min) + out_min

if __name__ == '__main__':
	stdscr.refresh()
	print "Use arrow keys to move, Del key to stop"
	rospy.init_node('keyboard_teleop', anonymous=True)
	stdscr.addstr(2, 20, "Throttle ")
	stdscr.addstr(2, 40, "PWM ")
	stdscr.addstr(3, 20, "Steer ")
	stdscr.addstr(3, 40, "PWM ")
	key = ''
	while key != ord('q'):
		key = stdscr.getch()
		stdscr.refresh()

		if key == curses.KEY_UP and throttle < 50: 
			throttle = throttle + 0.5
			pwm1 = arduino_map(throttle,-50,50,0,180)
			stdscr.addstr(2, 30, '%.2f ' % throttle)
			stdscr.addstr(2, 45, '%d ' % pwm1)
		elif key == curses.KEY_DOWN and throttle > -50:
			throttle = throttle - 0.5
			pwm1 = arduino_map(throttle,-50,50,0,180)
			stdscr.addstr(2, 30, '%.2f ' % throttle)
			stdscr.addstr(2, 45, '%d ' % pwm1)
		if key == curses.KEY_LEFT and steer > -50:
			steer = steer - 0.5
			pwm2 = arduino_map(steer,-50,50,0,180)
			stdscr.addstr(3, 30, '%.2f ' % steer)
			stdscr.addstr(3, 45, '%d ' % pwm2)
		elif key == curses.KEY_RIGHT and steer < 50:
			steer = steer + 0.5
			pwm2 = arduino_map(steer,-50,50,0,180)
			stdscr.addstr(3, 30, '%.2f ' % steer)
			stdscr.addstr(3, 45, '%d ' % pwm2)
		if key == curses.KEY_DC:
			steer = 0
			throttle = 0
			pwm1 = arduino_map(throttle,-50,50,0,180)
			pwm2 = arduino_map(steer,-50,50,0,180)			
			stdscr.addstr(2, 30, '%.2f ' % throttle)
			stdscr.addstr(3, 30, '%.2f ' % steer)
			stdscr.addstr(2, 45, '%d ' % pwm1)
			stdscr.addstr(3, 45, '%d ' % pwm2)
			stdscr.addstr(5, 20, "Stop")

		msg = drive_values()
		msg.pwm_drive = pwm1
		msg.pwm_angle = pwm2
		pub.publish(msg)
	curses.endwin()
	rospy.spin()
