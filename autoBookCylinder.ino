#include <LiquidCrystal.h>                            //16x2 lcd hedder file                       
#define DT  A5                                        //to connect DAT pin of Hx117 module to A5 of Arduino
#define SCK A4                                        //to connect SCk pin of Hx117 module to A4 of Arduino   
#define BUZ  7                                        //to connect the buzzer negetive pin to digitalpin 7 of arduino
float val=0;                                          //creating a float variable
long  cnt=0,smp=0;                                    //creating a long variable
String str="Book a cylinder\nID:12345\nSneha\n2-24\nkukatpally\nhyderabad\n500011";//message to be sent to distributor
LiquidCrystal lcd(9, 8, 13, 12, 11, 10);              //to connect 16x2 display pins to the Arduino pins

void setup()                                          //one time execution code will be placed here                                           
{  
  lcd.begin(16,2);                                    //initializing the display(16 columns,2 rows)
  lcd.print("  LPG AUTOMATIC");                       //prints on LCD
  delay(100);                                         //waits for 100ms
  lcd.setCursor(0,1);
  lcd.print(" BOOKING SYSTEM");                       //setting cursor to next row first colomn
  delay(2000);                                        //waits for 2sec
  Serial.begin(9600);                                 //initializing the serial communication at 9600bps
  pinMode(SCK, OUTPUT);                               //setting the SCK pin as the output
  pinMode(BUZ, OUTPUT);                               //setting the BUZ pin as output
  digitalWrite(BUZ, HIGH);                            //making buz pin high(buzzer off)
  calibrate();                                        //call the calibrate() function
  //gsm_init();                                         //this initialises the GSM module 
  
}


void loop()                                           //which runs forever
{
  cnt= readCount();                                   //calls readCount() function and saved the return value in cnt variable
  int w=(((cnt-smp)/val)-2*((cnt-smp)/val));          //getting weight in grams
  int p=w*(0.05);                                     //getting the percentage and saved in p variable
  if(p<10)SendMessage();                             //if percentage equals 10% then SendMessage() function will be called
  if(p>100)p=100;                                     //limiting the percentage to 100%(2Kg max)
  float q=w/1000.0;                                   //converting grams into Kg
  lcd.print("GAS Lft: WEIGHT:");                      //prints on Display
  lcd.setCursor(0,1);                                 //go to next line in the display
  lcd.print(p);                                       //prints p value
  lcd.print("%  ");                                   //prints % symbol
  lcd.setCursor(9,1);                                 //goto 9th column 2nd row
  lcd.print(q);                                       //prints q value
  lcd.print("Kg       ");                             //prints Kg
 
}

 void SendMessage()                                   //sents a message
{
  lcd.clear();                                        //clears the display
  lcd.print("booking cylinder");                      //prints on the screen
  Serial.println("AT+CMGF=1");                        //command to set GSM module in text mode 
  delay(1000);                                        //wait for a second
  Serial.println("AT+CMGS=\"07675819192\"\r");        //command to send message to that number
  delay(1000);
  Serial.println(str);                                //print the message contained string
  digitalWrite(BUZ, LOW);                             //buzzer active
  delay(100);
  digitalWrite(BUZ, HIGH);                            //buzzer off
  Serial.println((char)26);                           //ending the message(Ctrl+Z in ASCCI)
  digitalWrite(BUZ, LOW);                             //buzzer active
  delay(1000);
  digitalWrite(BUZ, HIGH);                            //buzzer off
  delay(3000);
  lcd.clear();                                        //clears the display
  

}


void gsm_init()                                           //this function calls to initialise the GSM module
{
  lcd.clear();
  lcd.print("GSM initializing...." );
 boolean at_flag=1;                                       //flage for check network
 while(at_flag)
  {
   Serial.println("AT");
   while(Serial.available()>0)
    {
     if(Serial.find("OK"))
     at_flag=0;
    }
    delay(1000);
  }
  boolean echo_flag=1;
  while(echo_flag)
   {
    Serial.println("ATE0");                               //to check network signal
    while(Serial.available()>0) 
     {
      if(Serial.find("OK"))
      echo_flag=0;
     }
     delay(1000);
   }
   boolean net_flag=1;
   while(net_flag)
    {
     Serial.println("AT+CPIN?");                          //confirmation command
     while(Serial.available()>0)
      {
       if(Serial.find("+CPIN: READY"))net_flag=0;
      }
      delay(1000);
    }lcd.clear();
}
unsigned long readCount(void)                             //reads counts from the Hx117 module
{
  unsigned long Cnt;                                      //creating a unsigned long variable
  unsigned char i;                                        //creating a unsigned char variable
  pinMode(DT, OUTPUT);                                    //setting the SCK pin as the output          
  digitalWrite(DT,HIGH);                                  // DT pin set to high
  digitalWrite(SCK,LOW);                                  //SCK pin set to low
  Cnt=0;
  pinMode(DT, INPUT);                                     //setting the SCK pin as the input
  while(digitalRead(DT));                                 //if DT high then Execuits
  for (i=0;i<24;i++)
  {
    digitalWrite(SCK,HIGH);
    Cnt=Cnt<<1;
    digitalWrite(SCK,LOW);
    if(digitalRead(DT)) 
    Cnt++;
  }
  digitalWrite(SCK,HIGH);
  Cnt=Cnt^0x800000;                                      //invert operation
  digitalWrite(SCK,LOW);
  return(Cnt);                                           //this total functon returns the Cnt value
}
void calibrate()                                         // calibrates the weiging sensor
{
  lcd.clear();
  digitalWrite(BUZ, LOW);
  delay(200);
  digitalWrite(BUZ, HIGH);
  lcd.print("Calibrating...");
  lcd.setCursor(0,1);
  lcd.print("Please Wait...");
  for(int i=0;i<100;i++)
  {
    cnt=readCount();
    smp+=cnt;
    
  }
  smp/=100;
 
  lcd.clear();
  lcd.print("Put 240g & wait");
  cnt=0;
  while(cnt<1000)
  {
    cnt=readCount();
    cnt=smp-cnt;
    
  }
  lcd.clear();
  lcd.print("Please Wait....");
  delay(2000);
  for(int i=0;i<100;i++)
  {
    cnt=readCount();
    val+=smp-cnt;
   
  }
  val=val/100.0;
  val=val/240.0;                                       // put here your calibrating weight
  lcd.clear();
  digitalWrite(BUZ, LOW);
  delay(150);
  digitalWrite(BUZ, HIGH);
}

