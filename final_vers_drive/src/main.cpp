#include <Arduino.h>
#include "drive_interface.h"

#define ARRAY_LEN 4
#define MSG_LEN ARRAY_LEN*4 + 2

std_msgs::Float64MultiArray ros_array;
float printed_array[ARRAY_LEN];

HardwareSerial Serial3(PB11, PB10);

DriveInterface driveSystem(ARRAY_LEN, MSG_LEN);

std_msgs::Float64MultiArray my_marray;
ros::NodeHandle nh;
ros::Publisher encoder_pub("encoder_values",&my_marray);

char str_arr[MSG_LEN+1];
int inc_char;
static String myStr;

void read();

void setup() {
  ros_array.data=(float *)malloc(sizeof(float)*ARRAY_LEN);
  ros_array.data_length=ARRAY_LEN;
  my_marray.data=(float *)malloc(sizeof(float)*ARRAY_LEN);
  my_marray.data_length=ARRAY_LEN;
  for(int i=0;i<ARRAY_LEN;i++){
    ros_array.data[i]=i*-0.1;
  }
  Serial3.begin(9600);
  nh.initNode();
  nh.advertise(encoder_pub);
}

void loop() {
  nh.spinOnce();
  /*
  //SF MESSAGE UART SENDER !!CHECKED!!
  driveSystem.assignCommandArr(ros_array);
  driveSystem.multiArrToArr();
  Serial3.println(driveSystem.generateMCUMessage());
  delay(500);
  */

  //AB ENCODER PUBLISHER
  read();

  


  delay(1);
}


void read(){
    static bool receive_flag=false;
    static int receive_counter=0;
    inc_char=Serial3.read();
    delay(1);
    if(inc_char=='A'){
        myStr="";
        receive_flag=true;
        receive_counter++;
    }
    if(receive_flag && inc_char!='A' && inc_char!= 'B'){
        myStr+=(char)inc_char;
        receive_counter++;
    }
    if(inc_char=='B'){
      driveSystem.getThrustings(myStr);

      for(int i=0;i<ARRAY_LEN;i++){
        my_marray.data[i]=driveSystem.returnFeedbackMultiArray().data[i];
      }
        
      encoder_pub.publish(&my_marray);
      receive_flag=false;
      receive_counter=0;
      myStr="";
    }
}
