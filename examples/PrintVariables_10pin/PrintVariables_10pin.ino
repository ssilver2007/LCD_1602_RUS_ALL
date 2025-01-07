/* Пример для 10-контактного подключения дисплея
  The circuit:
   LCD RS pin to digital pin 12
   LCD Enable pin to digital pin 11
   LCD D4 pin to digital pin 5
   LCD D5 pin to digital pin 4
   LCD D6 pin to digital pin 3
   LCD D7 pin to digital pin 2
   LCD R/W pin to ground
   LCD VSS pin to ground
   LCD VCC pin to 5V
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)
*/
//Тип подключения дисплея: 1 - по шине I2C, 2 - десятиконтактное. Обязательно указывать ДО подключения библиотеки
//Если этого не сделать, при компиляции возникнет ошибка: "LCD type connect has not been declared"
#define _LCD_TYPE 2
#include <LCD_1602_RUS_ALL.h>

//LCD_1602_RUS lcd(12, 11, 5, 4, 3, 2);
LCD_1602_RUS lcd(8, 9, 4, 5, 6, 7 );//For LCD Keypad Shield

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
}

void loop() {
  String str = "Строка";
  double d_var = 8.5;
  int i_var = 10;

  // Print a variables to the LCD.
  lcd.setCursor(0, 0);
  lcd.print("String:");
  lcd.setCursor(8, 0);
  lcd.print(str);
  //
  lcd.setCursor(0, 1);
  lcd.print("double:");
  lcd.setCursor(8, 1);
  lcd.print(d_var, 3);
  //
  delay(2000);
  lcd.clear();
  //
  lcd.setCursor(0, 0);
  lcd.print("int DEC:");
  lcd.setCursor(9, 0);
  lcd.print(i_var, DEC);
  //
  lcd.setCursor(0, 1);
  lcd.print("int HEX:");
  lcd.setCursor(9, 1);
  lcd.print(i_var, HEX);
  //
  delay(2000);
  lcd.clear();
}
