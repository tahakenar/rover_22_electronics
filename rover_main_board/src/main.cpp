//#include <Arduino.h>-
//#include <ros.h>
//#include <std_msgs/Float64MultiArray.h>
//#include <std_msgs/String.h>

#include "HardwareSerial.h"

#include "arm_interface.h"

#define TRUE 1
#define FALSE 0

#define ARRAY_LEN 6
#define STR_MSG_LEN (ARRAY_LEN*4 + 2)
#define STR_PIECE_LEN 4
const float mapping_coef = 300;

// ROS Procedure //


ros::NodeHandle nh;
void commandCallback(const std_msgs::Float64MultiArray &command_msg);
void publishEncoderFeedback();
void readDataFromMCU();

std_msgs::Float64MultiArray feedback_pub_arr;

ros::Publisher feedback_pub("arm_feedback_topic",&feedback_pub_arr);
ros::Subscriber<std_msgs::Float64MultiArray> command_sub("arm_command_topic",&commandCallback);

std_msgs::String debug_str;
char debug_ch[50];

ros::Publisher debug_pub("debug_topic",&debug_str);

// Serial Configurations and Variables //

int incoming_byte;
String incoming_str;
char str_arr[STR_MSG_LEN+1];
int receive_cnt_flag = FALSE;
String ongoing_str;
HardwareSerial armSerial(PB11,PB10); // RX, TX
// ----------------------------------- //

ArmInterface armObj(ARRAY_LEN,STR_MSG_LEN,STR_PIECE_LEN,mapping_coef);

void setup() {

    nh.initNode();

    feedback_pub_arr.data = (float*)malloc(sizeof(float)*ARRAY_LEN);
    feedback_pub_arr.data_length = ARRAY_LEN;

    nh.advertise(feedback_pub);
    nh.subscribe(command_sub);

    nh.advertise(debug_pub);

    // Serial inits
    armSerial.begin(57600);

}

void loop() {

    nh.spinOnce();

    //readDataFromMCU();

    delay(1);
}

void commandCallback(const std_msgs::Float64MultiArray &command_msg){

    
    armObj.assignCommandArr(command_msg);
    armObj.multiArrToArr();
    armObj.generateMCUMessage();
    ongoing_str = armObj.returnCommandStr();
    armSerial.println(ongoing_str);

    ongoing_str.toCharArray(debug_ch,50);
    debug_str.data = debug_ch;
    debug_pub.publish(&debug_str);
/*
    board.assignCommandArr(command_msg);
    board.multiArrToArr();
    board.generateStrMsg();
    ongoing_str = board.returnCommandStr();
    armSerial.println(ongoing_str);

    ongoing_str.toCharArray(debug_ch,50);
    //debug_str.data = debug_ch;
    //debug_pub.publish(&debug_str);


    nh.loginfo("New command!");
*/
#ifdef TEST
/*
    ongoing_str[0] = 'A';
    ongoing_str[STR_MSG_LEN-1] = 'B';
    ongoing_str.toCharArray(debug_ch,50);
    debug_str.data = debug_ch;
    debug_pub.publish(&debug_str);

    board.assignStrEncFeedback(ongoing_str);
    board.parseEncoderFeedback();
    board.arrToMultiArr();
    publishEncoderFeedback();
    */
#endif

}


void publishEncoderFeedback(){
    armObj.arrToMultiArr();
    feedback_pub_arr = armObj.returnFeedbackMultiArr();
    feedback_pub.publish(&feedback_pub_arr);
    nh.loginfo("Encoder feedback has published!");
}

void readDataFromMCU(){
     
    if (armSerial.available() > 0){
        incoming_byte = armSerial.read();
        if (incoming_byte == 'A'){
            incoming_str = "";
            incoming_str += (char) incoming_byte;
            receive_cnt_flag = TRUE;
            return;
        }
        else if (receive_cnt_flag = TRUE && incoming_byte != 'B'){
            incoming_str += (char) incoming_byte;
        }
        else if (incoming_byte == 'B'){
            incoming_str += (char) incoming_byte;
            incoming_str.toCharArray(str_arr,STR_MSG_LEN+1);
            nh.loginfo(str_arr);

            armObj.assignStrFeedback(incoming_str);
            armObj.parseEncoderFeedback();
            publishEncoderFeedback();

            incoming_str = "";
            receive_cnt_flag = FALSE;
        }   

    }
}