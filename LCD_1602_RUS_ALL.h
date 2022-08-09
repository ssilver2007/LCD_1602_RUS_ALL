#ifndef _LCD_1602_RUS_ALL_h
#define _LCD_1602_RUS_ALL_h

//#define _LCD_TYPE Тип подключения дисплея: 1 - по шине I2C, 2 - десятиконтактное. Обязательно указывать ДО подключения библиотеки
//Если этого не сделать, при компиляции возникнет ошибка: "LCD type connect has not been declared"
#if (_LCD_TYPE != 1)&&(_LCD_TYPE != 2)
#error "LCD type connect has not been declared"
#endif

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#if defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif
#if (_LCD_TYPE == 1) //Подключение по I2C
#define LCD_LIB LiquidCrystal_I2C
#define LCD_LIB_H <LiquidCrystal_I2C.h>
#elif (_LCD_TYPE == 2) //Подключение 10-контактное
#define LCD_LIB LiquidCrystal
#define LCD_LIB_H <LiquidCrystal.h>
#endif
#include LCD_LIB_H
#include <Print.h>
#include <Wire.h>
#include "font_LCD_1602_RUS.h"

#ifdef ARDUINO_ARCH_AVR
typedef uint32_t _uint_farptr_t;
#else
typedef uint8_t* _uint_farptr_t;
#ifndef memcpy_PF
#define memcpy_PF(dest, src, len) memcpy_P((dest), (src), (len))
#endif
#endif

//Описание класса набора изменяемых символов
class Symbol {
  public:
    Symbol (uint16_t _code)//Конструктор с начальной инициализацией класса по коду символа в unicode
    {
      index = 255;//Начальная инициализация (неинициализированы = 255)
      code = _code;
    }

    _uint_farptr_t getBitmap()//указатель на растровое изображение символа в дальней памяти. Для работы с таким указателем необходимо использовать функцию memcpy_PF(...)
    {
      uint16_t _tmp_unicode;

      for (uint8_t i = 0; i < ((sizeof(symbol_image)) / (sizeof(symbol_image[0]))); i++)
      {
        memcpy_PF(&_tmp_unicode, (_uint_farptr_t)&symbol_image[i]._unicode, sizeof(_tmp_unicode));//Загрузка значения из дальней памяти
        if (_tmp_unicode == code)
          return (_uint_farptr_t)symbol_image[i]._rastr;
      }
      return (_uint_farptr_t)0xFF;//Возвращает 255 при отсутствии совпадения (ошибка)
    }

    uint8_t index;
    uint16_t code;//Код символа в unicode
};

//Описание класса LCD_1602_RUS
//user_custom_symbols - количество символов, доступное пользователю для использования в своих целях (не более 8)
//Всего переопределяемых символов в LCD - 8.
//Символы с индексом от 0 до (7 - user_custom_symbols) используются библиотекой
//Символы с индексом от (8 - user_custom_symbols) до 7 - можно переопределять пользователю
//По умолчанию количество переопределяемых символов равно 0
class LCD_1602_RUS : public LCD_LIB {
  public:
#if (_LCD_TYPE == 1)
    LCD_1602_RUS (uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows, uint8_t user_custom_symbols = 0) : LCD_LIB (lcd_Addr, lcd_cols, lcd_rows) { //Конструктор для подключения I2C
      init_priv(user_custom_symbols);
    }
#elif (_LCD_TYPE == 2)
    LCD_1602_RUS (uint8_t rs,  uint8_t enable,
                  uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t user_custom_symbols = 0) : LCD_LIB (rs, enable, d0, d1, d2, d3) { //Конструктор для подключения 10-pin
      init_priv(user_custom_symbols);
    }
#endif

#ifdef FDB_LIQUID_CRYSTAL_I2C_H
#if defined(ARDUINO_ESP8266_ESP01) //Для ESP8266-01
    void init(uint8_t _sda = SDA, uint8_t _scl = SCL) {
      Wire.pins(_sda, _scl);
      LCD_LIB::begin();
    }
#else
    void init() {
      LCD_LIB::begin();
    }
#endif
#elif defined(ARDUINO_ESP8266_ESP01)  //Для ESP8266-01
    void init(uint8_t _sda = SDA, uint8_t _scl = SCL) {
      Wire.pins(_sda, _scl);
      LCD_LIB::init();
    }
#endif

    void print(const wchar_t* _str) {
      int current_char  = 0;
      int size = 0;

      //Определяем длину строки (количество символов)
      size = wStrlen((uint8_t*)_str);
      /*while (_str[size] != 0)
      {
        size++;
      }*/

      while (current_char < size)
      {
        printwc(_str[current_char]);
        current_char++;
        cursor_col++;
      }
    }
    void print(const char *c_str) {
      wchar_t _str;
      int current_char = 0;
      int size = strlen(c_str);

      while (current_char < size)
      {
        current_char += mbtowc(&_str, (char *)c_str + current_char, 2);
        printwc(_str);
        cursor_col++;
      }
    }
    void print(const String & str) {
      wchar_t _str;
      const char *c_str = str.c_str();
      int current_char = 0;
      int size = str.length();

      while (current_char < size)
      {
        current_char += mbtowc(&_str, (char*)c_str + current_char, 2);
        printwc(_str);
        cursor_col++;
      }
    }

