#include <VirtualWire.h>

//This has to match the number of analog pins on the transmitting side.
const int numberOfAnalogPins = 5;
//the next two pins control the H-bridge that drives the motor
const int in1Pin =47;
const int in2Pin =49;
//in my example I only drove one motor, so even thought I recieve 5 analog values I only use 1 valye. 
//Because of how it was wired I used A1 from the transmitting side. That is the second value in my array.
const int dataRead =1;

//this intializes an array that will store the recieved values
int data[numberOfAnalogPins];

//by calcuating the size of the array, the reciever will know how big of a packet it's going to recieve. That's why the number of analog pins has to match.
const int dataBytes = numberOfAnalogPins * sizeof(int);

//this converts the size of the array as an interger to the size of the array as a byte
byte msgLength = dataBytes;

void setup()
{
  Serial.begin(9600);
  Serial.println("Ready");
  //intialize the pins for driving the H-bridge
  pinMode(in1Pin,OUTPUT);
  pinMode(in2Pin,OUTPUT);
  
  //initalize the VirtualWrite to read. With the mega I specify which pin I'm using.
  vw_set_rx_pin(28);
  vw_setup(2000);
  //this begins reciving packets
  vw_rx_start();
}

void loop()
{
  //this line does the magic on the reciving end. If it recieved a message, the data array will now contain everything that was sent.
  if(vw_get_message((byte*)data, &msgLength))
  {
    Serial.println("Got: ");
    if(msgLength == dataBytes)
    {
      //these three conditionals read the second position in the array (A1 from the other board), set the H-bridge, then drive the motor.
      if(data[dataRead]>0)
      {
        analogWrite(2,data[dataRead]);
        digitalWrite(in2Pin,HIGH);
        digitalWrite(in1Pin,LOW);
        Serial.println(data[dataRead]);
      }
      else if(data[dataRead]<0)
      {
        analogWrite(2,(-1*data[dataRead]));
        digitalWrite(in2Pin,LOW);
        digitalWrite(in1Pin,HIGH);
        Serial.println(data[dataRead]);
      }
      //if the stick is in the dead band, this line acts as a motor break, pausing it in place.
      //you can allow the motor to "coast" by setting the pins to LOW instead.
      else
      {
        analogWrite(2,0);
        digitalWrite(in2Pin,HIGH);
        digitalWrite(in1Pin,HIGH);
        Serial.println(data[dataRead]);
      }
/*      for(int i = 0; i < numberOfAnalogPins; i++)
      {
        Serial.print("pin ");
        Serial.print(i);
        Serial.print(" = ");
        Serial.println(data[i]);
      }*/
    }
    else
    {
      Serial.print("unexpected msg lenght of ");
      Serial.println(msgLength);
    }
    Serial.println();
  }
}
