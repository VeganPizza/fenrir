
//    events.h - API description for end user functions
//    Copyright (C) 2011 Illinois Institute of Technology Robotics
//      <robotics@iit.edu>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#ifndef ROBOT_H
#define ROBOT_H


#define BAUD 57600
#define SerComm Serial2    //used to make it easy to change different serial ports such on the mega you can change to Serial1 or Serial2 if you want

#define TIMER_1HZ_      //1 hz and 10 hz timers still run but user functions are not called if not defined
#define TIMER_10HZ_ 
#define TIMER_25HZ_     //uncomment to run code at the specifed hz
//#define TIMER_50HZ_ 
//#define TIMER_100HZ_ 
//#define POWER_LED_        //blinks a led implemented in timer loop and pin specifed below

//#define WATCHDOG_       //To use a Watchdog timer (to use a watchdog timer, optiboot bootloader is needed or uno)
#ifdef WATCHDOG_
#include <avr/wdt.h>      //watchdog library timer loop resets the watch dog
#endif
//Adding quad encoder support
#define QUAD_ENCODER_

#ifdef  QUAD_ENCODER_
#define encoder0PinA  2
#define encoder0PinB  5
#define encoder1PinA  3
#define encoder1PinB  4
double encoder0Pos = 0;
double encoder1Pos = 0;
#endif

//#define PID_
#ifdef PID_
double leftOut;
double rightOut;
double actualL = 0;
double actualR = 0;
PID left_PID(&encoder1Pos,&leftOut,&actualL,1,0,0,REVERSE);
PID right_PID(&encoder0Pos,&rightOut,&actualR,0,0,0,REVERSE);
#endif
#define HOMEBREW_PID
#ifdef HOMEBREW_PID
#define PERROR_L 20
#define DERROR_L 0
#define IERROR_L 100
int last_l = 0;
int last_speed_l;
int perror_l = 0;
int ierror_l = 0;
int derror_l = 0;
int target_l = 0;
int left_out = 0;

#define PERROR_R 20
#define DERROR_R 0
#define IERROR_R 100
int last_r = 0;
int last_speed_r;
int perror_r = 0;
int ierror_r = 0;
int derror_r = 0;
int target_r = 0;
int right_out = 0;
#endif

int failsafeMode = true;
int failsafePermanent = false;  //places the arduino in a "permanent" failsafe mode such that having a network connection will not bring it out unless ROBOT_EVENT_CMD_START is sent
int failcount = 255;

#define QUEUE_SIZE 128    //change if you need a larger queue

enum {
  ROBOT_EVENT_CMD                 = 0x00, // Commands
  ROBOT_EVENT_CMD_NOOP            = 0x01, // No op
  ROBOT_EVENT_CMD_START           = 0x02, // Start
  ROBOT_EVENT_CMD_STOP            = 0x03, // Stop
  ROBOT_EVENT_CMD_REBOOT          = 0x04, // Reboot
  ROBOT_EVENT_CMD_SHUTDOWN        = 0x05, // Shutdown
  ROBOT_EVENT_CMD_FAILSAFE        = 0x06, // Failsafe

  ROBOT_EVENT_NET                 = 0x10, // Remote information
  ROBOT_EVENT_NET_STATUS_OK       = 0x11, // OK
  ROBOT_EVENT_NET_STATUS_ERR      = 0x12, // Error
  ROBOT_EVENT_NET_STATUS_NOTICE   = 0x13, // Notice

  ROBOT_EVENT_JOY_AXIS            = 0x20, // Joystick movements
  ROBOT_EVENT_JOY_BUTTON          = 0x30, // Button presses
  ROBOT_EVENT_JOY_HAT             = 0x31, // Hat movement/D-pad
  ROBOT_EVENT_JOY_STATUS          = 0x32, // Joystick Status

  ROBOT_EVENT_TIMER		    = 0x40, // Timer events
  ROBOT_EVENT_MOTOR	   	    = 0x50, // Motor events
  ROBOT_EVENT_ADC		    = 0x60, // ADC events
  ROBOT_EVENT_SET_VAR		    = 0x70, // Set variable events
  ROBOT_EVENT_READ_VAR	    = 0x80, // Read variable events
  ROBOT_EVENT_VAR                 = 0x90, // Send variable
};

//controller definitions 
int CON_XAXIS,
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

//pin definitions
int PIN_MOTOR1,
PIN_MOTOR2,
PIN_MOTOR3,
PIN_MOTOR4,
POWER_LED_PIN;    //Sets which pin should flash to indicate powered on 

typedef struct {
  unsigned char command; // command 
  unsigned char index; // eg the axis number or button number
  unsigned int value; // eg the axis value or button value
} 
robot_event;

typedef struct {
  robot_event array[QUEUE_SIZE];
  int head_index;
  int tail_index;
  int length;
} 
robot_queue;

void failsafe_mode(robot_queue *q);
#endif // !EVENTS_H


#define MOTOR_RIGHT 11
#define MOTOR_LEFT  9

#define MIN_MOTOR_SPEED 34
#define MAX_MOTOR_SPEED 154

#define MOTOR_RIGHT_PIN_A 3
#define MOTOR_RIGHT_PIN_B 9
#define MOTOR_LEFT_PIN_A 10
#define MOTOR_LEFT_PIN_B 11

#define CELL_1 A0 
#define CELL_2 A1
#define CELL_3 A2
#define CELL_4 A3
#define CELL_5 A4
#define CELL_6 A5
#define CELL_7 A6
#define CELL_8 A7

#define CURRENT A8
#define CURRENT_VOE 524

#define SHUTOFF_A 43
#define SHUTOFF_B 42
#define SHUTOFF_C 41
#define SHUTOFF_D 40

#define ENCOD1_PINA 12
#define ENCOD1_PINB 13
#define ENCOD2_PINA A14
#define ENCOD2_PINB A15

double readVolts(int cell);
double readVolts_norm(int cell);
double readCurrent();
double readPower();

//3v, 4v
int volt_table[8][2]={
  {
    631,837  }
  ,//Cell 1
  {
    316,417  }
  ,//Cell 2
  {
    197,261  }
  ,//Cell 3
  {
    147,195  }
  ,//Cell 4
  {
    634,835  }
  ,//Cell 5
  {
    312,412  }
  ,//Cell 6
  {
    197,261  }
  ,//Cell 7
  {
    146,194  }
};//Cell 8




