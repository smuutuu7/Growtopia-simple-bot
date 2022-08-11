#include <string>


#ifndef STRUCT_H_
#define STRUCT_H_



struct TankPacketStruct {
    uint8_t type = 0;
    uint8_t dropped = 0;
	uint8_t padding1 = 0;
	uint8_t padding2 = 0;
    int netid = 0;
    int snetid = 0;
    int state = 0;
    float padding4 = 0;
    int value = 0;
    float x = 0;
	float y = 0;
    float xspeed = 0;
	float yspeed = 0;
    int padding5 = 0;
    int tilex = 0;
	int tiley = 0;
    uint32_t extDataSize = 0;
};



#endif
