#include "arm_interface.h"

void ArmInterface::parseEncoderFeedback()
{
    if (str_feedback[0] == 'A' && str_feedback[_str_msg_length - 1] == 'B')
    {
        float direction;
        float angle;
        String mapped;
        int counter = 0;
        for (int i = 0; i < _array_length * _str_piece_len; i += _str_piece_len)
        {
            mapped = "";
            direction = detectDirection(str_feedback[i + 1]);
            mapped += (char)str_feedback[i + 2];
            mapped += (char)str_feedback[i + 3];
            mapped += (char)str_feedback[i + 4];
            angle = decodeRadians(mapped);
            feedback_to_send[counter] = direction * angle;
            counter++;
        }
    }

    return;
}


float ArmInterface::decodeRadians(String mapped_radian)
{
    float in_radians = mapped_radian.toFloat();

    in_radians = in_radians / _radian_map_coef;

    return in_radians;
}

void ArmInterface::generateMCUMessage()
{
    str_command = "S";

    for (int i = 0; i < _array_length; i++)
    {
        str_command += floatToStr(commands_to_send[i]);
    }

    str_command += "F";
}

String ArmInterface::floatToStr(float radian_data)
{
    String mapped_angle;
    if (radian_data > 0)
    {
        mapped_angle = "1";
    }
    else
    {
        mapped_angle = "0";
    }
    radian_data = abs(radian_data);
    int encoded_radian = radian_data * _radian_map_coef;

    String encoded_str = String(encoded_radian);

    while (encoded_str.length() < 3)
    {
        encoded_str = "0" + encoded_str;
    }

    mapped_angle += encoded_str;

    // S 1942 1471 1270 0000 0270 0471 F //


    return mapped_angle;
}