    template <typename T> void print(T val, int base) {
      cursor_col += LCD_LIB::print(val, base);
    }

    void clear() {
      LCD_LIB::clear();
      ResetAllIndex();
    }

    void setCursor(uint8_t col, uint8_t row) {
      cursor_col = col;
      cursor_row = row;
      LCD_LIB::setCursor(cursor_col, cursor_row);
    }

    uint8_t getCursorCol() {
      return cursor_col;
    }

    uint8_t getCursorRow() {
      return cursor_row;
    }

    //Возвращает длину строки для символов, кодируемых двумя и более байтами (unicode, UTF-8)
    int wStrlen(uint8_t *data) {
      int i = 0;
      int count = 0;
      while (data[i]) {
        if ((data[i] & 0xC0) != 0x80)
          count++;
        i++;
      }
      return count;
    }

    //Перевод символа из кодировки ASCII в Win1251 (для печати расширенных русских символов на LCD)
    wchar_t *ascii_win1251(unsigned char ascii) {
      if (ascii == 168) *char_utf8 = 0x401; //код ASCII буквы Ё (0xA8)
      else if (ascii == 184) *char_utf8 = 0x451; //код ASCII буквы ё (0xB8)
      else if (ascii == 165) *char_utf8 = 0x490; //код Ukr. ASCII буквы Г (0xA5)
      else if (ascii == 180) *char_utf8 = 0x491; //код Ukr. ASCII буквы г (0xB4)
      else if (ascii == 170) *char_utf8 = 0x404; //код Ukr. ASCII буквы Е (0xAA)
      else if (ascii == 186) *char_utf8 = 0x454; //код Ukr. ASCII буквы е (0xBA)
      else if (ascii == 175) *char_utf8 = 0x407; //код Ukr. ASCII буквы I (0xAF)
      else if (ascii == 191) *char_utf8 = 0x457; //код Ukr. ASCII буквы i (0xBF)
      else if (ascii >= 192) //остальные буквы русского алфавита
      {
        *char_utf8 = ascii + 848;
      }
      else *char_utf8 = ascii;

      return char_utf8;
    }
    //Перевод символа из кодировки ASCII в UTF-8 (для печати расширенных русских символов на LCD)
    //*ascii - указатель на массив из двух байт
    wchar_t *ascii_utf8(unsigned char *ascii) {
      if (ascii[0] > 0x7F)
      { //Кириллица
        if ((ascii[0] == 0xD0) && (ascii[1] == 0x81)) *char_utf8 = 0x401; //код ASCII буквы Ё (0xD081)
        else if ((ascii[0] == 0xD1) && (ascii[1] == 0x91)) *char_utf8 = 0x451; //код ASCII буквы ё (0xD191)
        else if ((ascii[0] == 0xD2) && (ascii[1] == 0x90)) *char_utf8 = 0x490; //код Ukr. ASCII буквы Г (0xD290)
        else if ((ascii[0] == 0xD2) && (ascii[1] == 0x91)) *char_utf8 = 0x491; //код Ukr. ASCII буквы г (0xD291)
        else if ((ascii[0] == 0xD0) && (ascii[1] == 0x84)) *char_utf8 = 0x404; //код Ukr. ASCII буквы Е (0xD084)
        else if ((ascii[0] == 0xD1) && (ascii[1] == 0x94)) *char_utf8 = 0x454; //код Ukr. ASCII буквы е (0xD194)
        else if ((ascii[0] == 0xD0) && (ascii[1] == 0x87)) *char_utf8 = 0x407; //код Ukr. ASCII буквы I (0xD087)
        else if ((ascii[0] == 0xD1) && (ascii[1] == 0x97)) *char_utf8 = 0x457; //код Ukr. ASCII буквы i (0xD087)
        else if ((ascii[0] == 0xD0) && (ascii[1] >= 0x90) && (ascii[1] <= 0xBF)) //остальные буквы русского алфавита (А...Я а...п)
        {
          *char_utf8 = ascii[1] + 896;
        }
        else if ((ascii[0] == 0xD1) && (ascii[1] >= 0x80) && (ascii[1] <= 0x8F)) //остальные буквы русского алфавита (р...я)
        {
          *char_utf8 = ascii[1] + 960;
        }
      }
      else
      { //Латиница
        *char_utf8 = ascii[0];
      }
      return char_utf8;
    }

    void ResetAllIndex() {
      symbol_index = 0;
      for (uint8_t i = 0; i < ((sizeof(font)) / (sizeof(font[0]))); i++)
        font[i].index = 255;
    }

  private:
    void init_priv(uint8_t _user_custom_symbols) { //Инициализация конструктора
      max_symbol_count = 8 - _user_custom_symbols;
      cursor_col = 0;
      cursor_row = 0;
      ResetAllIndex();//Сброс значений индексов (неинициализированы = 255)
    }

