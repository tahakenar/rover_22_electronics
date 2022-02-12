#include <Arduino.h>
#include "drive_interface.h"
#include "HardwareSerial.h"
#include "ros.h"
#include "std_msgs/Float64MultiArray.h"

#define ARRAY_LEN 4
#define MSG_LEN ARRAY_LEN*4 + 2

HardwareSerial DriveSerial(PC5, PC4);

std_msgs::Float64MultiArray ros_array;
std_msgs::Float64MultiArray drive_published_feedback;

DriveInterface DriveSystem(ARRAY_LEN, MSG_LEN, 0, 0);

ros::NodeHandle nh;
ros::Publisher driveFeedbackPub("drive_feedback_topic", &drive_published_feedback);

void DriveFeedbackListener(void);

void setup() {
  nh.initNode();
  nh.advertise(driveFeedbackPub);

  DriveSerial.begin(9600);
  ros_array.data=(float *)malloc(sizeof(float)*ARRAY_LEN);  
  ros_array.data_length=ARRAY_LEN;
  
  drive_published_feedback.data=(float *)malloc(sizeof(float)*ARRAY_LEN);  
  drive_published_feedback.data_length=ARRAY_LEN;
  
  ros_array.data[0]=0.55;
  ros_array.data[1]=-0.05;
  ros_array.data[2]=0.37;
  ros_array.data[3]=-0.00;
}

void loop() {
  nh.spinOnce();
  
  
  //Drive SxxxF Sender
  DriveSystem.assignCommandArr(ros_array);
  DriveSystem.multiArrToArr();
  DriveSerial.println(DriveSystem.generateMCUMessage());
  delay(500);
  
  
  //Drive AxxxB Receiver
  DriveFeedbackListener();
  
} 

void DriveFeedbackListener(void){
  char inc_char;
  static String driveFeedbackBuffer;
  static bool receive_flag=false;
  inc_char=DriveSerial.read();
  delay(10);
  if(inc_char=='A'){
    driveFeedbackBuffer="";
    receive_flag=true;
  }
  if(receive_flag && inc_char!='A' && inc_char!='B'){
    driveFeedbackBuffer+=inc_char;
  }
  if(inc_char=='B'){
    DriveSystem.getThrustings(driveFeedbackBuffer);
    DriveSerial.println(driveFeedbackBuffer);
    
    
    for(int i=0;i<ARRAY_LEN;i++){
      drive_published_feedback.data[i]=DriveSystem.returnFeedbackMultiArr().data[i];
    }
    driveFeedbackPub.publish(&drive_published_feedback);
    
    delay(50);

    receive_flag=false;
    driveFeedbackBuffer="";
  }
}

