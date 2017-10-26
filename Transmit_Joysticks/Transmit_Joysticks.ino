#include <VirtualWire.h>

//This sketch was designed to work with two joy sticks (each joy stick has 2 pots) and one standalone pot. That ends up being 5 analog values. 
//2 joysticks, 2 pots each equals 4
const int numberOfJoySticks = 4;
const int numberOfPots = 1;
const int numberOfAnalogPins = numberOfJoySticks + numberOfPots;

//The analog read on the joysticks read from 0 to 1023. These values allow me to convert the analog read values to a range of -255 to 255, where the neutral position eauals 0. 
//The joysticks I used have a bit of slop, so neutral isnt always exactly 511. The dead deadband allows you to allways have it be at 0
const int deadband = 20;
const int center = 511;
const int maXimum = 255;
const int miNimum = -255;
const int ZERO = 0;

//this is an array of ints. This will store all the analog values as they're read
int data[numberOfAnalogPins];

//this calcuates the size of the array in bytes. This is used by the VirtualWire to send the data.
const int dataBytes = numberOfAnalogPins * sizeof(int);

void setup()
{
  //Serial.begin(9600);
  //Serial.println("Ready");
  //initialize the VirtualWire
  vw_setup(2000); 
}

void loop()
{
  int values = 0;

  //this loop will read each of the joystick pots, map the read values to the output values and then store them in the array.
  for(int i=0; i < numberOfJoySticks; i++)
  {
    //I wanted to read 5 analog values, they were wired from A0 to A4. I could just use the iterator to read each value. 
    //If you don't want to wire it that way, you could also store the pin numbers in an array and loop over that array.
    values = analogRead(i);
    if(values >= (center+deadband))
      values = map(values,(center+deadband),1023,ZERO,maXimum);
    else if(values <= (center-deadband))
      values = map(values,ZERO,(center-deadband),miNimum,ZERO);
    else
      values = 0;
    data[i] = values;
  }

  //this loop will only read one value (pin A4). I loop it though incase I wanted to add more pots later, I could by just changing numberOfPots.
  for(int i = numberOfJoySticks; i < numberOfAnalogPins ;i++)
  {
    //this simply reads the analog input and stores it in the 5th position of the array.
    data[i] = analogRead(i);
  } 

  //this lets you see the data being sent before it's sent.
/*  for(int i = 0; i < numberOfAnalogPins; i++)
  {
    Serial.print("pin ");
    Serial.print(i);
    Serial.print(" = ");
    Serial.println(data[i]);   
  }
  Serial.println();*/
  //this funtion sends the data. The (byte*) casts the entire array to bytes, which VirtualWire will transmit.
  send((byte*)data, dataBytes);
  //a short gives it time to send the data before being called again.
  delay(10);
}

void send(byte *data, int nbrOfBytes)
{
  //Virtual wire does the heavy lifitng of transmitting the data.
  vw_send(data, nbrOfBytes);
  vw_wait_tx();
}
