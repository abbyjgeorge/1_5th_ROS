#include <ros.h>
#include <keyboard_teleop/drive_values.h>
#include <PWMServo.h>
ros::NodeHandle nh;

PWMServo relay;
PWMServo mux;

int estop_switches[]={0,0,0,0};
int teensy_led = 13;

int relay_open = 0;
int relay_close = 180;
int mux_remote = 0;
int mux_computer = 180;
int i;

int pwm_mid = 9331; // (16bit)  //  15% duty cycle // 38=0.15*255 (8bit)
int pwm_lo = 7196; //              10% duty cycle // 26=0.01*255
int pwm_hi = 13107; //              20% duty cycle // 51=0.20*255

void messageDrive(const keyboard_teleop::drive_values& pwm_value)
{
    if(pwm_value.pwm_drive < pwm_lo)
    {
      analogWrite(5,pwm_lo);      
    }
    else if(pwm_value.pwm_drive > pwm_hi)
    {
      analogWrite(5,pwm_hi);
    }
    else
    {
      analogWrite(5,pwm_value.pwm_drive);                  
    }

    if(pwm_value.pwm_angle < pwm_lo)
    {
      analogWrite(6,pwm_lo);   
    }
    else if(pwm_value.pwm_angle > pwm_hi)
    {
      analogWrite(6,pwm_hi);
    }
    else
    {
      analogWrite(6,pwm_value.pwm_angle);                  
    }
}

ros::Subscriber<keyboard_teleop::drive_values> sub_drive("drive_pwm", &messageDrive );

void setup() 
{
  pinMode(teensy_led, OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  Serial1.begin(115200);  //Begin HW serial
  
  mux.attach(9,1000,3700);          // attaches mux to pin 9, pwm range: 1000 to 3700 ms
  relay.attach(10,1000,3700);       // attaches relay to pin 10
  
  analogWriteFrequency(5, 180);
  analogWriteFrequency(6, 90);
  analogWriteResolution(16);
  analogWrite(5,pwm_mid);
  analogWrite(6,pwm_mid);

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
     mux.write(mux_computer);
     digitalWrite(teensy_led, HIGH);
    }
    else
    {
      mux.write(mux_remote);
    }
  }
    nh.spinOnce();
}
