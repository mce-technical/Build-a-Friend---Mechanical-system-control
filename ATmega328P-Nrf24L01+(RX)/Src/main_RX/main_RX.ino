#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <time.h>
#include <G:\My Drive\main\Mikayel tasks\Code 2\ATmega328P-Nrf24L01+(RX)\Inc\Driver_command.h>


// Nrf24L01+ RF module setup
RF24 radio(10, 9); // CE, CSN
const byte address[6] = "00001";

char inputString[100]; // input buffer for receiving commands
bool stringComplete = false; // flag for command complete. true if received false otherwise

int tick = 0;

// RPM  right wheel 
int rpmcountr;
// RPM left wheel 
int rpmcountl;

int dirr = 0; // direction of right side wheels. if 1 CW else if 0 CCW
int dirl = 0; // direction of left side wheels. if 1 CW else if 0 CCW

int speedr= 0; // speed of right side wheels. Range [255]
int speedl = 0; // speed of left side wheels. Range [255]

int angle = 0;
float f_angle = 0.0;
int sign = 0;
int distance = 0;
float f_distance = 0.0;

int r_read = 0;
int l_read = 0;
int rpml,rpmr;

// Constants 
float N = 6.0; // 6 ticks per revolution  
float R = 0.10795; // radius of wheels of hoverboard [meter]
float L = 0.611 ; // length of hoverboard [meter]
int delta_ticks = 0; // the amount of tick to pass the distance
int max_angular_speed = 50;
int max_forward_speed = 80;

void command()
{
  if(stringComplete)
  {
    
    if(inputString[0] == 'm')
    { 
      sscanf(inputString,"m%d,%d,%d,%d",&dirr,&speedr,&dirl,&speedl);
      char buf[10];
      send_to_rdriver(235,dirr,speedr,dirl,speedl); 
        
    }else if(inputString[0] == 'a'){

     sscanf(inputString,"a%d,s%d,d%d",&angle,&sign,&distance);
     
     f_angle =  float(angle) * (3.141592/180.0); // Degree to radian
     f_distance = float(distance) / 10;
     //Stop reading Interrupt pins.
     // detachInterrupt(digitalPinToInterrupt(2));
     // detachInterrupt(digitalPinToInterrupt(3));
     rpmcountr = 0; // setting 0 the counter for right wheel 
     rpmcountl = 0; // setting 0 the counter for left wheel 
     if(sign == 1){// 1 is positive direction CCW
                    dirr = 0;
                    dirl = 0;
     }else if(sign == 0){// 0 is negative direction CW
                    dirr = 1;
                    dirl = 1;
     }else{
                    dirr = 0;
                    dirl = 0;
     }
     speedr = max_angular_speed;
     speedl = max_angular_speed;
     delta_ticks = int((N * f_angle * L) / (4.0 * 3.141592 * R));
      // Serial.println(( delta_ticks));
      //Start reading Interrupt pins.
      //attachInterrupt(digitalPinToInterrupt(2), rpm_call_r, RISING);
      //attachInterrupt(digitalPinToInterrupt(3), rpm_call_l, RISING);
      //Start moving forward
      send_to_rdriver(235,dirr,speedr,dirl,speedl);
      //Wait until the end 

      do{
          r_read = rpmcountr;
          l_read = rpmcountl;
          delay(20);
          rpmr = 0;
          rpml = 0;
        }
      while(r_read < delta_ticks || l_read < delta_ticks);
      
      // Serial.print(r_read);
      //Serial.print("    ");
      speedr = 9;
      speedl = 9;
      send_to_rdriver(235,1 - dirr,speedr,1 - dirl,speedl);
      //delay(10);
      //detachInterrupt(digitalPinToInterrupt(2));
      //detachInterrupt(digitalPinToInterrupt(3));
      // Forward
      dirr = 1; 
      dirl = 0;
      speedr = max_forward_speed - 5;
      speedl = max_forward_speed;
      rpmcountr = 0; // setting 0 the counter for right wheel 
      rpmcountl = 0; // setting 0 the counter for left wheel 
      r_read = 0;
      l_read = 0;
      delta_ticks = int((N * f_distance)/(2.0 * 3.141592 * R));
      // attachInterrupt(digitalPinToInterrupt(2), rpm_call_r, RISING);
      // attachInterrupt(digitalPinToInterrupt(3), rpm_call_l, RISING);
      send_to_rdriver(235,dirr,speedr,dirl,speedl);
      do{
          r_read = rpmcountr;
          l_read = rpmcountl;
          delay(20);
          rpmr = 0;
          rpml = 0;
        } while(r_read < delta_ticks || l_read < delta_ticks);
        
      speedr = 9;
      speedl = 9;
      send_to_rdriver(235,1 - dirr,speedr,1 - dirl,speedl);
      delay(1000);
      speedr = 1;
      speedl = 1;
      send_to_rdriver(235,1 - dirr,speedr,1 - dirl,speedl);
      

    }
    
    memset(inputString,0,strlen(inputString)); // Clears the command buffer
    stringComplete = false;  // Clears Command received flag
  }
}

void rpm_call_r()
{
  rpmcountr++;
  return 0;
}

void rpm_call_l()
{

  rpmcountl++;
  return 0;
}


void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
  //RPM
  attachInterrupt(digitalPinToInterrupt(2), rpm_call_r, RISING);
  attachInterrupt(digitalPinToInterrupt(3), rpm_call_l, RISING);
  // right wheel 
  rpmcountr = 0;
  // left wheel 
  rpmcountl = 0;
  //send_to_rdriver(235,dirr,30,dirl,30);
}


void loop() {


    command();
    
    // Radio listening with Nrf24L01+
    stringComplete = false;
    if (radio.available()){ 
    while(radio.available()) {
    radio.read(&inputString, sizeof(inputString));
    stringComplete = true;
    }}

}
