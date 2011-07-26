const int LBIT = 1;
const int RBIT = 2;
const int B1BIT = 3;
const int B2BIT = 4;
const int B3BIT = 5;
const int B4BIT = 6;
const int L1BIT = 7;
const int R1BIT = 8;
const int L2BIT = 9;
const int R2BIT = 10;
const int DRIVE = 0;
const int ARM = 1;
const int MOTOR1 = 9;
const int MOTOR2 = 10;
const int VERTACT = 3;
const int HORZACT = 11;
const int GRIPPER = 6;
const int UP = 1;
const int DOWN = 2;
const int OUT = 1;
const int IN = 2;
const int M1 = 0;
const int M2 = 1;
const int BUT1 = 2;
const int BUT2 = 3;
const int BUT3 = 4;
const int BUT4 = 5;
const int L1 = 6;
const int R1 = 7;
const int L2 = 8;
const int R2 = 9;

int inputArray[11];
int ledPin = 13;
int SerialData = 0;
int MotorSpeed1 = 0;
int MotorSpeed2 = 0;
int twoActive = 0;
int fourActive = 0;
int r1Active = 0;
int r2Active = 0;
byte write1 = 190; //instantiated to zero for victor speed controllers
byte write2 = 190; //to be written using analogWrite()
float redFac = .35; //reduce motor outputs
int lBias = 0;
int rBias = 0;
//int c = 3; //This appears to be unsused
int counter = 0;


unsigned long timer=0;
unsigned long timer2=0;

void failsafe();
void run ();

void setup() 
{
    Serial.begin(57600);
    Serial.print("READY TO GO");
    Serial.println(33, BYTE);
}

void loop()
{
    //Serial.println(1, BYTE); //ping controller
    timer2=millis();
    if ((timer2-timer)>500)
    {
        failsafe();
    }

    while ( Serial.available()>0)
    {
        timer=millis();
        counter++;  
        SerialData = Serial.read(); 
        if(SerialData == 10 ) // 10 shows the end of a data frame
        {
            if(counter == 11)// Received data is 10 and counter is 11 so data are valid
            {
                run();        // data is valid => Run command
                counter = 0;  // it is ready for new data frame      
            }
            else // Received data is 10 but counter is not 11 so some data is lost
            {    // reset values
                failsafe();
                counter = 0;
            }
        }
        if(counter == LBIT) // first byte in data frame is Motor1 Speed value
        {
            inputArray[M1] = SerialData;       
        }
        else if(counter == RBIT) // second byte in data frame is Motor2 Speed value
        {
            inputArray[M2] = SerialData;       
        }
        else if(counter == L1BIT)
        {
            if(SerialData)
            {
                 redFac = .5;
            }
            else
            {
                redFac = .35;
            }
        }
        else if(counter == L2BIT)
        {
			if(SerialData)
			{
				redFac = .5;
			}
			else
			{
				redFac = .35;
			}
        }
        else if(counter > 11) // if counter is greater than 11 something is wrong 
        {                   // Reset values
            failsafe();
        }  
    }
}
void run ()
{
    write1 = (byte)(190-((redFac*(inputArray[M1]-190))));
    write2 = (byte)(190+((redFac*(inputArray[M2]-190))));
    int out1 = inputArray[M1];
    int out2 = inputArray[M2];
    Serial.print(out1);
    Serial.print("    ");
    Serial.println(out2);
    analogWrite(MOTOR1,write1);
    analogWrite(MOTOR2,write2);
}

void failsafe(){
    inputArray[M1] = 190;
    inputArray[M2] = 190;
    counter = 0;
    float redFac = .35;
    run();
}
