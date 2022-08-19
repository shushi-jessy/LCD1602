//LCD1602 display programe for Arduino Uno R3
//made by Jessy Polytech Tours France
//29/05/2020
//***********************************************************************************************************************************

#include "Arduino.h"
#include "LCD1602.h"

static int RS,E,RW,D0,D1,D2,D3,D4,D5,D6,D7;
int bitmode;//8-bit=0;4-bit=1;
int FUNCTIONSET,DISPLAYCONTROL,CURSORDISPLAYSHIFT,ENTRYMODE;
//personnally, I don't like to initialize this value as defualt in the very begining. I prefer to set it while initializing the LCD.

//Instruction Code Defination
//Clear Display
    #define LCD_ClearDisplay        0x1    //DB0=1
//Return Home
    #define LCD_ReturnHome          0x2    //DB1=1
//Entry Mode
    #define LCD_EntryMode           0x4    //BD2=1
    #define LCD_cursordirection     0x2    //DB1=I/D
    #define LCD_displayshift        0x1    //DB0=SH
//Display Control
    #define LCD_displayControl      0x8    //DB3=1
    #define LCD_display             0x4    //DB2=D  
    #define LCD_cursor              0x2    //DB1=C 
    #define LCD_blink               0x1    //DB0=B  
//Cursor or Display Shift
    #define LCD_CursorDisplayShift  0x10    //DB4=1
    #define LCD_cursorshift         0x8     //DB3=S/C
    #define LCD_direction           0x4     //DB2=R/L
//Function Set
    #define LCD_FunctionSet         0x20   //DB5=1
    #define LCD_length              0x10   //DB4=DL 
    #define LCD_line                0x8    //DB3=N
    #define LCD_font                0x4    //DB2=F 
//Set CGRAM // this is used to rewrite character by program
    #define LCD_SetCGRAM            0x40   //DB6=1 +Address
//Set DDRAM // this is used to call the address of DDRAM, wich is helpful for changing lines or postions  
    #define LCD_SetDDRAM            0x80   //DB7=1 +Address


void LCD_4bit(int RSpin,int RWpin,int Epin,int D4pin,int D5pin,int D6pin,int D7pin)
{
  //set pins
    RS=RSpin; pinMode(RS,OUTPUT);
    RW=RWpin; pinMode(RW,OUTPUT);
    E=Epin;   pinMode(E,OUTPUT); 
    D4=D4pin; pinMode(D4,OUTPUT); 
    D5=D5pin; pinMode(D5,OUTPUT); 
    D6=D6pin; pinMode(D6,OUTPUT);
    D7=D7pin; pinMode(D7,OUTPUT);

  //initialiation
    bitmode=1;
    //wait for more than 40ms 
    //note that the reason why I don't use delayMicroseconds() function is that high acurrate delay means high CPU load and this is not good for simulating in real time
    //if high accurate isn't needed, I personnally don't sppose to use us delay, it is also way of reducing the wear down of the equipement!
    delay(50);
    //***********HERE IS REALLY IMPORTANT***************
    //because we can only use 4 pins in the same time NOT 8 pins
    //so evertime we have to firstly send MSB then LSB 
    //**************************************************
    //Function set 8-bit and wait for more than 39us
      FUNCTIONSET = LCD_FunctionSet|LCD_length;//it is very interesting to note that we have to send 8-bit mode function set first then send 4-bit ones
          LCDwrite(FUNCTIONSET,0);delay(1);
          /*    D7 D6 D5 D4    
          1st   0  0  1  1     =>0x03
          2nd   don't have to   NONE         
          */
    //Function set 4-bit and wait for more than 39us
      FUNCTIONSET = LCD_FunctionSet|(!LCD_length)|LCD_line;//we have set FUNCTIONSET for initializing it and this will be necessary later if we want to change it 
          LCDwrite(FUNCTIONSET>>4,0);// for 0x20 => 0x02
          LCDwrite(FUNCTIONSET,0);delay(1);
          /*    D7 D6 D5 D4    
          1st   0  0  1  0    =>0x02  so we just have to write LCD_FunctionSet, which is in the MSB of FUNCTIONSET
          2nd   1  0  0  0    =>0x08  so we just have to write LCD_line(N) bit, which is in the LSB of FUNCTIONSET
          */ 
          LCDwrite(LCD_FunctionSet>>4,0);// for 0x20 => 0x02
          LCDwrite(FUNCTIONSET,0);delay(1);//second set  and wait for more than 37us    Since we have initialzied FUNCTIONSET in the first 4-bit function set, we don't have to do it again
    //Display ON/OFF control and wait for more than 39us
      DISPLAYCONTROL=LCD_displayControl|LCD_display|LCD_cursor|LCD_blink;//initialize DISPLAYCONTROL  
          LCDwrite(0x00,0);//MSB of DISPLAYCONTROL is 0000 
          LCDwrite(DISPLAYCONTROL,0);delay(1);
    /*    D7 D6 D5 D4    
    1st   0  0  0  0    =>0x0
    2nd   1  1  0  1    =>0xC
    */
    //Display clear and wait for more than 1.53ms
    LCDwrite(0x00,0);//MSB of command for Display Clear is 0000 
    LCDwrite(LCD_ClearDisplay&(0x0F),0);delay(2);//we just OR it with 0x0F and use its LSB since its MSB are all 0s and we don't mind it;  Display clear and wait for more than 1.53ms
    /*    D7 D6 D5 D4    
    1st   0  0  0  0    =>0x0
    2nd   0  0  0  1    =>0x1
    */
}
void LCD_8bit(int RSpin,int RWpin,int Epin,int D0pin,int D1pin,int D2pin,int D3pin,int D4pin,int D5pin,int D6pin,int D7pin)
{
  //set pins
    RS=RSpin; pinMode(RS,OUTPUT);
    RW=RWpin; pinMode(RW,OUTPUT);
    E=Epin;   pinMode(E,OUTPUT); 
    D0=D0pin; pinMode(D0,OUTPUT); 
    D1=D1pin; pinMode(D1,OUTPUT); 
    D2=D2pin; pinMode(D2,OUTPUT);
    D3=D3pin; pinMode(D3,OUTPUT);
    D4=D4pin; pinMode(D4,OUTPUT); 
    D5=D5pin; pinMode(D5,OUTPUT); 
    D6=D6pin; pinMode(D6,OUTPUT); 
    D7=D7pin; pinMode(D7,OUTPUT);
    
  //initialiation
    bitmode=0;
    // wait for more than 40ms 
      delay(50);
    //Function set and wait for more than 39us
      FUNCTIONSET = LCD_FunctionSet|LCD_length|LCD_line;
          LCDwrite(FUNCTIONSET,0);delay(1);
          LCDwrite(FUNCTIONSET,0);delay(1);//seocnd set
    //Display ON/OFF control and wait for more than 39us 
      DISPLAYCONTROL =LCD_displayControl|LCD_display|LCD_blink;
          LCDwrite(DISPLAYCONTROL,0);delay(1);;  
    //Display clear and wait for more than 1.53ms
      LCDwrite(LCD_ClearDisplay,0);delay(2);;
  }
