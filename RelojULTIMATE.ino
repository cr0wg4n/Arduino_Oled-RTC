#include <Wire.h>
#include <toneAC.h>
#include <Adafruit_SSD1306.h>
#define DS3231_I2C_ADDRESS 0x68
#define OLED_ADDR   0x3C
byte tempe=10;
byte timer=0;
Adafruit_SSD1306 display(-1);
byte mat[7][21]={{0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{
                  0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{
                  1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},{
                  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},{
                  1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},{
                  0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{
                  0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
void setup()
{
  Wire.begin();
  // DS3231 seconds, minutes, hours, day, date, month, year
  // setDS3231time(30,42,21,4,26,11,14);
  //setDS3231time(00,7,11,7,7,7,18);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  limp();
}
void loop()
{ Temp();
  displayTime();
  //logo();
  onda();
}

byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}
void Temp(){
timer++;
for(int i=0;i<7;i++){
  for(int j=0;j<21;j++){
    if(mat[i][j]==1){
     dibuP(90+7-i,17+21-j);
    }
  }  
}
  if(timer==25){
    tempe=analogRead(A2);
    tempe=(5.0*tempe*100.0)/1024.0;
    timer=0;
  }
  escrib(100,25,(String)tempe+" C",1);
 
}
void logo(){
for(int i=0;i<128;i++){
int n=random(1,16);
  for(int j=n;j>0;j--){
    dibuP(i,16-j);
  }
}
}
void onda(){
  for(int i=0;i<128;i++){
  int n=random(1,9);
    for(int j=0;j<n;j++){
      dibuP(i,8+j);
      dibuP(i,8-j);   
    }
}

}
void dibuP(int x, int y){
  display.drawPixel(x,y,WHITE);
}
void limp(){
  display.clearDisplay();
 // display.display();
}
void escrib(int x,int y,String l,int t){
   display.setTextSize(t);
   display.setTextColor(WHITE);
   display.setCursor(x,y);
   display.print(l);
}
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}
void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}
void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  //Serial.print(hour, DEC);
  String a="";
  String b="";
  String day;
  if(minute%30==0&&second==0){
  toneAC(1024,10,45,false);
  }
  toneAC();
  if(minute<10){
  b="0";
  }
  if(hour<10){
  a="0" ; 
  }
  switch(dayOfWeek){
    case 1:
    day="Dom";
    break;
    case 2:
    day="Lun";
    break;
    case 3:
    day="Mar";
    break;
    case 4:
    day="Mie";
    break;
    case 5:
    day="Jue";
    break;
    case 6:
    day="Vie";
    break;
    case 7:
    day="Sab";
    break;
  }
  //hora y fecha
  //escrib(12,28,a+(String)hour+":"+b+(String)minute+"  "+(String)dayOfMonth+"/"+(String)month+"/"+(String)year+"  "+day,1);
  escrib(0,45,(String)dayOfMonth+"/"+(String)month+"/"+(String)year+" "+day,2);
  escrib(0,17,a+(String)hour+":"+b+(String)minute,3);
  display.display();
  limp();
}
