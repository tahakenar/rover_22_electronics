#include "drive_interface.h"

void DriveInterface::getThrustings(void){
    if(comingString[0]=='A' && comingString[comingString.length()-1]=='B'){
        static int counter=0;
        while(counter<4){
            leftThrusting+=comingString[counter+1];
            counter++;
        }
        counter=0;
        while(counter<4){
            rightThrusting+=comingString[counter+5];
            counter++;
        }
    }
    return ;
}

void DriveInterface::getForwardSteerings(void){
    if(comingString[0]=='A' && comingString[comingString.length()-1]=='B'){
        static int counter=0;
        while(counter<4){
            leftForwardSteering+=comingString[counter+9];
            counter++;
        }
        counter=0;
        while(counter<4){
            rightForwardSteering+=comingString[counter+13];
            counter++;
        }
    }
    return ;
}

void DriveInterface::getBackwardSteerings(void){
    if(comingString[0]=='A' && comingString[comingString.length()-1]=='B'){
        static int counter=0;
        while(counter<4){
            leftBackwardSteering+=comingString[counter+17];
            counter++;
        }
        counter=0;
        while(counter<4){
            rightBackwardSteering+=comingString[counter+21];
            counter++;
        }
    }
    return ;
}

int DriveInterface::getAbs(float x){
    if(x<=0){
        x = -x;
    }
    else if(x>0){
        x = x;
    }
    return x;
}

float DriveInterface::mapData(float x){
    return 255*x;
}

String DriveInterface::getDirection(float x){
    static int direction;
    if(x<=0){
        direction=0;
    }
    else if(x>0){
        direction=1;
    }
    return String(direction);
}

String DriveInterface::generateString(float x){
    String processedString = String(getAbs(mapData(x)));
    while(processed_string.length()<3){
        processed_string = "0" + processed_string;
    }
    return getDirection(x) + processed_string;
}

String DriveInterface::generateMCUMessage(void){
    sentString = 'S';
    for(int i=0;i<ARRAY_LEN<i++){
        sentString+=generateString(commands_to_send[i]);
    }
    sentString+='F';

    return ;
}


