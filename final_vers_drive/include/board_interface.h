#ifndef BOARD_INTERFACE_H
#define BOARD_INTERFACE_H

#include "Arduino.h"
#include "ros.h"
#include "std_msgs/Float64MultiArray.h"
#include "std_msgs/String.h"
#include "HardwareSerial.h"

class BoardInterface
{
    public:
        int _array_length;
        int _str_msg_length;
        int _str_piece_len;
        float _radian_map_coef;
        float *command_to_send;
        float *feedback_to_send;


        std_msgs::Float64MultiArray feedback_arr;
        std_msgs::Float64MultiArray command_arr;

        String str_command;
        String str_feedback;

    public:
        BoardInterface(int arr_len, int str_msg_len);
        ~BoardInterface();

        void assignFeedbackArr(std_msgs::Float64MultiArray &goal_arr);
        void assignCommandArr(const std_msgs::Float64MultiArray &given_arr);
        std_msgs::Float64MultiArray returnFeedbackMultiArray();
        void multiArrToArr();
        void arrToMultiArr();
        void assignStrFeedback(const String &enc_str);
        String returnCommandString();
};

#endif