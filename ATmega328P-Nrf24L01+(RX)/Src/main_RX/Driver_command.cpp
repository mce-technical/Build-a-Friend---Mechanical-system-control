#include <Arduino.h>
#include "G:\My Drive\main\Mikayel tasks\Code 2\ATmega328P-Nrf24L01+(RX)\Inc\Driver_command.h"

//int dirr_prev = 0; // direction of right side wheels. if 1 CW else if 0 CCW
//int dirl_prev = 0; // direction of left side wheels. if 1 CW else if 0 CCW

//int speedr_prev = 0; // speed of right side wheels. Range [0, 255]
//int speedl_prev = 0; // speed of left side wheels. Range [0, 255]

void send_to_rdriver(int id, int dirr,int speedr,int dirl,int speedl)
{ 
  //if(dirr!=dirr_prev && speedr!=speedr_prev && dirl!=dirl_prev && speedl!=speedl_prev){
      char buf[10];
      int first_byte = 238 + dirr;
      int check_sum = first_byte ^ speedr;
      sprintf(buf,"%c%c%c",first_byte ,speedr,check_sum);
      Serial.print(buf);
      delay(10);

      char buf2[10];
      first_byte = 237 - dirl;
      check_sum = first_byte ^ speedl;
      sprintf(buf2,"%c%c%c",first_byte ,speedl,check_sum);
      Serial.print(buf2);
      //dirr_prev = dirr;
      //dirl_prev = dirl;
      //speedr_prev = speedr;
      //speedl_prev = speedl;
      delay(10);
//}

}
