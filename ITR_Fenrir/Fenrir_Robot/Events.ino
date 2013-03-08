//    robot_events.c - User implementation of robot events
//    Copyright (C) 2011 Illinois Institute of Technology Robotics
//	  <robotics@iit.edu>
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

//
// This file contains all the user defined code for events triggered by
// a change of state. On the robot side, a change of state occurs
// when a datagram is received that communicates an event on the
// controller's side. Also on timers.
//
// This is where each robots personal code should be placed. 
// Feel free to add more code but try not to remove any 
// Since most of it is used for failafe and other imporant aspects
int l_stick = 127;
int r_stick = 127;
// on_init runs when the program starts up for the first time
void on_init(robot_queue *q) {

}
// on_axis_change is called when a joystick axis changes postion
// axis is the axis number, value is the position of the axis from 0-255
// a value of 127 is center
void on_axis_change(robot_event *ev){

  if(ev->index == 1)
    l_stick = ev->value;
  if(ev->index == 2)
    r_stick = ev->value;
  int tempL = (l_stick)+(r_stick-127);
  int tempR = l_stick-(r_stick-127);
  if(tempL<0)
    tempL = 0;
  if(tempL>255)
    tempL = 255;
  if(tempR<0)
    tempR = 0;
  if(tempR>255)
    tempR = 255;

  analogWrite(MOTOR_LEFT, map(tempL,0,255,MIN_MOTOR_SPEED,MAX_MOTOR_SPEED));
  analogWrite(MOTOR_RIGHT,map(tempR,0,255,MIN_MOTOR_SPEED,MAX_MOTOR_SPEED));
  /*
  Serial.print("$STICK: ");
  Serial.print(tempL);
  Serial.print(" ");
  Serial.print(tempR);
  Serial.println();
  //rMotor.write(map(tempR,0,255,0,179));
  //lMotor.write(map(tempL,0,255,0,179));
  */


}

// on_button_up is called when a joystick button is released
// button is the button number
void on_button_up(robot_event *ev) {
}

// on_button_down is called when a joystick button is pressed
// button is the button number
void on_button_down(robot_event *ev) {	


}

//on_motor is called when a motor speed is updated
//index is motor number value is motor speed, 127 bias
void on_motor(robot_event *ev) {
}

// timer that runs each second
void on_1hz_timer(robot_event *ev){
  /*
  robot_event event;
   event.index = 4;
   event.value=(int)(readVolts(CELL_4)*100);
   event.command=ROBOT_EVENT_VAR;
   send_event(&event);
   */
  Serial.print("$BATT 0 ");
  Serial.print(readVolts(CELL_4));
  Serial.print(" BATT 1 ");
  Serial.print(readVolts(CELL_8));
  Serial.print(" CURRENT ");
  Serial.println(readCurrent());

  /*
  for(int i = 4; i<8;++i){
   Serial.print(" CELL: ");
   Serial.print(i+1);
   Serial.print(" Voltage: ");
   Serial.print(readVolts(54+i));
   }
   Serial.print('/n');
   */
}

// timer that runs each 100 milliseconds
void on_10hz_timer(robot_event *ev){

}

//place code that has to be run every 20hz
void on_25hz_timer(robot_event *ev){

}

//place code that has to be run every 50hz
void on_50hz_timer(robot_event *ev){

}

//place code that has to be run every 100hz
//100hz is the fastest that code should run since 
//most sensors don't update faster than that anyway
void on_100hz_timer(robot_event *ev){

}

// on_command_code is called when the remote computer sends a command datagram
void on_command_code(robot_event *ev) {
  switch(ev->command) {
  case ROBOT_EVENT_CMD_NOOP:
    if(!failsafePermanent){
      failcount = 0;
      failsafeMode = false;
    }
    break;
  case ROBOT_EVENT_CMD_START:
    failsafeMode = false;
    failsafePermanent = false;
    break;
  case ROBOT_EVENT_CMD_STOP:
    break;
    failsafePermanent = true;
    failsafe_mode();
  case ROBOT_EVENT_CMD_FAILSAFE:
    break;
    failsafePermanent = true;
    failsafe_mode();
  case ROBOT_EVENT_CMD_SHUTDOWN:
    //don't know if we need/want it but here it is
    //make sure that the event sent was a shutdown 
    //since if it wasn't this will turn off the arduino and 
    //will require a manual reset
    /*
    if(ev->index == 0 and ev->value == 0){
     failsafe_mode(&qu); //go to failsafe first by placing everything into a safe position
     exit(0);            //exit the program and turn off the arduino
     } 
     */
    break;
  case ROBOT_EVENT_CMD_REBOOT:
#ifdef WATCHDOG_
    delay(500); //lets the watchdog time out and do a reset of the system
#endif
    break;
  default:
    // unknown command code datagram
    break;
  }
}

// on_status_code is called when the remote computer sends a status datagram
void on_status_code(robot_event *ev) {
  switch(ev->command) {
  case ROBOT_EVENT_NET_STATUS_OK:
    break;
  case ROBOT_EVENT_NET_STATUS_ERR:
    break;
  case ROBOT_EVENT_NET_STATUS_NOTICE:
    break;
  default:
    // unknown status datagram
    break;
  }

}

// on_set_variable is called when the computer wants to set a robostix variable
void on_set_variable(robot_event *ev){ 
}

// on_read_variable is called when the computer wants to read a robostix variable
void on_read_variable(robot_event *ev){
}

void on_variable(robot_event *ev){
}

double readVolts(int cell){
  int cell_norm = cell-54;//values of analog start at 54
  int a = volt_table[cell_norm][1];
  int b = volt_table[cell_norm][0];
  double slope = 1/((double)(a-b));
  return slope*(analogRead(cell)-b)+3.0;
}
double readVolts_norm(int cell){
  if(cell==CELL_1||cell==CELL_5)
    return readVolts(cell);
  return readVolts(cell)-readVolts(cell-1);
}
double readCurrent(){
  return ((analogRead(CURRENT)-CURRENT_VOE)/(double)1023)*5*100/.625;
}
double readPower(){
  return (readVolts(CELL_4)+readVolts(CELL_8))/2*readCurrent();
}




