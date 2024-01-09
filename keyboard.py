#!/usr/bin/env python

import rospy
from keyboard_teleop.msg import drive_param
import curses

throttle = 0;
steer = 0;

stdscr = curses.initscr()
curses.cbreak()
stdscr.keypad(1)
rospy.init_node('keyboard_talker', anonymous=True)
pub = rospy.Publisher('drive_parameters', drive_param, queue_size=10)

stdscr.refresh()
print "Use arrow keys to move"
key = ''
while key != ord('q'):
	key = stdscr.getch()
	stdscr.refresh()
	if key == curses.KEY_UP and throttle <= 50: 
		throttle = throttle + 0.5
		stdscr.addstr(2, 20, "Forward ")
		stdscr.addstr(2, 30, '%.2f' % throttle)
	elif key == curses.KEY_DOWN and throttle >= -50:
		throttle = throttle - 0.5 
		stdscr.addstr(2, 20, "Reverse ")
		stdscr.addstr(2, 30, '%.2f' % throttle)
	if key == curses.KEY_LEFT and steer >= -50:
		steer = steer - 0.5
		stdscr.addstr(3, 20, "left ")
		stdscr.addstr(3, 30, '%.2f' % steer)
	elif key == curses.KEY_RIGHT and steer <= 50:
		steer = steer + 0.5
		stdscr.addstr(3, 20, "right ")
		stdscr.addstr(3, 30, '%.2f' % steer)
	if key == curses.KEY_DC:
		steer = 0
		throttle = 0
		stdscr.addstr(2, 30, '%.2f' % throttle)
		stdscr.addstr(3, 30, '%.2f' % steer)
		stdscr.addstr(5, 20, "Stop")
	msg = drive_param()
	msg.throttle = throttle
	msg.steer = steer
	pub.publish(msg)
curses.endwin()
