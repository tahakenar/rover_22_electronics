#include "board_interface.h"

BoardInterface::BoardInterface(int arr_len, int str_msg_len, int str_piece_len, float mapping_coef)
{
    _array_length = arr_len;
    _str_msg_length = str_msg_len;
    _str_piece_len = str_piece_len;
    _radian_map_coef = mapping_coef;

    commands_to_send = (float *)malloc(sizeof(float) * _array_length);
    feedback_to_send = (float *)malloc(sizeof(float) * _array_length);

    feedback_arr.data = (float *)malloc(sizeof(float) * _array_length);
    feedback_arr.data_length = _array_length;

    command_arr.data = (float *)malloc(sizeof(float) * _array_length);
    command_arr.data_length = _array_length;
}

BoardInterface::~BoardInterface()
{
    free(commands_to_send);
    free(feedback_to_send);
    free(feedback_arr.data);
    free(command_arr.data);
}

void BoardInterface::assignFeedbackArr(std_msgs::Float64MultiArray &goal_arr)
{
    goal_arr = feedback_arr;
}

void BoardInterface::assignCommandArr(const std_msgs::Float64MultiArray &given_arr) 
{
    command_arr = given_arr;
}

std_msgs::Float64MultiArray BoardInterface::returnFeedbackMultiArr() 
{
    return feedback_arr;
}

void BoardInterface::multiArrToArr() 
{
    //TODO: Test this function
    for (int i = 0; i < _array_length; i++){
        commands_to_send[i] = command_arr.data[i];
    }
}

void BoardInterface::arrToMultiArr()
{
    //TODO: Test this function
    for (int i = 0; i < _array_length; i++){
        feedback_arr.data[i] = feedback_to_send[i];
    }
}

void BoardInterface::assignStrFeedback(const String &enc_str) 
{
    str_feedback = enc_str;
}

String BoardInterface::returnCommandStr() 
{
    return str_command;
}

float BoardInterface::detectDirection(char direction_byte)
{
    if (direction_byte == '0')
    {
        return -1.0;
    }
    else
    {
        return 1.0;
    }
}
