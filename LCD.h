//LCD1602 display programe for Arduino Uno R3
//made by Jessy Polytech Tours France
//29/05/2020
//***********************************************************************************************************************************

#ifndef LCD1062.h
#define LCD1062.h

#include "Arduino.h"

void LCD_4bit(int RSpin,int RWpin,int Epin,int D4pin,int D5pin,int D6pin,int D7pin);
void LCD_8bit(int RSpin,int RWpin,int Epin,int D0pin,int D1pin,int D2pin,int D3pin,int D4pin,int D5pin,int D6pin,int D7pin);
void Enable(void);
void LCDwrite(int data,int type);
void LCDprint(char string[]);
void LCDdisplayon(void);
void LCDdisplayoff(void);
void LCDcursoron(void);
void LCDcursoroff(void);
void LCDblinkon(void);
void LCDblinkoff(void);
void LCD1line(void);
void LCD2line(void);
void font5_8(void);
void font5_11(void);
void LCDcursor_shiftright(void);
void LCDcursor_shiftleft(void);
void LCDcursor_directionright(void);
void LCDcursor_directionleft(void);
void LCDReturnHome(void);
void LCDClear();
void LCDfirstline();
void LCDsecondline();
void LCDpostion(int column, int row);

#endif