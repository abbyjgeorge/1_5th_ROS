#include <ros.h>
#include <keyboard_teleop/drive_values.h>
#include <PWMServo.h>
ros::NodeHandle nh;

#define STEER_FREQ 200

PWMServo steer(STEER_FREQ);
PWMServo throttle;
PWMServo relay;
PWMServo mux;

int estop_switches[]={0,0,0,0};
int teensy_led = 13;

int relay_open = 0;
int relay_close = 180;
int mux_remote = 0;
int mux_computer = 180;
int i;

//0 = left, 90 = center, 180 = right
//0 = full reverse, 90 = zero, 180 = full forward

int steer_left = 0;
int throttle_reverse_max = 0;

int steer_center = 90;  
int throttle_zero = 90; 

int steer_right = 180;
int throttle_forward_max = 180;

void messageDrive(const keyboard_teleop::drive_values& pwm_value )
{
    if(pwm_value.pwm_drive < throttle_zero)
    {
      throttle.write(throttle_zero);      
    }
    else if(pwm_value.pwm_drive > throttle_forward_max)
    {
      throttle.write(throttle_forward_max);
    }
    else
    {
     throttle.write(pwm_value.pwm_drive);                  
    }

    if(pwm_value.pwm_angle < steer_left)
    {
      steer.write(steer_left);   
    }
    else if(pwm_value.pwm_angle > steer_right)
    {
     steer.write(steer_right);
    }
    else
    {
     steer.write(pwm_value.pwm_angle);                  
    }
}

ros::Subscriber<keyboard_teleop::drive_values> sub_drive("drive_pwm", &messageDrive );

void setup() 
{
  pinMode(teensy_led, OUTPUT);
  Serial1.begin(115200);  //Begin HW serial
  
  throttle.attach(5,1000,3700);
  steer.attach(6);
  
  mux.attach(9,1000,2000);          // attaches mux to pin 9, pwm range: 1000 to 3700 ms
  relay.attach(10,1500,3700);       // attaches relay to pin 10

  analogWriteFrequency(6, 180);
  analogWriteFrequency(5, 90);
  

  throttle.write(throttle_zero);
  steer.write(steer_center);

  nh.initNode();
  nh.subscribe(sub_drive);
}

void loop() 
{
    if(Serial1.available())
  {
    for(i=0;i<4;i++)
    {
      estop_switches[i] = Serial1.read();
    }
    
    if(estop_switches[0] == 1)
    {
      digitalWrite(teensy_led, HIGH);
      relay.write(relay_open);        
    }
    else
    {
      digitalWrite(teensy_led, LOW);
      relay.write(relay_close);
    }

    if(estop_switches[1] == 1)
    {
     mux.write(179);
     digitalWrite(teensy_led, HIGH);
    }
    else
    {
      mux.write(0);
    }
  }
    nh.spinOnce();
}
