#ifndef ARM_INTERFACE_H
#define ARM_INTERFACE_H

#include "board_interface.h"


class ArmInterface : public BoardInterface
{
    private:
        double _low_deg_arr[6] = {-135.0, 24.2, 92.6, -90.0, -90.0, -180.0};
        double _upp_deg_arr[6] = {135.0, 85.7, 145.3, 90.0, 90.0, 180.0};
    public:

        ArmInterface(int arr_len, int str_msg_len, int str_piece_len, float mapping_coef) : BoardInterface(arr_len,str_msg_len,str_piece_len,mapping_coef){};
        // Constructor. Assings necessary data and allocates memory

        ~ArmInterface(){free(commands_to_send); free(feedback_to_send); free(feedback_arr.data); free(command_arr.data);}
        // Destructor. Frees the memory

        void parseEncoderFeedback();
        // Parses the incoming encoder feedback and assigns necessary data to the relevant arrays

        float decodeRadians(String mapped_radian);
        // Decodes the mapped radian into real value in radians -> Helper method for parseEncoderFeedback

        String intToStrPiece(int mapped_data);

        void generateMCUMessage();
        // Generates a string message to transmit to the lower controller -> In this case Robotic Arm MCU

        String floatToStr(float radian_data);
        // Converts float to String -> A helper function for generateMCUMessage()



};

#endif
