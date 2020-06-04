#include <TinyGPS++.h>


double LAT,LONG,ALT;
int xsample=0,ysample=0,zsample=0;
int ax,ay,az;
double AX, AY, AZ,roll,pitch,xvalue,yvalue,zvalue;
int j = 0;
int i = 0;
int kk = 0;
TinyGPSPlus gps;
/*SoftwareSerial Serial2 (2,3);
SoftwareSerial Serial1(4, 5);
*/

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial1.begin(9600);
Serial2.begin(9600);

for( kk = 0; kk<10; kk++)
{
  xsample += analogRead(A1);
ysample += analogRead(A2);
zsample += analogRead(A3);
}

xsample/=10;
ysample/=10;
zsample/=10;

xvalue = (((double)(xsample * 5)/1024)-1.65) / 0.33;
yvalue = (((double)(ysample * 5)/1024)-1.65) / 0.33;
zvalue = (((double)(zsample * 5)/1024)-1.65) / 0.33;

Serial.println(xvalue);
Serial.println(yvalue);


}

void loop() {
gg:
while(Serial1.available()>0)
{
  if(gps.encode(Serial1.read()))
{
sendmsg();
  
ax = analogRead(A1);
ay = analogRead(A2);
az = analogRead(A3);

AX = (((double)(ax * 5)/1024)-1.65) / 0.33;
AY = (((double)(ay * 5)/1024)-1.65) / 0.33;
AZ = (((double)(az * 5)/1024)-1.65) / 0.33;

roll = (((atan2(AY,AZ)*180)/3.14)+180) ;
pitch = (((atan2(AZ,AX)*180)/3.14)+180) ;
Serial.println(roll);
Serial.println(pitch);
Serial.println(AX - xvalue);
Serial.println(AY - yvalue);
Serial.println(AZ - zvalue);

if(roll > 220 || roll < 180 || pitch > 270 || pitch <230 || ((AX - xvalue)>5) || ((AX - xvalue)<-5) ||((AY - yvalue)>5) || ((AY - yvalue)<-5) )
{
  if(j==0)
  {
  Serial2.println("AT");
updateSerial();
Serial2.println("AT+CSQ");
updateSerial();
Serial2.println("AT+CMGF=1");
updateSerial();
Serial2.println("AT+CMGS=\"+917899046512\"");
updateSerial();
Serial2.println("Accident has occured at ");
updateSerial();
Serial2.print("LATTITUDE : ");
updateSerial();
Serial2.println(LAT,6);
updateSerial();
Serial2.print("LONGITUDE : ");
updateSerial();
Serial2.println(LONG,6);
updateSerial();
Serial2.print("ALTITUDE : ");
updateSerial();
Serial2.println(ALT,6);
updateSerial();
Serial2.write(26);
  Serial.print("sent");
  j++;
}
}
  }
}
if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    goto gg;
  }
}
  
void sendsms()
{
  Serial2.println("AT+CMGF=1");
  updateSerial();
  Serial2.println("AT+CMGS=\"+918123329629\"");
  updateSerial();
  Serial2.print("Lattitude :");
  Serial2.println(LAT);
  updateSerial();
  Serial2.print("Longitude :");
  Serial2.println(LONG);
  updateSerial();
  Serial2.print("Altitude :");
  Serial2.println(ALT);
  updateSerial();
  Serial2.write(26);
  //Serial.print("yes");
  //updateSerial();
}

void sendmsg()
{
  if (gps.location.isValid())
  {
    LAT = gps.location.lat();
    LONG = gps.location.lng();
    ALT = gps.altitude.meters();
    
  }
}
void updateSerial()
{
 
  delay(500);
  /*while(Serial.available())
  {
    
    Serial2.write(Serial.read());
  }*/
  
  while(Serial2.available())
  {
    Serial.write(Serial2.read());
  }
}

void fulldata()
{
gg :
 
  // This sketch displays information every time a new sentence is correctly encoded.
  while (Serial1.available() > 0)
  {
    Serial.println("jj");
    Serial.println(gps.encode(Serial1.read()));
    if (gps.encode(Serial1.read()))
    {
      Serial.println("gg");
      sendmsg();
// Serial.print("lat read");
    }
  }
  
  // If 5000 milliseconds pass and there are no characters Serial1ing in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    goto gg;
  }
}
