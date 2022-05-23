#include <TFT_eSPI.h> 
#include "fonts.h"
#include "time.h"
#include "RTClib.h"

RTC_DS3231 rtc;

TFT_eSPI tft = TFT_eSPI(); 
TFT_eSprite img = TFT_eSprite(&tft);

#define color1 TFT_WHITE
#define color2  0x8410
#define color3 0x5ACB
#define color4 0x15B3
#define color5 0x00A3

volatile int counter = 0;
float VALUE;
float lastValue=0;

double rad=0.01745;
float x[360];
float y[360];


float px[360];
float py[360];

float lx[360];
float ly[360];


int r=104;
int sx=120;
int sy=120;

String cc[12]={"45","40","35","30","25","20","15","10","05","0","55","50"};
String days[]={"SUNDAY", "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY"};
int start[12];
int startP[60];

const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 0;


int angle=0;
bool onOff=0;
bool debounce=0;

String h,m,s,d1,d2,m1,m2;

void setup() {



    if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");

  }
    pinMode(2,OUTPUT);
    pinMode(0,INPUT_PULLUP);
    pinMode(35,INPUT_PULLUP);
    pinMode(13,INPUT_PULLUP);

    digitalWrite(2,0);

    ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
    ledcAttachPin(5, pwmLedChannelTFT);
    ledcWrite(pwmLedChannelTFT, 200);

    tft.init();
   
    tft.setSwapBytes(true);
    tft.fillScreen(TFT_BLACK);
 
        
    img.setSwapBytes(true);
    img.createSprite(240, 240);
    img.setTextDatum(4);

int b=0;
int b2=0;


    for(int i=0;i<360;i++)
    {
       x[i]=(r*cos(rad*i))+sx;
       y[i]=(r*sin(rad*i))+sy;
       px[i]=((r-16)*cos(rad*i))+sx;
       py[i]=((r-16)*sin(rad*i))+sy;

       lx[i]=((r-26)*cos(rad*i))+sx;
       ly[i]=((r-26)*sin(rad*i))+sy;
       
       if(i%30==0){
       start[b]=i;
       b++;
       }

       if(i%6==0){
       startP[b2]=i;
       b2++;
       }
      }

}

int lastAngle=0;
float circle=100;
bool dir=0;
int rAngle=359;


void loop() {

 rAngle=rAngle-2;
 DateTime now = rtc.now();

  angle=now.second()*6; 
  
  s=String(now.second());
  m=String(now.minute());
  h=String(now.hour());

  if(m.toInt()<10)
  m="0"+m;

   if(h.toInt()<10)
  h="0"+h;

    if(s.toInt()<10)
  s="0"+s;


  if(now.day()>10)
  {
    d1=now.day()/10;
    d2=now.day()%10;
    }
    else
    {
      d1="0";
      d2=String(now.day());
      }

   if(now.month()>10)
  {
    m1=now.month()/10;
    m2=now.month()%10;
    }
    else
    {
      m1="0";
      m2=String(now.month());
      }
  
  
  if(angle>=360)
  angle=0;

   if(rAngle<=0)
  rAngle=359;



  if(dir==0)
  circle=circle+0.5;
  else
  circle=circle-0.5;
  
  if(circle>140)
  dir=!dir;

  if(circle<100)
  dir=!dir;



  if(angle>-1)
  {
     lastAngle=angle;      

     VALUE=((angle-270)/3.60)*-1;
     if(VALUE<0)
     VALUE=VALUE+100;
 
     
     
 img.fillSprite(TFT_BLACK);
 img.fillCircle(sx,sy,124,color5);
 
 img.setTextColor(TFT_WHITE,color5);
 
img.drawString(days[now.dayOfTheWeek()],circle,120,2);

 
 for(int i=0;i<12;i++)
 if(start[i]+angle<360){
 img.drawString(cc[i],x[start[i]+angle],y[start[i]+angle],2);
 img.drawLine(px[start[i]+angle],py[start[i]+angle],lx[start[i]+angle],ly[start[i]+angle],color1);
 }
 else
 {
 img.drawString(cc[i],x[(start[i]+angle)-360],y[(start[i]+angle)-360],2);
 img.drawLine(px[(start[i]+angle)-360],py[(start[i]+angle)-360],lx[(start[i]+angle)-360],ly[(start[i]+angle)-360],color1);
 }

 

  
 img.setFreeFont(&DSEG7_Modern_Bold_20);
 img.drawString(s,sx,sy-36);
 img.setFreeFont(&DSEG7_Classic_Regular_28);
 img.drawString(h+":"+m,sx,sy+28);
 img.setTextFont(0);

 img.fillRect(70,86,12,20,color3);
 img.fillRect(84,86,12,20,color3);
  img.fillRect(150,86,12,20,color3);
 img.fillRect(164,86,12,20,color3);

 img.setTextColor(0x35D7,TFT_BLACK);
 img.drawString("MONTH",84,78);
 img.drawString("DAY",162,78);
  img.setTextColor(TFT_ORANGE,TFT_BLACK);
  img.drawString("VOLOS PROJECTS",120,174);
  img.drawString("***",120,104);

  img.setTextColor(TFT_WHITE,color3);
  img.drawString(m1,77,96,2);
  img.drawString(m2,91,96,2);

   img.drawString(d1,157,96,2);
  img.drawString(d2,171,96,2);
 
  for(int i=0;i<60;i++)
  if(startP[i]+angle<360)
 img.fillCircle(px[startP[i]+angle],py[startP[i]+angle],1,color1);
 else
 img.fillCircle(px[(startP[i]+angle)-360],py[(startP[i]+angle)-360],1,color1);
 
 img.fillTriangle(sx-1,sy-70,sx-5,sy-56,sx+4,sy-56,TFT_ORANGE);
 img.fillCircle(px[rAngle],py[rAngle],6,TFT_RED);
 img.pushSprite(0, 0);

 
}

}
