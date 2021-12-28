#include "board_interface.h"

BoardInterface::BoardInterface(int arr_len, int str_msg_len){
    _array_length=arr_len;
    _str_msg_length=str_msg_len;


    command_to_send=(float *)malloc(sizeof(float)*_array_length);
    feedback_to_send=(float *)malloc(sizeof(float)*_array_length);

    feedback_arr.data=(float *)malloc(sizeof(float)*_array_length);
    feedback_arr.data_length=_array_length;

    command_arr.data=(float *)malloc(sizeof(float)*_array_length);
    command_arr.data_length=_array_length;
}

BoardInterface::~BoardInterface(){
    free(command_to_send);
    free(feedback_to_send);
    free(feedback_arr.data);
    free(command_arr.data);
}

void BoardInterface::assignFeedbackArr(std_msgs::Float64MultiArray &goal_arr){
    goal_arr=feedback_arr;
}

void BoardInterface::assignCommandArr(const std_msgs::Float64MultiArray &given_arr){
    command_arr=given_arr;
}

std_msgs::Float64MultiArray BoardInterface::returnFeedbackMultiArray(){
    return feedback_arr;
}

void BoardInterface::multiArrToArr(){
    for (int i = 0; i < _array_length; i++){
        command_to_send[i]=command_arr.data[i];
    }
}

void BoardInterface::arrToMultiArr(){
    for (int i = 0; i < _array_length; i++){
        feedback_arr.data[i] = feedback_to_send[i];
    }
}

void BoardInterface::assignStrFeedback(const String &enc_str){
    str_feedback=enc_str;
}


String BoardInterface::returnCommandString(){
    return str_command;
}





