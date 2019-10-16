//Пример для подключения дисплея по шине I2C
//
//Тип подключения дисплея: 1 - по шине I2C, 2 - десятиконтактное. Обязательно указывать ДО подключения библиотеки
//Если этого не сделать, при компиляции возникнет ошибка: "LCD type connect has not been declared"
#define _LCD_TYPE 1
#include <LCD_1602_RUS_ALL.h>

LCD_1602_RUS <LiquidCrystal_I2C> lcd(0x27, 16, 2);

void setup()
{
  String str;
  str = "язык";

  lcd.init(); // Инициализация LCD
  // Печать сообщения на LCD
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print("Русский");
  lcd.setCursor(6, 1);
  lcd.print(str);
}

void loop()
{
}
