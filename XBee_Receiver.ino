#include <PWMServo.h>

PWMServo relay;
PWMServo mux;

int estop_mode[]={0,0,0,0};
int red_led = 13;
int relay_open = 0;
int relay_close = 180;
int mux_computer = 180;
int mux_transmitter = 0;
int i;

void setup()
{
  pinMode(red_led, OUTPUT);
  Serial.begin(115200);   //Begin serial monitor port
  Serial1.begin(115200);  //Begin HW serial
  relay.attach(20);       // attaches relay to pin 20
  mux.attach(9);
}
void loop()
{
  if(Serial1.available())
  {
    Serial.println("Packet:");
    for(i=0;i<4;i++)
    {
      estop_mode[i] = Serial1.read();
      Serial.println(estop_mode[i]);
    }
    
    if(estop_mode[0] == 1)
    {
      digitalWrite(red_led, HIGH);
      relay.write(relay_open);
    }
    else
    {
      digitalWrite(red_led, LOW);
      relay.write(relay_close);
    }

    if(estop_mode[1] == 1)
    {
     mux.write(mux_computer);
    }
    else
    {
      mux.write(mux_transmitter);
    }
  }
}