    uint8_t getIndex(uint16_t unicode) { //Функция возвращает индекс символа в массиве font по передаваемому в функцию коду unicode
      //unicode - код символа в unicode
      for (uint8_t i = 0; i < ((sizeof(font)) / (sizeof(font[0]))); i++)
      {
        if (unicode == font[i].code)
          return i;
      }
      return 0xFF;//Возвращает 255 при отсутствии совпадения (ошибка)
    }

    void CharSetToLCD(uint8_t *array, uint8_t *index) { //Загрузка растрового изображения символа в LCD
      uint8_t x, y;

      if (*index == 255) // Если символ еще не создан, то создаем
      {
        x = getCursorCol();
        y = getCursorRow();
        LCD_LIB::createChar(symbol_index, (uint8_t *)array);// Создаем символ на текущем (по очереди) месте в знакогенераторе (от 0 до MAX_SYMBOL_COUNT)
        setCursor(x, y);
        LCD_LIB::write(symbol_index);// Выводим символ на экран
        //Запомианем, что букве соответствует определенный индекс
        *index = symbol_index;
        symbol_index++;
        if (symbol_index >= max_symbol_count)
        {
          ResetAllIndex();
        }
      }
      else   //Иначе печатаем уже существующий
        LCD_LIB::write(*index);
    }

    void printwc(const wchar_t _chr) {
      uint8_t rus_[8];

      if (_chr < 128) //Английский алфавит без изменения
        LCD_LIB::print((char)_chr);
      else
        //Кириллица
        //Единовременно может быть заменено только 8 символов
        switch (_chr)
        {
          //Русский алфавит, использующий одинаковые с английским алфавитом символы
          case 1040: //А
            LCD_LIB::print("A");
            break;
          case 1042: //В
            LCD_LIB::print("B");
            break;
          case 1045: //Е
            LCD_LIB::print("E");
            break;
          case 1025: //Ё
            LCD_LIB::print("E");
            break;
          case 1050: //К
            LCD_LIB::print("K");
            break;
          case 1052: //M
            LCD_LIB::print("M");
            break;
          case 1053: //H
            LCD_LIB::print("H");
            break;
          case 1054: //O
            LCD_LIB::print("O");
            break;
          case 1056: //P
            LCD_LIB::print("P");
            break;
          case 1057: //C
            LCD_LIB::print("C");
            break;
          case 1058: //T
            LCD_LIB::print("T");
            break;
          case 1061: //X
            LCD_LIB::print("X");
            break;
          case 1072: //а
            LCD_LIB::print("a");
            break;
          case 1077: //е
            LCD_LIB::print("e");
            break;
          case 1086: //o
            LCD_LIB::print("o");
            break;
          case 1088: //p
            LCD_LIB::print("p");
            break;
          case 1089: //c
            LCD_LIB::print("c");
            break;
          case 1091: //y
            LCD_LIB::print("y");
            break;
          case 1093: //x
            LCD_LIB::print("x");
            break;
          case 0x00B0: //Знак градуса
            LCD_LIB::write(223);
            break;
          default:
            //Русский алфавит, требующий новых символов
            memcpy_PF(rus_, (_uint_farptr_t)font[getIndex(_chr)].getBitmap(), 8);
            CharSetToLCD((uint8_t *)rus_, &font[getIndex(_chr)].index);
            break;
        }
    }

    uint8_t mbtowc(wchar_t *_chr, char *_str, uint8_t mb_num) {
      if (mb_num != 2) return 0;
      if ((_str[0] & 0xC0) == 0xC0 && (_str[1] & 0x80) == 0x80) {
        *_chr = ((_str[0] & 0x1F) << 6) + (_str[1] & 0x3F);
        return 2;
      }
      else {
        *_chr = _str[0];
        return 1;
      }
    }

    uint8_t max_symbol_count;  //Максимальное количество переназначаемых символов (по умолчанию 8: от 0 до 7)
    int symbol_index;//Индекс символа (от 0 до MAX_SYMBOL_COUNT)
    uint8_t cursor_col;
    uint8_t cursor_row;
    wchar_t char_utf8[2] = {0};
    Symbol font[53] = { //Переменная font - набор изменияемых символов
      1041, //Б
      1043, //Г
      1044, //Д
      1046, //Ж
      1047, //З
      1048, //И

      1049, //Й
      1051, //Л
      1055, //П
      1059, //У
      1060, //Ф

      1062, //Ц
      1063, //Ч
      1064, //Ш
      1065, //Щ

      1066, //Ъ
      1067, //Ы
      1068, //Ь
      1069, //Э
      1070, //Ю
      1071, //Я

      1073, //б
      1074, //в
      1075, //г
      1076, //д
      1105, //ё
      1078, //ж

      1079, //з
      1080, //и
      1081, //й
      1082, //к
      1083, //л

      1084, //м
      1085, //н
      1087, //п
      1090, //т

      1092, //ф
      1094, //ц
      1095, //ч
      1096, //ш
      1097, //щ
      1098, //ъ

      1099, //ы
      1100, //ь
      1101, //э
      1102, //ю
      1103, //я

      0x490, //Укр. Г
      0x491, //укр. г
      0x404, //укр Е
      0x454, //укр е
      0x407, //укр I
      0x457 //укр i
    };
};

#endif