void Enable(void)//enable pulse control
{
//Enable cycle time>=1200ns; Enable pulse with>=140ns ;Data hold time>=10ns  
digitalWrite(E, HIGH);
delay(1);
digitalWrite(E, LOW);
delay(1);
}
void LCDwrite(int data,int type)
{
  if(type==0)digitalWrite(RS, LOW);//write command  RS=0
  if(type==1)digitalWrite(RS, HIGH);//write data    RS=1
  if(bitmode==0)
    { 
        digitalWrite(D0, data&0x01);
        digitalWrite(D1, data&0x02);
        digitalWrite(D2, data&0x04);
        digitalWrite(D3, data&0x08);
        digitalWrite(D4, data&0x10);
        digitalWrite(D5, data&0x20);
        digitalWrite(D6, data&0x40);
        digitalWrite(D7, data&0x80);
        Enable();
    }
   if(bitmode==1)
   {    
        //don't be confused with the order of the data, first MSB then LSB, if it's inversed, there will be messy code
        digitalWrite(D4, data&0x10);
        digitalWrite(D5, data&0x20);
        digitalWrite(D6, data&0x40);
        digitalWrite(D7, data&0x80);
        Enable();
        digitalWrite(D4, data&0x01);
        digitalWrite(D5, data&0x02);
        digitalWrite(D6, data&0x04);
        digitalWrite(D7, data&0x08);
        Enable();
   }
 }
