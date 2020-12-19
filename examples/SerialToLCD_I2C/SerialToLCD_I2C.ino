//Пример для подключения дисплея по шине I2C
//
//Тип подключения дисплея: 1 - по шине I2C, 2 - десятиконтактное. Обязательно указывать ДО подключения библиотеки
//Если этого не сделать, при компиляции возникнет ошибка: "LCD type connect has not been declared"
#define _LCD_TYPE 1
#include <LCD_1602_RUS_ALL.h>

LCD_1602_RUS lcd(0x27, 16, 2);

void setup() {
  lcd.init(); //Инициализация LCD (по умолчанию для ESP8266: 4 - SDA, 5 - SCL)
  //lcd.init(0, 2); //ESP8266-01 I2C: 0 - SDA, 2 - SCL

  // Печать сообщения на LCD
  lcd.backlight();
  lcd.print("Ввод с монитора");
  lcd.setCursor(0, 1);
  lcd.print("порта");
  //Инициализируем последовательный порт
  Serial.begin(9600);
}

void loop() {
  unsigned char str;
  unsigned char w_str[2];//Для расширенного символа UTF-8 два байта

  // when characters arrive over the serial port...
  if (Serial.available())
  {
    // wait a bit for the entire message to arrive
    delay(100);
    // clear the screen
    lcd.clear();
    lcd.setCursor(0, 0);
    // read all the available characters
    
    /*//Вариант для Win1251
    while (Serial.available() > 0)
    {
      // display each character to the LCD
      str = Serial.read();
      lcd.print(lcd.ascii_win1251(str));//Вывод на экран LCD, в т.ч. по-русски. Перевод из ASCII в Win1251.
    }*/
    
    //Вариант для UTF-8
    while (Serial.available() > 0)
    {
      // display each character to the LCD
      w_str[0] = Serial.read();
      if(w_str[0] > 0x7F)
        w_str[1] = Serial.read();//Если кириллица, то читаем второй байт
      lcd.print(lcd.ascii_utf8(w_str));//Вывод на экран LCD, в т.ч. по-русски. Перевод из ASCII в UTF-8.
    }
    Serial.println();
  }
}
