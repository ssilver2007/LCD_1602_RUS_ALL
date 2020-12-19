//Пример для подключения двух дисплеев по шине I2C
//
//Тип подключения дисплея: 1 - по шине I2C, 2 - десятиконтактное. Обязательно указывать ДО подключения библиотеки
//Если этого не сделать, при компиляции возникнет ошибка: "LCD type connect has not been declared"
#define _LCD_TYPE 1
#include <LCD_1602_RUS_ALL.h>

LCD_1602_RUS lcd1(0x3B, 16, 2);
LCD_1602_RUS lcd2(0x3F, 16, 2);

void setup() {
  String str;
  str = "система";

  lcd1.init(); //Инициализация LCD (по умолчанию для ESP8266: 4 - SDA, 5 - SCL)
  //lcd1.init(0, 2); //ESP8266-01 I2C: 0 - SDA, 2 - SCL
  lcd2.init(); // Инициализация LCD №2
  
  // Печать сообщения на LCD №1
  lcd1.backlight();
  lcd1.setCursor(3, 0);
  lcd1.print("Десятичная");
  lcd1.setCursor(4, 1);
  lcd1.print(str);

  // Печать сообщения на LCD №2
  lcd2.backlight();
  lcd2.setCursor(2, 0);
  lcd2.print("Русский язык");
  lcd2.setCursor(5, 1);
  lcd2.print("супер");
}

void loop()
{
}
