#include<LiquidCrystal.h>
#define motors PORTD 
#define n_rot 0x05
#define s_rot 0x0A
#define e_rot 0xA0
#define w_rot 0x50
#define wait 0x00
int e_ldr = A0,w_ldr = A1,n_ldr = A2,s_ldr = A3,d=A4,ldata,dust;
unsigned int east,west,north,south,a[20],i,j;
//RS=13,RW=Gnd,EN=12,D4=11,D5=10,D6=9,D7=8;
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
void setup()
{
  pinMode(0,OUTPUT);
  pinMode(1,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  //--------------------
  lcd.begin(16, 2);
  lcd.setCursor(0,0);  //lcd.setCursor(column,line) starts with 0
  lcd.print("DUAL AXIS SOLAR ");
  lcd.setCursor(0,1);
  lcd.print("TRACKING SYSTEM ");
  delay(1000);
  lcd.setCursor(0,0);  //lcd.setCursor(column,line) starts with 0
  lcd.print("E:       W:     ");
  lcd.setCursor(0,1);
  lcd.print("N:       S:     ");
  delay(200);
}
void loop()
{
    scan();
    compare();
    delay(500);
}
//*********************************************************************************
void scan()
{
    east = analogRead(e_ldr);
    lcd.setCursor(2,0);  //lcd.setCursor(column,line) starts with 0
    lcd.print(east);
	Serial.print("E");
	Serial.print(east);
	Serial.print("#");
    //-----------------
    west = analogRead(w_ldr);
    lcd.setCursor(12,0);  //lcd.setCursor(column,line) starts with 0
    lcd.print(west);
	Serial.print("W");
	Serial.print(west);
	Serial.print("#");
    //-----------------
    north = analogRead(n_ldr);
    lcd.setCursor(2,1);  //lcd.setCursor(column,line) starts with 0
    lcd.print(north);
	Serial.print("N");
	Serial.print(north);
	Serial.print("#");
    //-----------------
    south = analogRead(s_ldr);
    lcd.setCursor(12,1);  //lcd.setCursor(column,line) starts with 0
    lcd.print(south);
	Serial.print("S");
	Serial.print(south);
	Serial.print("#");
    //-----------------
    dust = analogRead(d);
}
//******************************************************
void compare(void)
{
  if(east>west)
  {
    if(east-west>200)
      {
        motors = e_rot;
        delay(50);
        motors = wait;
      }
     else
     {
       motors = wait;
     }
  }
  else if(west>east)
  {
     if(west-east>200)
      {
        motors = w_rot;
        delay(50);
        motors = wait;        
      }
     else
     {
       motors = wait;
     }
  }
 //----------------------
   if(north>south)
  {
     if(north-south>200)
      {
        motors = n_rot;
        delay(50);
        motors = wait;        
      }
     else
     {
       motors = wait;
     }
  }
  else if(south>north)
  {
     if(south-north>200)
      {
        motors = s_rot;
        delay(50);
        motors = wait;        
      }
     else
     {
       motors = wait;
     }
  }
   if(dust<50)
  {
    Serial.print("*1");  
  }
  else
  {
    Serial.print("*0#");  
  }
}
//*****************************
