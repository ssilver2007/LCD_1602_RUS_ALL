#ifndef _LCD_1602_RUS_ALL_h
#define _LCD_1602_RUS_ALL_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <avr/pgmspace.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
#include <Print.h>

#define BYTE 0
#ifdef ARDUINO_ARCH_AVR
typedef uint32_t _uint_farptr_t;
#else
typedef uint8_t* _uint_farptr_t;
#ifndef memcpy_PF
#define memcpy_PF(dest, src, len) memcpy((dest), (src), (len))
#endif
#endif

class LCD_1602_RUS_I2C : public LiquidCrystal_I2C {
  public:
    LCD_1602_RUS_I2C(uint8_t, uint8_t, uint8_t, uint8_t = 0);
    void print(const wchar_t[]);
    void print(const char[]);
    void print(int, int = DEC);
    void print(unsigned int, int = DEC);
    void print(long, int = DEC);
    void print(unsigned long, int = DEC);
    void print(const String &);
    void print(char, int = BYTE);
    void print(unsigned char, int = BYTE);
    void print(double, int = 2);
    void clear();
    void setCursor(uint8_t, uint8_t);
    uint8_t getCursorCol();
    uint8_t getCursorRow();
    wchar_t *asciiutf8(unsigned char);

  private:
    void CharSetToLCD(uint8_t *, uint8_t *);
    void ResetAllIndex();
    void printwc(const wchar_t);
    uint8_t mbtowc(wchar_t *, char *, uint8_t);

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
class LCD_1602_RUS_10PIN : public LiquidCrystal {
  public:
    LCD_1602_RUS_10PIN(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t = 0);
    void print(const wchar_t[]);
    void print(const char[]);
    void print(int, int = DEC);
    void print(unsigned int, int = DEC);
    void print(long, int = DEC);
    void print(unsigned long, int = DEC);
    void print(const String &);
    void print(char, int = BYTE);
    void print(unsigned char, int = BYTE);
    void print(double, int = 2);
    void clear();
    void setCursor(uint8_t, uint8_t);
    uint8_t getCursorCol();
    uint8_t getCursorRow();
    wchar_t *asciiutf8(unsigned char);

  private:
    void CharSetToLCD(uint8_t *, uint8_t *);
    void ResetAllIndex();
    void printwc(const wchar_t);
    uint8_t mbtowc(wchar_t *, char *, uint8_t);

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

extern const uint8_t rus_B[];
extern const uint8_t rus_G[];
extern const uint8_t rus_D[];
extern const uint8_t rus_ZH[];
extern const uint8_t rus_Z[];
extern const uint8_t rus_I[];
extern const uint8_t rus_II[];
extern const uint8_t rus_L[];
extern const uint8_t rus_P[];
extern const uint8_t rus_U[];
extern const uint8_t rus_F[];
extern const uint8_t rus_TS[];
extern const uint8_t rus_CH[];
extern const uint8_t rus_SH[];
extern const uint8_t rus_SCH[];
extern const uint8_t rus_tverd[];
extern const uint8_t rus_Y[];
extern const uint8_t rus_myagk[];
extern const uint8_t rus_EE[];
extern const uint8_t rus_YU[];
extern const uint8_t rus_YA[];
extern const uint8_t rus_b[];
extern const uint8_t rus_v[];
extern const uint8_t rus_g[];
extern const uint8_t rus_d[];
extern const uint8_t rus_yo[];
extern const uint8_t rus_zh[];
extern const uint8_t rus_z[];
extern const uint8_t rus_i[];
extern const uint8_t rus_ii[];
extern const uint8_t rus_k[];
extern const uint8_t rus_l[];
extern const uint8_t rus_m[];
extern const uint8_t rus_n[];
extern const uint8_t rus_p[];
extern const uint8_t rus_t[];
extern const uint8_t rus_u[];
extern const uint8_t rus_f[];
extern const uint8_t rus_ts[];
extern const uint8_t rus_ch[];
extern const uint8_t rus_sh[];
extern const uint8_t rus_sch[];
extern const uint8_t rus_tverd_mal[];
extern const uint8_t rus_y[];
extern const uint8_t rus_myagk_mal[];
extern const uint8_t rus_ee[];
extern const uint8_t rus_yu[];
extern const uint8_t rus_ya[];

extern wchar_t char_utf8[];

#endif
