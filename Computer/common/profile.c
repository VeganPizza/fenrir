#include "profile.h"


int		  CON_XAXIS,
		  CON_YAXIS,
		  CON_RAXIS,
          CON_X1AXIS,
          CON_Y1AXIS,
		  CON_TURBO1,
		  CON_TURBO2,
		  CON_ARM_UP,
		  CON_ARM_DOWN,
		  CON_GRIP,
		  CON_FRONT,
		  CON_EXTRA,
		  CON_REAR;

void setProfile(char data){
	switch(data) {
		case 't':
		default:
			CON_XAXIS 		= 0x00;
			CON_YAXIS 		= 0x01;
			CON_RAXIS 		= 0x02;
            CON_X1AXIS      = 0x02;
            CON_Y1AXIS      = 0x03;
			CON_TURBO1 		= 0x05;
			CON_TURBO2 		= 0x07;
			CON_ARM_UP 		= 0x04;
			CON_ARM_DOWN 	= 0x06;
			CON_GRIP 		= 0x01;
			CON_FRONT		= 0x02;
			CON_EXTRA		= 0x08; 
			CON_REAR	    = 0x03;
			break;
	}
}
