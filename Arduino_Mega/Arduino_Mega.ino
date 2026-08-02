#include <Arduino.h>

//==========================================================
// Pin definitions
//==========================================================

// Segments
#define SEG_A  2
#define SEG_F  3
#define DIG2   4
#define DIG3   5
#define SEG_B  6
#define SEG_E  7
#define SEG_D  8
#define DP     9
#define SEG_C 10
#define SEG_G 11
#define DIG4  12
#define DIG1  13


//==========================================================
// UART variables
//==========================================================

char distanceBuffer[5];   // 9999
char checksumBuffer[3];   // checksum max 36

byte distanceIndex = 0;
byte checksumIndex = 0;
uint32_t frameStartTime = 0;

int distance = 0;
int receivedChecksum = 0;


enum State
{
  WAIT_START,
  RECEIVE_DISTANCE,
  RECEIVE_CHECKSUM
};


State state = WAIT_START;


//==========================================================
// Segment table Common Anode
//==========================================================

const byte segments[10][7] =
{
  {0,0,0,0,0,0,1}, //0
  {1,0,0,1,1,1,1}, //1
  {0,0,1,0,0,1,0}, //2
  {0,0,0,0,1,1,0}, //3
  {1,0,0,1,1,0,0}, //4
  {0,1,0,0,1,0,0}, //5
  {0,1,0,0,0,0,0}, //6
  {0,0,0,1,1,1,1}, //7
  {0,0,0,0,0,0,0}, //8
  {0,0,0,0,1,0,0}  //9
};


const byte segPins[7] =
{
  SEG_A,
  SEG_B,
  SEG_C,
  SEG_D,
  SEG_E,
  SEG_F,
  SEG_G
};


//==========================================================
// Display
//==========================================================

byte digitValue[4];


void setNumber(uint16_t number)
{
  if(number > 9999)
    number = 9999;

  digitValue[0] = number / 1000;
  digitValue[1] = (number / 100) % 10;
  digitValue[2] = (number / 10) % 10;
  digitValue[3] = number % 10;
}



void refreshDisplay()
{
  static byte currentDigit = 0;


  digitalWrite(DIG1, LOW);
  digitalWrite(DIG2, LOW);
  digitalWrite(DIG3, LOW);
  digitalWrite(DIG4, LOW);



  for(byte i=0; i<7; i++)
  {
    digitalWrite(segPins[i],
                 segments[digitValue[currentDigit]][i]);
  }


  digitalWrite(DP,HIGH);



  switch(currentDigit)
  {
    case 0:
      digitalWrite(DIG1,HIGH);
      break;

    case 1:
      digitalWrite(DIG2,HIGH);
      break;

    case 2:
      digitalWrite(DIG3,HIGH);
      break;

    case 3:
      digitalWrite(DIG4,HIGH);
      break;
  }


  currentDigit++;

  if(currentDigit>=4)
    currentDigit=0;


  delay(2);
}


//==========================================================
// Checksum
//==========================================================

byte calculateChecksum(int number)
{
  byte sum = 0;


  sum += number / 1000;
  sum += (number / 100) % 10;
  sum += (number / 10) % 10;
  sum += number % 10;


  return sum;
}


//==========================================================
// UART reception
//==========================================================

void receiveFrame()
{

  
  while(Serial.available())
  {

    char c = Serial.read();


    switch(state)
    {

      case WAIT_START:

        if(c == '<')
        {
          distanceIndex = 0;
          checksumIndex = 0;

         // frameStartTime = millis();

          state = RECEIVE_DISTANCE;
        }
        
        
        
        break;



      case RECEIVE_DISTANCE:

        
        
        if(c == ',')
        {
          distanceBuffer[distanceIndex]='\0';

          state = RECEIVE_CHECKSUM;
        }


        else
        {
          if(distanceIndex < 4)
          {
            distanceBuffer[distanceIndex]=c;
            distanceIndex++;
          }
        }

        break;



      case RECEIVE_CHECKSUM:

        
        if(c == '>')
        {

          checksumBuffer[checksumIndex]='\0';


          distance = atoi(distanceBuffer);

          receivedChecksum = atoi(checksumBuffer);



          byte calculatedChecksum = calculateChecksum(distance);



          if(calculatedChecksum == receivedChecksum)
          {
            setNumber(distance);
          }



          state = WAIT_START;
        }



        else
        {

          if(checksumIndex < 2)
          {
            checksumBuffer[checksumIndex]=c;
            checksumIndex++;
          }

        }


        break;

    }

  }

}


//==========================================================
// Setup
//==========================================================

void setup()
{

  for(int i=2;i<=13;i++)
  {
    pinMode(i,OUTPUT);
    digitalWrite(i,HIGH);
  }


  digitalWrite(DIG1,LOW);
  digitalWrite(DIG2,LOW);
  digitalWrite(DIG3,LOW);
  digitalWrite(DIG4,LOW);


  Serial.begin(9600);


  setNumber(0);

}



//==========================================================
// Loop
//==========================================================

void loop()
{

  refreshDisplay();


  receiveFrame();
/*
  if(state != WAIT_START)
{
    if(millis() - frameStartTime > 100)
    {
        state = WAIT_START;
        distanceIndex = 0;
        checksumIndex = 0;
    }
}
 */ 

}
