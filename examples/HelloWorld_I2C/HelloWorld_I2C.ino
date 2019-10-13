//Пример для подключения дисплея по шине I2C
#include <LCD_1602_RUS_ALL.h>

LCD_1602_RUS_I2C lcd(0x27, 16, 2);

void setup()
{
  String str;
  str = "на русском";

  lcd.init(); // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("Можно печатать");
  lcd.setCursor(3, 1);
  lcd.print(str);

}

void loop()
{
}
