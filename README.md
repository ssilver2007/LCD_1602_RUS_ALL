# LCD_1602_RUS_ALL
Arduino библиотека поддержки кириллицы для дисплеев на базе контроллера HD44780 без встроенной кириллицы. Подключение дисплея как напрямую (10-контактное), так и по интерфейсу I2C.

Максимально возможно отображение 8 уникальных по начертанию русских символа (например Ж, Д, И, Ю и т.п.) Символы, одинаковые по начертанию с английскими (A, B, C, O, P и т.п.) используются из английского набора символов. Дополнительно встроена возможность печати знака градуса Цельсия. Для этого в тексте программы необходимо набрать код UTF-8 (Alt+0176)

При использовании подключения по I2C должна быть установелна базовая библиотека LiquidCrystal_I2C. Например, может быть рекомендована эта: https://github.com/marcoschwartz/LiquidCrystal_I2C


Файлы примеров:

HelloWorld - простой пример вывода кириллических символов на LCD экран.

HelloWorldCustom - пример вывода задаваемых пользователем и кириллических символов на LCD экран.

SerialToLCD - Ввод символов (в т.ч. кириллических) с монитора порта и их вывод на LCD экран.

(c) Сергей Сироткин

По вопросам обращаться: ssilver2007@rambler.ru


Донат:

Ethereum: 0x0A0B8C37146107a36053C3A1433f1C7Fd736eE58

Bitcoin: 1L5DEgxYLu5Yigf1z4Nd9dVCVunH6iXRsi
