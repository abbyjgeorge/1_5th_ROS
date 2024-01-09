int eStop = 7;  //toggle button pin for e-stop enable/disable
int manual = 8; //momentary button pin for manual mode using RC transmitter
int computer = 9; // momentary button pin for computer controlled mode

void setup()
{
  //Begin serial monitor port
  Serial.begin(115200);
  //Begin HW serial
  Serial1.begin(115200);

  pinMode(eStop, INPUT);
  pinMode(manual, INPUT);
  pinMode(computer, INPUT);

}

void loop()
{
  bool status_eStop;
  bool status_manual;
  bool status_computer;

  status_eStop = digitalRead(eStop);
  status_manual = digitalRead(manual);
  status_computer = digitalRead(computer);

  //Serial.println(ButtonState, BIN);
  Serial1.write(status_eStop);
  Serial1.write(status_manual);
  Serial1.write(status_computer);
  Serial1.write(1);
  
  //Wait to reduce serial load
  delay(50);
}
