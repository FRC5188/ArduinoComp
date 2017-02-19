#include <Wire.h>
#include <Pixy.h>

String piOutput = "wut", liOutput = "wut";
String input = "quoi";
const String PIXY = "pi", LIDAR = "li";
Pixy pixy;

void setup(){
  Serial.begin(9600);
  pinMode(8, INPUT);
  pinMode (13, OUTPUT);
  Wire.begin(4);                // join i2c bus with address #4
  Wire.begin(0);
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent);
  pixy.init();
}

void loop(){
  uint16_t blocks = pixy.getBlocks();
  
  int biggest = -1;
  double area = 0, temp;
  for(int i=0;i<blocks;i++){
    //if(pixy.blocks[i].signature == 3){ 
      temp = pixy.blocks[i].width * pixy.blocks[i].height;
      if(temp > area){
        area = temp;
        biggest = i;
      }
    //}
  }
  if(!blocks){
    piOutput = "none";
  }else{
    piOutput = String(pixy.blocks[biggest].x / 319.0);
    piOutput += "|";
    piOutput += String(pixy.blocks[biggest].y / 199.0);
    piOutput += "|";
    piOutput += String(area / 64000);
    
  }

  receiveEvent(0);
  delay(70);
}

void requestEvent(){
  //Wire.write(output.c_str());
  Wire.write(input == PIXY ? piOutput.c_str() : (input == LIDAR ? liOutput.c_str() : "none"));
}

void receiveEvent(int bytes){


  String data = "";
  static boolean buttonState = LOW;
  
  while ( Wire.available() > 0 ){
    char n = (char)Wire.read();
    if(((int)n)>((int)(' ')))
      data += n;
  }
  input = data == PIXY ? PIXY : LIDAR;
  if (input == PIXY){
    //code to run if input data is from pixy 
    //run vision tracking 
  }
  else if (input == LIDAR){//this is not code from lidar! this is the signal from robrio
    //code to run if input data from lidar 
    //get distance from lidar 
    //send distance to robrio
  }
    //digitalWrite(13, buttonState ? HIGH : LOW);
}

