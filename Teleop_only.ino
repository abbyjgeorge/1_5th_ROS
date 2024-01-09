#include <ros.h>
#include <keyboard_teleop/drive_values.h>
ros::NodeHandle nh;

int pwm_mid = 9331; // (16bit)  //  15% duty cycle // 38=0.15*255 (8bit)
int pwm_lo = 7196; //              10% duty cycle // 26=0.10*255
int pwm_hi = 11466; //              20% duty cycle // 51=0.20*255

void messageDrive(const keyboard_teleop::drive_values& pwm_value )
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

void setup() {
  
  analogWriteFrequency(5, 100);
  analogWriteFrequency(6, 100);
  analogWriteResolution(16);
  analogWrite(5,pwm_mid);
  analogWrite(6,pwm_mid);
  Serial.begin(9600);
  nh.initNode();
  nh.subscribe(sub_drive);
}

void loop() {
    nh.spinOnce();
}
