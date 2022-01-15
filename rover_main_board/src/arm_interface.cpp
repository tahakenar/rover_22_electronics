#include "arm_interface.h"
#include "math_helpers.h"

void ArmInterface::parseEncoderFeedback()
{
    if (str_feedback[0] == 'A' && str_feedback[_str_msg_length - 1] == 'B')
    {
        int16_t axis1 = str_feedback.substring(1, 4).toInt();
        int16_t axis2 = str_feedback.substring(5, 9).toInt();
        int16_t axis3 = str_feedback.substring(9, 13).toInt();
        int16_t axis4 = str_feedback.substring(13, 17).toInt();
        int16_t axis5 = str_feedback.substring(17, 21).toInt();
        int16_t axis6 = str_feedback.substring(21, 26).toInt();

        double axis1_position = rover::map((double)axis1, -999, 999, -135 * DEG_TO_RAD, 135 * DEG_TO_RAD);
        double axis2_position = rover::map((double)axis2, 0, 999, 0, -(85.7 - 24.2) * DEG_TO_RAD);
        double axis3_position = rover::map((double)axis3, 0, 999, 0, -(145.3 - 92.6) * DEG_TO_RAD);
        double axis4_position = rover::map((double)axis4, -999, 999, -90 * DEG_TO_RAD, 90 * DEG_TO_RAD);
        double axis5_position = rover::map((double)axis5, -999, 999, -90 * DEG_TO_RAD, 90 * DEG_TO_RAD);
        double axis6_position = rover::map((double)axis6, -999, 999, -180 * DEG_TO_RAD, 180 * DEG_TO_RAD);

        feedback_to_send[0] = axis1_position;
        feedback_to_send[1] = axis2_position;
        feedback_to_send[2] = axis3_position;
        feedback_to_send[3] = axis4_position;
        feedback_to_send[4] = axis5_position;
        feedback_to_send[5] = axis6_position;
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

    double axis_1_deg = commands_to_send[0] * RAD_TO_DEG;
    double axis_2_deg = 85.7 + (commands_to_send[1] * RAD_TO_DEG);
    double axis_3_deg = 92.6 - (commands_to_send[2] * RAD_TO_DEG);
    double axis_4_deg = commands_to_send[3] * RAD_TO_DEG;
    double axis_5_deg = commands_to_send[4] * RAD_TO_DEG;
    double axis_6_deg = commands_to_send[5] * RAD_TO_DEG;

    axis_1_deg = rover::clamp(axis_1_deg, -135.0, 135.0);
    axis_2_deg = rover::clamp(axis_2_deg, 24.2, 85.7);
    axis_3_deg = rover::clamp(axis_3_deg, 92.6, 145.3);
    axis_4_deg = rover::clamp(axis_4_deg, -90.0, 90.0);
    axis_5_deg = rover::clamp(axis_5_deg, -90.0, 90.0);
    axis_6_deg = rover::clamp(axis_6_deg, -180.0, 180.0);

    str_command += intToStrPiece(rover::map(axis_1_deg, -135, 135, -999, 999));
    str_command += intToStrPiece(rover::map(axis_2_deg, 24.2, 85.7, 999, 0));
    str_command += intToStrPiece(rover::map(axis_3_deg, 92.6, 145.3, 0, 999));
    str_command += intToStrPiece(rover::map(axis_4_deg, -90, 90, -999, 999));
    str_command += intToStrPiece(rover::map(axis_5_deg, -90, 90, -999, 999));
    str_command += intToStrPiece(rover::map(axis_6_deg, -180, 180, -999, 999));

    str_command += "F";
}

String ArmInterface::intToStrPiece(int mapped_data){
    String str_piece;

    if (mapped_data > 0)
        str_piece = "1";
    else
        str_piece = "0";

    String deg_as_str = String(abs(mapped_data));
    while (deg_as_str.length() < 3)
    {
        deg_as_str = "0" + deg_as_str;
    }
    str_piece = str_piece + deg_as_str;
    return str_piece;

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
    //int degrees = radian_data*RAD_TO_DEG;
    int encoded_radian = radian_data * _radian_map_coef;
    //int mapped_degrees = rover::clamp(degrees, _low_deg_arr[motor_index], _upp_deg_arr[motor_index]);

    String encoded_str = String(encoded_radian);

    while (encoded_str.length() < 3)
    {
        encoded_str = "0" + encoded_str;
    }

    mapped_angle += encoded_str;

    // S 1942 1471 1270 0000 0270 0471 F //
    return mapped_angle;
}