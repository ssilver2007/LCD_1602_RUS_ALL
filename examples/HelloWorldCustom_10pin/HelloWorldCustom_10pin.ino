/*Пример для 10-контактного подключения дисплея
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

//Выделим два переопределяемых символа
LCD_1602_RUS lcd(8, 9, 4, 5, 6, 7, 2);//For LCD Keypad Shield
//                                ^^^ количество пользовательских символов
//LCD_1602_RUS lcd(12, 11, 5, 4, 3, 2, 2);
//                                    ^^^ количество пользовательских символов

uint8_t symbol_one[8] = {
  B01110,
  B11111,
  B11111,
  B11111,
  B01111,
  B00111,
  B00011,
  B00001,
};
uint8_t symbol_two[8] = {
  B01110,
  B11111,
  B11111,
  B11111,
  B11110,
  B11100,
  B11000,
  B10000,
};

void setup() {
  //Настраиваем LCD 16 символов в 2 строках
  lcd.begin(16, 2);

  // Print a message to the LCD.
  lcd.setCursor(6, 0);
  lcd.print("Свои");
  lcd.setCursor(2, 1);
  lcd.print("символы:");

  //Переназначаем свои символы - с индексами 6 и 7
  lcd.createChar(6, symbol_one);
  lcd.createChar(7, symbol_two);
  lcd.setCursor(12, 1);
  lcd.write(6);
  lcd.setCursor(13, 1);
  lcd.write(7);
}

void loop() {
}
