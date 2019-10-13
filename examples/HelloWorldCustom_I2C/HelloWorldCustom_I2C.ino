//Пример для подключения дисплея по шине I2C
#include <LCD_1602_RUS_ALL.h>

//Выделим два переопределяемых символа
LCD_1602_RUS_I2C lcd(0x27, 16, 2, 2);
//                               ^^^ количество пользовательских символов

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

void setup()
{
  lcd.init();                      // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(6, 0);
  lcd.print(L"Свои");
  lcd.setCursor(2, 1);
  lcd.print(L"символы:");

  //Переназначаем свои символы - с индексами 6 и 7
  lcd.createChar(6, symbol_one);
  lcd.createChar(7, symbol_two);
  lcd.setCursor(12, 1);
  lcd.write(6);
  lcd.setCursor(13, 1);
  lcd.write(7);
}

void loop()
{
}