void LCDprint(char string[])
{
  for(uint8_t i=0;  i< 16 && string[i]!=NULL; i++)//LCD1602 has the code for chars same as ASCII 
  {
    LCDwrite((uint8_t)string[i],1);
  }
}
void LCDdisplayon()
{  
  DISPLAYCONTROL&=0xfB;//this step is used for keep the other bits of DISPLAYCONTROL while reseting the D(display on/off) bit
  DISPLAYCONTROL|=LCD_display;
  LCDwrite(DISPLAYCONTROL,0); 
}
void LCDdisplayoff()
{
  DISPLAYCONTROL&=0xfB;//reset display on/off bit
  DISPLAYCONTROL|=(!LCD_cursor);
  LCDwrite(DISPLAYCONTROL,0);   
}
void LCDcursoron()
{
  DISPLAYCONTROL&=0xfD;//this step is used for keep the other bits of DISPLAYCONTROL while reseting the C(cursor on/off) bit
  DISPLAYCONTROL|=LCD_cursor;
  LCDwrite(DISPLAYCONTROL,0); 
}
void LCDcursoroff()
{
  DISPLAYCONTROL&=0xfD;// reset cursor on/off bit
  DISPLAYCONTROL|=(!LCD_cursor);
  LCDwrite(DISPLAYCONTROL,0); 
}
void LCDblinkon()
{
  DISPLAYCONTROL&=0xfE;////this step is used for keep the other bits of DISPLAYCONTROL while reseting the B(blinking on/off) bit
  DISPLAYCONTROL|=LCD_blink;
  LCDwrite(DISPLAYCONTROL,0); 
}
void LCDblinkoff()
{
  DISPLAYCONTROL&=0xfE;//reset the blinking on/off bit
  DISPLAYCONTROL|=(!LCD_blink);
  LCDwrite(DISPLAYCONTROL,0); 
}
void LCD1line()
{
  FUNCTIONSET&=0xf7;////this step is used for keep the other bits of FUNCTIONSE while reseting the N(2-line/1-line) bit
  FUNCTIONSET|=(!LCD_line);
  LCDwrite(FUNCTIONSET,0); 
}
void LCD2line()
{
  FUNCTIONSET&=0xf7;////treset the 2-line/1-line bit
  FUNCTIONSET|=LCD_line;
  LCDwrite(FUNCTIONSET,0); 
}
void font5_8()
{
  FUNCTIONSET&=0xfB;////this step is used for keep the other bits of FUNCTIONSE while reseting the F(5*11/5*8) bit
  FUNCTIONSET|=(!LCD_font);
  LCDwrite(FUNCTIONSET,0); 
}
void font5_11()
{
  FUNCTIONSET&=0xfB;////this step is used for keep the other bits of FUNCTIONSE while reseting the F(5*11/5*8) bit
  FUNCTIONSET|=LCD_font;
  LCDwrite(FUNCTIONSET,0); 
}
void LCDcursor_shiftright()
{
  CURSORDISPLAYSHIFT&=0xfB;////this step is used for keep the other bits of CURSORDISPLAYSHIFT while reseting the direction of cursor shifting(R/L) bit
  CURSORDISPLAYSHIFT|=LCD_CursorDisplayShift|LCD_cursorshift;//we have to OR CURSORDISPLAYSHIFT with LCD_CursorDisplayShift in the same time inorder to initialize it since we didn't do that before
  LCDwrite(CURSORDISPLAYSHIFT,0); 
 /*for examlpe Xstands for cursur  
 123456X ,after right shifting two times, it becomes X6 
 note that 5 is still there, but if we try to rewrite data this moment then it will be changed
 else, the register will keep the 5
 and the 1234 are STILL IN THE REGISTER TOO! but the cursor is moved to 5 so the display moves to the first postision
 */
}
void LCDcursor_shiftleft()
{
  CURSORDISPLAYSHIFT&=0xfB;////reset the direction of cursor shifting bit
  CURSORDISPLAYSHIFT|=LCD_CursorDisplayShift|(!LCD_cursorshift);//initialize in the same time
  LCDwrite(CURSORDISPLAYSHIFT,0); 
 /*for examlpe Xstands for cursur  
 123456X ,after letf shifting two times, it becomes 1234X6
 note that 5 is still there, but if we try to rewrite data this moment then it will be changed
 else, the register will keep the 5
 */
}
void LCDcursor_directionright()
{
  ENTRYMODE&=0xfB;////this step is used for keep the other bits of CURSORDISPLAYSHIFT while reseting the direction of cursor shifting(R/L) bit
  ENTRYMODE|=LCD_EntryMode|LCD_cursordirection;//we have to OR ENTRYMODE with LCD_EntryMode in the same time inorder to initialize it since we didn't do that before
  LCDwrite(ENTRYMODE,0); 
/*for examlpe 123456 
after using this fuction and writng 789 then it wil become 123456789
*/
}
void LCDcursor_directionleft()
{
  ENTRYMODE&=0xfB;////this step is used for keep the other bits of CURSORDISPLAYSHIFT while reseting the direction of cursor shifting(R/L) bit
  ENTRYMODE|=LCD_EntryMode|(!LCD_cursordirection);//initialize in the same time
  LCDwrite(ENTRYMODE,0); 
/*for examlpe 123456 
after using this fuction and writng 789 then it wil become 123987
*/
}
void LCDReturnHome()
{
  //we use this fonction to drive back the cursor if it's shifted.
  LCDwrite(LCD_ReturnHome,0); 
}
void LCDClear()
{
  LCDwrite(LCD_ClearDisplay,0);
}
void LCDfirstline()
{    
    LCDwrite(LCD_SetDDRAM|(0x00),0);
}
void LCDsecondline()
{
    LCDwrite(LCD_SetDDRAM|(0x40),0); 
}
void LCDpostion(int column, int row)
{
  //For LCD1602 only 16 rows for 1st col and 16 rows for 2nd col is visible   but NOTE that for the register address it is availble for 00H-7FH
  row=(row-1)&0x0F;//statr with 0 and user may think its 1 so we have to be considerable
  if(column==1)LCDwrite(LCD_SetDDRAM|row,0);        //00H-0FH
  if(column==2)LCDwrite(LCD_SetDDRAM|(0x40)|row,0);  //40H-4FH 
}