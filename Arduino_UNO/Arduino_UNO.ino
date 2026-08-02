#define Echo 3
#define Trig 2

int Time;
int Distance;



void setup()
{
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);

  Serial.begin(9600);
}


int CalculateDistance(short PinEcho , short PinTrig)
{
  digitalWrite(PinTrig, LOW);
  delayMicroseconds(5);

  digitalWrite(PinTrig, HIGH);
  delayMicroseconds(10);

  digitalWrite(PinTrig, LOW);

  Time = pulseIn(PinEcho, HIGH);

  Distance = 0.034 * Time / 2;

  return Distance;
}

byte calculateChecksum(int number)
{
  

  byte CheckSum = 0;

  CheckSum += number / 1000;
  CheckSum += (number / 100) % 10;
  CheckSum += (number / 10) % 10;
  CheckSum += number % 10;

  return CheckSum;
  
}


void loop()
{
  Distance = CalculateDistance(Echo, Trig);
  byte CheckSum = calculateChecksum(Distance);
  
//  Envoie  <distance,checksum>.

 // Distance = 125;
  //byte CheckSum = 9;

  //<125,<126,9>
  
  Serial.print('<');
  Serial.print(Distance);
  Serial.print(',');
  Serial.print(CheckSum);
  Serial.print('>');

  

  delay(100);
}
