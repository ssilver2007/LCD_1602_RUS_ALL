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

#include <avr/pgmspace.h>
#if (_LCD_TYPE == 1) //Подключение по I2C
#include <LiquidCrystal_I2C.h>
#elif (_LCD_TYPE == 2) //Подключение 10-контактное
#include <LiquidCrystal.h>
#endif
#include <Print.h>
#include "font_LCD_1602_RUS.h"

#define BYTE 0
#ifdef ARDUINO_ARCH_AVR
typedef uint32_t _uint_farptr_t;
#else
typedef uint8_t* _uint_farptr_t;
#ifndef memcpy_PF
#define memcpy_PF(dest, src, len) memcpy((dest), (src), (len))
#endif
#endif

wchar_t char_utf8[] = L" ";

//Шаблон класса LCD_1602_RUS
//user_custom_symbols - количество символов, доступное пользователю для использования в своих целях (не более 8)
//Всего переопределяемых символов в LCD - 8.
//Символы с индексом от 0 до (7 - user_custom_symbols) используются библиотекой
//Символы с индексом от (8 - user_custom_symbols) до 7 - можно переопределять пользователю
//По умолчанию количество переопределяемых символов равно 0
template <class BASE> class LCD_1602_RUS : public BASE {
  public:
    LCD_1602_RUS (uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows, uint8_t user_custom_symbols = 0) : BASE (lcd_Addr, lcd_cols, lcd_rows)//Конструктор для подключения I2C
    {
      max_symbol_count = 8 - user_custom_symbols;
      symbol_index = 0;
      cursor_col = 0;
      cursor_row = 0;
      ResetAllIndex();//Сброс значений индексов (неинициализированы = 255)
    }
    LCD_1602_RUS (uint8_t rs,  uint8_t enable,
                  uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t user_custom_symbols = 0) : BASE (rs, enable, d0, d1, d2, d3)//Конструктор для подключения 10-pin
    {
      max_symbol_count = 8 - user_custom_symbols;
      symbol_index = 0;
      cursor_col = 0;
      cursor_row = 0;
      ResetAllIndex();//Сброс значений индексов (неинициализированы = 255)
    }

    void print(const wchar_t* _str)
    {
      int current_char  = 0;
      int size = 0;

      //Определяем длину строки (количество символов)
      while (_str[size] != 0)
      {
        size++;
      }

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
    void print(const String &str) {
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
    
    template <typename T> void print(T val, int base)
    {
      cursor_col += BASE::print(val, base);
    }

    void clear()
    {
      BASE::clear();
      ResetAllIndex();
    }

    void setCursor(uint8_t col, uint8_t row)
    {
      cursor_col = col;
      cursor_row = row;
      BASE::setCursor(cursor_col, cursor_row);
    }
    uint8_t getCursorCol()
    {
      return cursor_col;
    }
    uint8_t getCursorRow()
    {
      return cursor_row;
    }

    //Перевод символа из кодировки ASCII в Win1251 (для печати расширенных русских символов на LCD)
    wchar_t *ascii_win1251(unsigned char ascii)
    {
      if (ascii == 168) *char_utf8 = 0x401; //код ASCII буквы Ё (0xA8)
      else if (ascii == 184) *char_utf8 = 0x451; //код ASCII буквы ё (0xB8)
      else if (ascii >= 192) //остальные буквы русского алфавита
      {
        *char_utf8 = ascii + 848;
      }
      else *char_utf8 = ascii;

      return char_utf8;
    }
    //Перевод символа из кодировки ASCII в UTF-8 (для печати расширенных русских символов на LCD)
    //*ascii - указатель на массив из двух байт
    wchar_t *ascii_utf8(unsigned char *ascii)
    {
      if ((ascii[0] == 0xD0) || (ascii[0] == 0xD1))
      {//Кириллица
        if ((ascii[0] == 0xD0)&&(ascii[1] == 0x81)) *char_utf8 = 0x401; //код ASCII буквы Ё (0xD081)
        else if ((ascii[0] == 0xD1)&&(ascii[1] == 0x91)) *char_utf8 = 0x451; //код ASCII буквы ё (0xD191)
        else if ((ascii[0] == 0xD0)&&(ascii[1] >= 0x90)&&(ascii[1] <= 0xBF)) //остальные буквы русского алфавита (А...Я а...п)
        {
          *char_utf8 = ascii[1] + 896;
        }
        else if ((ascii[0] == 0xD1)&&(ascii[1] >= 0x80)&&(ascii[1] <= 0x8F)) //остальные буквы русского алфавита (р...я)
        {
          *char_utf8 = ascii[1] + 960;
        }
      }
      else
      {//Латиница
        *char_utf8 = ascii[0];
      }
      return char_utf8;
    }


  private:
    void CharSetToLCD(uint8_t *array, uint8_t *index)
    {
      uint8_t x, y;

      if (*index == 255) // Если символ еще не создан, то создаем
      {
        x = getCursorCol();
        y = getCursorRow();
        BASE::createChar(symbol_index, (uint8_t *)array);// Создаем символ на текущем (по очереди) месте в знакогенераторе (от 0 до MAX_SYMBOL_COUNT)
        setCursor(x, y);
        BASE::write(symbol_index);// Выводим символ на экран
        //Запомианем, что букве соответствует определенный индекс
        *index = symbol_index;
        symbol_index++;
        if (symbol_index >= max_symbol_count)
        {
          symbol_index = 0;
          ResetAllIndex();
        }
      }
      else   //Иначе печатаем уже существующий
        BASE::write(*index);
    }

    void ResetAllIndex()
    {
      symbol_index = 0;
      index_rus_B = 255;
      index_rus_G = 255;
      index_rus_D = 255;
      index_rus_ZH = 255;
      index_rus_Z = 255;
      index_rus_I = 255;
      index_rus_II = 255;
      index_rus_L = 255;
      index_rus_P = 255;
      index_rus_U = 255;
      index_rus_F = 255;
      index_rus_TS = 255;
      index_rus_CH = 255;
      index_rus_SH = 255;
      index_rus_SCH = 255;
      index_rus_tverd = 255;
      index_rus_Y = 255;
      index_rus_myagk = 255;
      index_rus_EE = 255;
      index_rus_YU = 255;
      index_rus_YA = 255;
      index_rus_b = 255;
      index_rus_v = 255;
      index_rus_g = 255;
      index_rus_d = 255;
      index_rus_yo = 255;
      index_rus_zh = 255;
      index_rus_z = 255;
      index_rus_i = 255;
      index_rus_ii = 255;
      index_rus_k = 255;
      index_rus_l = 255;
      index_rus_m = 255;
      index_rus_n = 255;
      index_rus_p = 255;
      index_rus_t = 255;
      index_rus_u = 255;
      index_rus_f = 255;
      index_rus_ts = 255;
      index_rus_ch = 255;
      index_rus_sh = 255;
      index_rus_sch = 255;
      index_rus_tverd_mal = 255;
      index_rus_y = 255;
      index_rus_myagk_mal = 255;
      index_rus_ee = 255;
      index_rus_yu = 255;
      index_rus_ya = 255;
    }

    void printwc(const wchar_t _chr) {
      uint8_t rus_[8];
      switch (_chr)
      {
        //Русский алфавит, требующий новых символов
        //Единовременно может быть заменено только 8 символов
        case 1041: //Б
          memcpy_PF(rus_, (_uint_farptr_t)rus_B, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_B);
          break;
        case 1043: //Г
          memcpy_PF(rus_, (_uint_farptr_t)rus_G, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_G);
          break;
        case 1044: //Д
          memcpy_PF(rus_, (_uint_farptr_t)rus_D, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_D);
          break;
        case 1046: //Ж
          memcpy_PF(rus_, (_uint_farptr_t)rus_ZH, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_ZH);
          break;
        case 1047: //З
          memcpy_PF(rus_, (_uint_farptr_t)rus_Z, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_Z);
          break;
        case 1048: //И
          memcpy_PF(rus_, (_uint_farptr_t)rus_I, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_I);
          break;
        case 1049: //Й
          memcpy_PF(rus_, (_uint_farptr_t)rus_II, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_II);
          break;
        case 1051: //Л
          memcpy_PF(rus_, (_uint_farptr_t)rus_L, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_L);
          break;
        case 1055: //П
          memcpy_PF(rus_, (_uint_farptr_t)rus_P, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_P);
          break;
        case 1059: //У
          memcpy_PF(rus_, (_uint_farptr_t)rus_U, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_U);
          break;
        case 1060: //Ф
          memcpy_PF(rus_, (_uint_farptr_t)rus_F, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_F);
          break;
        case 1062: //Ц
          memcpy_PF(rus_, (_uint_farptr_t)rus_TS, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_TS);
          break;
        case 1063: //Ч
          memcpy_PF(rus_, (_uint_farptr_t)rus_CH, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_CH);
          break;
        case 1064: //Ш
          memcpy_PF(rus_, (_uint_farptr_t)rus_SH, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_SH);
          break;
        case 1065: //Щ
          memcpy_PF(rus_, (_uint_farptr_t)rus_SCH, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_SCH);
          break;
        case 1066: //Ъ
          memcpy_PF(rus_, (_uint_farptr_t)rus_tverd, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_tverd);
          break;
        case 1067: //Ы
          memcpy_PF(rus_, (_uint_farptr_t)rus_Y, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_Y);
          break;
        case 1068: //Ь
          memcpy_PF(rus_, (_uint_farptr_t)rus_myagk, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_myagk);
          break;
        case 1069: //Э
          memcpy_PF(rus_, (_uint_farptr_t)rus_EE, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_EE);
          break;
        case 1070: //Ю
          memcpy_PF(rus_, (_uint_farptr_t)rus_YU, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_YU);
          break;
        case 1071: //Я
          memcpy_PF(rus_, (_uint_farptr_t)rus_YA, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_YA);
          break;
        case 1073: //б
          memcpy_PF(rus_, (_uint_farptr_t)rus_b, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_b);
          break;
        case 1074: //в
          memcpy_PF(rus_, (_uint_farptr_t)rus_v, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_v);
          break;
        case 1075: //г
          memcpy_PF(rus_, (_uint_farptr_t)rus_g, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_g);
          break;
        case 1076: //д
          memcpy_PF(rus_, (_uint_farptr_t)rus_d, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_d);
          break;
        case 1105: //ё
          memcpy_PF(rus_, (_uint_farptr_t)rus_yo, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_yo);
          break;
        case 1078: //ж
          memcpy_PF(rus_, (_uint_farptr_t)rus_zh, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_zh);
          break;
        case 1079: //з
          memcpy_PF(rus_, (_uint_farptr_t)rus_z, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_z);
          break;
        case 1080: //и
          memcpy_PF(rus_, (_uint_farptr_t)rus_i, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_i);
          break;
        case 1081: //й
          memcpy_PF(rus_, (_uint_farptr_t)rus_ii, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_ii);
          break;
        case 1082: //к
          memcpy_PF(rus_, (_uint_farptr_t)rus_k, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_k);
          break;
        case 1083: //л
          memcpy_PF(rus_, (_uint_farptr_t)rus_l, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_l);
          break;
        case 1084: //м
          memcpy_PF(rus_, (_uint_farptr_t)rus_m, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_m);
          break;
        case 1085: //н
          memcpy_PF(rus_, (_uint_farptr_t)rus_n, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_n);
          break;
        case 1087: //п
          memcpy_PF(rus_, (_uint_farptr_t)rus_p, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_p);
          break;
        case 1090: //т
          memcpy_PF(rus_, (_uint_farptr_t)rus_t, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_t);
          break;
        case 1092: //ф
          memcpy_PF(rus_, (_uint_farptr_t)rus_f, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_f);
          break;
        case 1094: //ц
          memcpy_PF(rus_, (_uint_farptr_t)rus_ts, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_ts);
          break;
        case 1095: //ч
          memcpy_PF(rus_, (_uint_farptr_t)rus_ch, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_ch);
          break;
        case 1096: //ш
          memcpy_PF(rus_, (_uint_farptr_t)rus_sh, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_sh);
          break;
        case 1097: //щ
          memcpy_PF(rus_, (_uint_farptr_t)rus_sch, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_sch);
          break;
        case 1098: //ъ
          memcpy_PF(rus_, (_uint_farptr_t)rus_tverd_mal, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_tverd_mal);
          break;
        case 1099: //ы
          memcpy_PF(rus_, (_uint_farptr_t)rus_y, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_y);
          break;
        case 1100: //ь
          memcpy_PF(rus_, (_uint_farptr_t)rus_myagk_mal, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_myagk_mal);
          break;
        case 1101: //э
          memcpy_PF(rus_, (_uint_farptr_t)rus_ee, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_ee);
          break;
        case 1102: //ю
          memcpy_PF(rus_, (_uint_farptr_t)rus_yu, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_yu);
          break;
        case 1103: //я
          memcpy_PF(rus_, (_uint_farptr_t)rus_ya, 8);
          CharSetToLCD((uint8_t *)rus_, &index_rus_ya);
          break;
        //Русский алфавит, использующий одинаковые с английским алфавитом символы
        case 1040: //А
          BASE::print("A");
          break;
        case 1042: //В
          BASE::print("B");
          break;
        case 1045: //Е
          BASE::print("E");
          break;
        case 1025: //Ё
          BASE::print("E");
          break;
        case 1050: //К
          BASE::print("K");
          break;
        case 1052: //M
          BASE::print("M");
          break;
        case 1053: //H
          BASE::print("H");
          break;
        case 1054: //O
          BASE::print("O");
          break;
        case 1056: //P
          BASE::print("P");
          break;
        case 1057: //C
          BASE::print("C");
          break;
        case 1058: //T
          BASE::print("T");
          break;
        case 1061: //X
          BASE::print("X");
          break;
        case 1072: //а
          BASE::print("a");
          break;
        case 1077: //е
          BASE::print("e");
          break;
        case 1086: //o
          BASE::print("o");
          break;
        case 1088: //p
          BASE::print("p");
          break;
        case 1089: //c
          BASE::print("c");
          break;
        case 1091: //y
          BASE::print("y");
          break;
        case 1093: //x
          BASE::print("x");
          break;
        case 0x00B0: //Знак градуса
          BASE::write(223);
          break;
        //Английский алфавит без изменения
        default:
          BASE::print((char)_chr);
          break;
      }
    }

    uint8_t mbtowc(wchar_t *_chr, char *_str, uint8_t mb_num)
    {
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
    int symbol_index;//Индекс символа (от 0 до 7)
    uint8_t cursor_col;
    uint8_t cursor_row;

    //===Буквы====
    uint8_t index_rus_B;
    uint8_t index_rus_G;
    uint8_t index_rus_D;
    uint8_t index_rus_ZH;
    uint8_t index_rus_Z;
    uint8_t index_rus_I;
    uint8_t index_rus_II;
    uint8_t index_rus_L;
    uint8_t index_rus_P;
    uint8_t index_rus_U;
    uint8_t index_rus_F;
    uint8_t index_rus_TS;
    uint8_t index_rus_CH;
    uint8_t index_rus_SH;
    uint8_t index_rus_SCH;
    uint8_t index_rus_tverd;
    uint8_t index_rus_Y;
    uint8_t index_rus_myagk;
    uint8_t index_rus_EE;
    uint8_t index_rus_YU;
    uint8_t index_rus_YA;
    uint8_t index_rus_b;
    uint8_t index_rus_v;
    uint8_t index_rus_g;
    uint8_t index_rus_d;
    uint8_t index_rus_yo;
    uint8_t index_rus_zh;
    uint8_t index_rus_z;
    uint8_t index_rus_i;
    uint8_t index_rus_ii;
    uint8_t index_rus_k;
    uint8_t index_rus_l;
    uint8_t index_rus_m;
    uint8_t index_rus_n;
    uint8_t index_rus_p;
    uint8_t index_rus_t;
    uint8_t index_rus_u;
    uint8_t index_rus_f;
    uint8_t index_rus_ts;
    uint8_t index_rus_ch;
    uint8_t index_rus_sh;
    uint8_t index_rus_sch;
    uint8_t index_rus_tverd_mal;
    uint8_t index_rus_y;
    uint8_t index_rus_myagk_mal;
    uint8_t index_rus_ee;
    uint8_t index_rus_yu;
    uint8_t index_rus_ya;
};

#endif
