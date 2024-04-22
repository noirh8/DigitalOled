
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> OLED Analog and Digital Clock with Settings Menu and Alarm

//----------------------------------------Include Library
#include "RTClib.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>
//----------------------------------------

//----------------------------------------Initialize RTC_DS1307Initialize RTC_DS1307
RTC_DS1307 rtc;
//----------------------------------------

//----------------------------------------OLED size configuration in Pixels
#define SCREEN_WIDTH 128 //--> OLED display width, in pixels
#define SCREEN_HEIGHT 32 //--> OLED display height, in pixels
//----------------------------------------

//----------------------------------------Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//----------------------------------------

//----------------------------------------Variables for Time and Date

byte _day, _month, _hour24, _hour12, _minute, _second, _dtw;
int _year;
byte hr24;
char st[2];
char _nameoftheDay[9];
char _monthsName[3];

const char daysOfTheWeek_0[] PROGMEM = "Sunday";
const char daysOfTheWeek_1[] PROGMEM = "Monday";
const char daysOfTheWeek_2[] PROGMEM = "Tuesday";
const char daysOfTheWeek_3[] PROGMEM = "Wednesday";
const char daysOfTheWeek_4[] PROGMEM = "Thursday";
const char daysOfTheWeek_5[] PROGMEM = "Friday";
const char daysOfTheWeek_6[] PROGMEM = "Saturday";

const char *const daysOfTheWeek_table[] PROGMEM = {daysOfTheWeek_0, daysOfTheWeek_1, daysOfTheWeek_2, daysOfTheWeek_3, daysOfTheWeek_4, daysOfTheWeek_5, daysOfTheWeek_6};

const char months_name_0[] PROGMEM = "Jan";
const char months_name_1[] PROGMEM = "Feb";
const char months_name_2[] PROGMEM = "Mar";
const char months_name_3[] PROGMEM = "Apr";
const char months_name_4[] PROGMEM = "May";
const char months_name_5[] PROGMEM = "Jun";
const char months_name_6[] PROGMEM = "Jul";
const char months_name_7[] PROGMEM = "Aug";
const char months_name_8[] PROGMEM = "Sep";
const char months_name_9[] PROGMEM = "Oct";
const char months_name_10[] PROGMEM = "Nov";
const char months_name_11[] PROGMEM = "Dec";

const char *const months_name_table[] PROGMEM = {
    months_name_0,
    months_name_1,
    months_name_2,
    months_name_3,
    months_name_4,
    months_name_5,
    months_name_6,
    months_name_7,
    months_name_8,
    months_name_9,
    months_name_10,
    months_name_11,
};
//----------------------------------------


byte btn_Menu_Select, btn_Up, btn_Down, btn_Flappy; //--> Variables for Buttons

byte Buzzer = 7; //--> Variable for Buzzer

//----------------------------------------Variables for Pointer / Cursor Icons on Menus
// 'Select_Top', 5x3px

const unsigned char Select_Top[] PROGMEM = {
    0x20, 0x70, 0xf8};

// 'Select_Side', 3x5px
const unsigned char Select_Side[] PROGMEM = {
    0x80, 0xc0, 0xe0, 0xc0, 0x80};

// 'Select_Buttom', 5x3px
const unsigned char Select_Buttom[] PROGMEM = {
    0xf8, 0x70, 0x20};

//----------------------------------------

//----------------------------------------Variables for Millis to update Time and Date. Also for the Alarm interval
unsigned long previousMillisGetTimeDate = 0; //--> will store last time was updated
const long intervalGetTimeDate = 1000;       //--> interval (milliseconds)
//----------------------------------------

//----------------------------------------Variables for Menus
bool Menu_Stat = false;
byte Menu_Cnt = 0;
bool Menu_Set = false;
byte Menu_Set_cnt;
byte Menu_Set_TimeDate = 0;

bool set_Hour = false;
bool set_Minute = false;
bool set_Day = false;
bool set_Month = false;
bool set_Year = false;

bool blink_cursor = false;
int _delay = 100;

byte Alarm_Stat = 0;
byte hour_alarm, minute_alarm;
#define Alarm_Stat_address 0
#define hour_alarm_address 1
#define minute_alarm_address 2
bool Set_hour_alarm = false;
bool Set_minute_alarm = false;
byte Menu_Set_Alarm = 0;
bool Alarm_Sound = false;
bool Alarm_Start = false;
bool Alarm_Trig = true;
byte Alarm_Duration = 0;
bool Blink_Alarm_Display = false;

bool stopWatchRunning = false;


void setup()
{
  Serial.begin(9600);
  delay(1000);

  //----------------------------------------Push-button configuration as INPUT
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  //----------------------------------------

  pinMode(Buzzer, OUTPUT); //--> Configure Buzzer as OUTPUT
  rtc.begin();
  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }

  if (!rtc.isrunning())
  {
    Serial.println("RTC is NOT running!");
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // following line sets the RTC to the date & time this sketch was compiled (Set the time and date based on your computer time and date)
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //-> If that doesn't work, use this line of code outside of "if (! rtc.isrunning())"
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  // following line sets the RTC to the date & time this sketch was compiled (Set the time and date based on your computer time and date)
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //-> Use this line of code if it doesn't work in "if (! rtc.isrunning())"
  // rtc.adjust(DateTime(2020, 12, 2, 22, 23, 0));
  // If the time and date are successfully set, then deactivate the code line (make the code a comment), then re-upload the code.
  // if not done then the time and date will return to the beginning when it was set when arduino is reset or restarted.

  //----------------------------------------SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  // Address 0x3C for 128x32 and Address 0x3D for 128x64.
  // But on my 128x64 module the 0x3D address doesn't work. What works is the 0x3C address.
  // So please try which address works on your module.
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; //--> Don't proceed, loop forever
  }
  //----------------------------------------

  //----------------------------------------Show initial display buffer contents on the screen
  // the library initializes this with an Adafruit splash screen.
  // Show the display buffer on the screen(Update screen). You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(100);
  //----------------------------------------

  //----------------------------------------Writes Variable value to EEPROM
  // This is only used once, if it works, comment out this line of code and upload again.
  //  EEPROM.write(Alarm_Stat_address, Alarm_Stat);
  //  delay(100);
  //  EEPROM.write(hour_alarm_address, hour_alarm);
  //  delay(100);
  //  EEPROM.write(minute_alarm_address, minute_alarm);
  //  delay(100);
  //----------------------------------------

  //----------------------------------------Read Variable value to EEPROM
  Alarm_Stat = EEPROM.read(Alarm_Stat_address);
  delay(100);
  hour_alarm = EEPROM.read(hour_alarm_address);
  delay(100);
  minute_alarm = EEPROM.read(minute_alarm_address);
  delay(100);
  //----------------------------------------
}

//________________________________________________________________________void loop()
void loop()
{
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++ To update the Time and Date
  unsigned long currentMillisGetTimeDate = millis();
  if (currentMillisGetTimeDate - previousMillisGetTimeDate >= intervalGetTimeDate)
  {
    // save the last time you blinked the LED
    previousMillisGetTimeDate = currentMillisGetTimeDate;

    //----------------------------------------Time and Date are displayed if Menu Status = False
    if (Menu_Stat == false)
    {
      GetDateTime();

      display.clearDisplay();

      //~~~~~~~~~~~~~~~~~~~Alarm start condition
      if (Alarm_Stat == 1 && _hour24 == hour_alarm && _minute == minute_alarm && Alarm_Trig == true)
      {
        Alarm_Start = true;
        Alarm_Trig = false;
      }
      //~~~~~~~~~~~~~~~~~~~

      if (_minute != minute_alarm)
      {
        Alarm_Trig = true;
      }

      //~~~~~~~~~~~~~~~~~~~A condition that is executed if the alarm starts and stops
      if (Alarm_Start == true)
      {
        Alarm_Sound = !Alarm_Sound;
        if (Alarm_Sound == true)
        {
          Button_Sound(1);
        }
        else
        {
          Button_Sound(0);
        }

        Alarm_Duration++;
        // Change the value 59 to increase or decrease the Alarm duration
        if (Alarm_Duration > 30)
        {
          Button_Sound(0);
          Alarm_Duration = 0;
          Alarm_Start = false;
        }

        display.setTextSize(1);
        Blink_Alarm_Display = !Blink_Alarm_Display;
        if (Blink_Alarm_Display == true)
        {
          display.setTextColor(WHITE);
        }
        else
        {
          display.setTextColor(BLACK);
        }

        display.setCursor(82, 22);
        display.print("Alarm");
        display.setCursor(82, 32);
        if (hour_alarm < 10)
        {
          display.print("0");
          display.print(hour_alarm);
        }
        else
        {
          display.print(hour_alarm);
        }
        display.print(":");
        if (minute_alarm < 10)
        {
          display.print("0");
          display.print(minute_alarm);
        }
        else
        {
          display.print(minute_alarm);
        }
      }
      else
      {
        Digital_Clock_and_Date(_hour24, _minute, _second, _dtw, _day, _month, _year);
      }
      //~~~~~~~~~~~~~~~~~~~

      display.display();
    }
    //----------------------------------------
  }
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  read_button();

  //----------------------------------------Stop the Alarm using Button
  if (btn_Menu_Select == LOW && Alarm_Start == true)
  {
    Button_Sound(1);
    delay(1000);
    Button_Sound(0);
    Alarm_Start = false;
    Alarm_Duration = 0;
    read_button();
  }
  //----------------------------------------

  //---------------------------------------Conditions for displaying to the Menu
  if (btn_Menu_Select == LOW && Menu_Stat == false)
  {
    Menu_Cnt = 1;
    Menu_Stat = true;
  }


  //----------------------------------------Looping to display Menu
  while (Menu_Stat == true)
  {
    read_button();
    //~~~~~~~~~~~~~~~~~~~Button Condition for selecting Setup Menu
    if (btn_Down == LOW)
    {
      Menu_Cnt++;
      if (Menu_Cnt > 4)
        Menu_Cnt = 1;
    }

    if (btn_Up == LOW)
    {
      Menu_Cnt--;
      if (Menu_Cnt < 1)
        Menu_Cnt = 4;
    }

    if (btn_Menu_Select == LOW && Menu_Cnt == 1)
    {
      Menu_Set = true;
      Menu_Set_cnt = 1;
      Menu_Set_TimeDate = 1;
    }

    if (btn_Menu_Select == LOW && Menu_Cnt == 2)
    {
      Menu_Set = true;
      Menu_Set_cnt = 2;
      Menu_Set_Alarm = 1;
      Alarm_Stat = EEPROM.read(Alarm_Stat_address);
      delay(100);
      hour_alarm = EEPROM.read(hour_alarm_address);
      delay(100);
      minute_alarm = EEPROM.read(minute_alarm_address);
      delay(100);
    }

    if(btn_Menu_Select == LOW && Menu_Cnt == 3)
    {
      stopWatchRunning = !stopWatchRunning;
    }

    if (btn_Menu_Select == LOW && Menu_Cnt == 4)
    {
      Menu_Cnt = 1;
      Menu_Stat = false;
    }
    //~~~~~~~~~~~~~~~~~~~

    //~~~~~~~~~~~~~~~~~~~Conditions for displaying Menu contents
    if (Menu_Cnt == 1)
    {
      Menu_Display(Menu_Cnt);
    }
    else if (Menu_Cnt == 2)
    {
      Menu_Display(Menu_Cnt);
    }
    else if(Menu_Cnt == 3)
    {
      Menu_Display(Menu_Cnt);
    }
    if (Menu_Cnt == 4)
    {
      Menu_Display(Menu_Cnt);
    }

    //~~~~~~~~~~~~~~~~~~~

    //~~~~~~~~~~~~~~~~~~~Looping for Time and Date Setting Menu
    while (Menu_Set == true && Menu_Set_cnt == 1)
    {
      Set_Time_Date();
      delay(50);
    }
    //~~~~~~~~~~~~~~~~~~~

    //~~~~~~~~~~~~~~~~~~~Looping for Alarm Settings Menu
    while (Menu_Set == true && Menu_Set_cnt == 2)
    {
      Set_Alarm();
      delay(50);
    }
    //~~~~~~~~~~~~~~~~~~~
    
    delay(50);
  }
  //----------------------------------------
}
//________________________________________________________________________

//________________________________________________________________________Subroutines for reading Buttons
void read_button()
{
  btn_Menu_Select = digitalRead(4);
  btn_Up = digitalRead(5);
  btn_Down = digitalRead(6);
}
//________________________________________________________________________

//________________________________________________________________________Subroutine to retrieve or update the time and date from DS1307 RTC
void GetDateTime()
{
  DateTime now = rtc.now();
  _day = now.day();
  _month = now.month();
  _year = now.year();
  _hour24 = now.hour();
  _minute = now.minute();
  _second = now.second();
  _dtw = now.dayOfTheWeek();

  hr24 = _hour24;
  if (hr24 > 12)
  {
    _hour12 = hr24 - 12;
  }
  else if (hr24 == 0)
  {
    _hour12 = 12;
  }
  else
  {
    _hour12 = hr24;
  }

  if (hr24 < 12)
  {
    strcpy(st, "AM");
  }
  else
  {
    strcpy(st, "PM");
  }
}
//________________________________________________________________________

//________________________________________________________________________Subroutines for displaying Digital Clock and Date
void Digital_Clock_and_Date(byte h, byte m, byte s, byte dtw, byte dy, byte mt, int yr)
{
  display.setTextSize(1);
  display.setTextColor(WHITE);

  //----------------------------------------To display Digital Clock
  display.setCursor(63, 0);
  if (h < 10)
  {
    display.print("0");
    display.print(h);
  }
  else
  {
    display.print(h);
  }
  display.print(":");
  if (m < 10)
  {
    display.print("0");
    display.print(m);
  }
  else
  {
    display.print(m);
  }
  display.print(":");
  if (s < 10)
  {
    display.print("0");
    display.print(s);
  }
  else
  {
    display.print(s);
  }
  //----------------------------------------

  //----------------------------------------To display Day Name
  if (Menu_Stat == false)
  {
    strcpy_P(_nameoftheDay, (char *)pgm_read_word(&(daysOfTheWeek_table[dtw])));
    byte display_center_area = (127 - 65) / 2;
    byte center_text = (strlen(_nameoftheDay) * 6) / 2;
    byte x_text = 56 + (display_center_area - center_text);
    display.setCursor(x_text, 10); //--> (x position, y position)
    display.println(_nameoftheDay);
  }
  //----------------------------------------

  //----------------------------------------To display the Date
  strcpy_P(_monthsName, (char *)pgm_read_word(&(months_name_table[mt - 1])));
  display.setCursor(53, 20); //--> (x position, y position)
  if (dy < 10)
  {
    display.print("0");
    display.print(dy);
  }
  else
  {
    display.print(dy);
  }
  display.print(",");
  display.print(_monthsName);
  display.print(",");
  display.print(yr);
  //----------------------------------------
}
//________________________________________________________________________

//_________________Subroutine to display Menu Display
void Menu_Display(byte slct)
{
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(40, 0);
  display.print("--MENU--");

  display.setCursor(7, 7);
  display.print("Set Time and Date");

  display.setCursor(7, 15);
  display.print("Set Alarm");

  display.setCursor(70, 15);
  display.print("Stopwatch");

  display.setCursor(7, 23);
  display.print("Back");

  //----------------------------------------Conditions for the cursor position on the Menu
  if (slct == 1)
  {
    display.drawBitmap(0, 8, Select_Side, 3, 5, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  }
  else if (slct == 2)
  {
    display.drawBitmap(0, 16, Select_Side, 3, 5, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  }
  else if (slct == 3)
  {
    display.drawBitmap(65, 16, Select_Side, 3, 5, WHITE);
  }
  else if(slct == 4) 
  {
    display.drawBitmap(0, 24, Select_Side, 3, 5, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  }
  //----------------------------------------

  display.display();
}
//________________________________________________________________________

//_________________________Subroutines for displaying Time and Date settings
void Set_Time_Date()
{
  read_button();

  display.clearDisplay();

  Digital_Clock_and_Date(_hour24, _minute, _second, _dtw, _day, _month, _year);

  display.setCursor(0, 20);
  display.print("Back");

  display.setCursor(27, 20);
  display.print("Save");

  //----------------------------------------Condition of Buttons for selecting Time Settings
  if (btn_Down == LOW)
  {
    Menu_Set_TimeDate++;
    if (Menu_Set_TimeDate > 7)
      Menu_Set_TimeDate = 1;
  }

  if (btn_Up == LOW)
  {
    Menu_Set_TimeDate--;
    if (Menu_Set_TimeDate < 1)
      Menu_Set_TimeDate = 7;
  }

  if (btn_Menu_Select == LOW && Menu_Set_TimeDate == 1)
  {
    set_Hour = true;
  }

  if (btn_Menu_Select == LOW && Menu_Set_TimeDate == 2)
  {
    set_Minute = true;
  }

  if (btn_Menu_Select == LOW && Menu_Set_TimeDate == 3)
  {
    set_Day = true;
  }

  if (btn_Menu_Select == LOW && Menu_Set_TimeDate == 4)
  {
    set_Month = true;
  }

  if (btn_Menu_Select == LOW && Menu_Set_TimeDate == 5)
  {
    set_Year = true;
  }

  //~~~~~~~~~~~~~~~~~~~Button Condition for saving Time and Date settings
  if (btn_Menu_Select == LOW && Menu_Set_TimeDate == 6)
  {
    rtc.adjust(DateTime(_year, _month, _day, _hour24, _minute, 0));
    Display_to_save();
  }
  //~~~~~~~~~~~~~~~~~~~

  if (btn_Menu_Select == LOW && Menu_Set_TimeDate == 7)
  {
    Menu_Set_cnt = 0;
    Menu_Set = false;
  }
  //----------------------------------------

  //----------------------------------------Conditions for the cursor position
  if (Menu_Set_TimeDate == 1)
  {
    Set_Time_Date_Display(Menu_Set_TimeDate);
  }
  else if (Menu_Set_TimeDate == 2)
  {
    Set_Time_Date_Display(Menu_Set_TimeDate);
  }
  else if (Menu_Set_TimeDate == 3)
  {
    Set_Time_Date_Display(Menu_Set_TimeDate);
  }
  else if (Menu_Set_TimeDate == 4)
  {
    Set_Time_Date_Display(Menu_Set_TimeDate);
  }
  else if (Menu_Set_TimeDate == 5)
  {
    Set_Time_Date_Display(Menu_Set_TimeDate);
  }
  else if (Menu_Set_TimeDate == 6)
  {
    Set_Time_Date_Display(Menu_Set_TimeDate);
  }
  else if (Menu_Set_TimeDate == 7)
  {
    Set_Time_Date_Display(Menu_Set_TimeDate);
  }
  //----------------------------------------

  display.display();

  //----------------------------------------Looping to set Hour
  while (set_Hour == true)
  {
    read_button();
    display.clearDisplay();

    display.setCursor(0, 0);
    display.print("Set");

    display.setCursor(0, 10);
    display.print("the hour");

    display.setCursor(0, 20);
    display.print("Back");

    display.setCursor(27, 20);
    display.print("Save");

    Digital_Clock_and_Date(_hour24, _minute, _second, _dtw, _day, _month, _year);

    if (btn_Down == LOW)
    {
      _delay = 50;
      _hour24++;
      if (_hour24 > 23)
        _hour24 = 0;
    }

    if (btn_Up == LOW)
    {
      _delay = 50;
      _hour24--;
      if (_hour24 > 254)
        _hour24 = 23;
    }

    if (btn_Menu_Select == LOW)
    {
      set_Hour = false;
    }

    if (btn_Down == HIGH && btn_Up == HIGH && btn_Menu_Select == HIGH)
    {
      _delay = 100;
    }

    blink_cursor = !blink_cursor;
    if (blink_cursor == true)
    {
      display.drawBitmap(66, 10, Select_Top, 5, 3, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
    }
    else
    {
      display.drawBitmap(66, 10, Select_Top, 5, 3, BLACK); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
    }

    display.display();
    delay(_delay);
  }
  //----------------------------------------

  //----------------------------------------Looping to set Minute
  while (set_Minute == true)
  {
    read_button();
    display.clearDisplay();

    display.setCursor(0, 0);
    display.print("Set");

    display.setCursor(0, 10);
    display.print("the minute");

    display.setCursor(0, 20);
    display.print("Back");

    display.setCursor(27, 20);
    display.print("Save");

    if (btn_Down == LOW)
    {
      _delay = 50;
      _minute++;
      if (_minute > 59)
        _minute = 0;
    }

    if (btn_Up == LOW)
    {
      _delay = 50;
      _minute--;
      if (_minute > 254)
        _minute = 59;
    }

    if (btn_Menu_Select == LOW)
    {
      set_Minute = false;
    }

    if (btn_Down == HIGH && btn_Up == HIGH && btn_Menu_Select == HIGH)
    {
      _delay = 100;
    }

    Digital_Clock_and_Date(_hour24, _minute, _second, _dtw, _day, _month, _year);

    blink_cursor = !blink_cursor;
    if (blink_cursor == true)
    {
      display.drawBitmap(84, 10, Select_Top, 5, 3, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
    }
    else
    {
      display.drawBitmap(84, 10, Select_Top, 5, 3, BLACK); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
    }

    display.display();
    delay(_delay);
  }
  //----------------------------------------

  //----------------------------------------Looping to set Day / Date
  while (set_Day == true)
  {
    read_button();
    display.clearDisplay();

    display.setCursor(0, 0);
    display.print("Set");

    display.setCursor(0, 10);
    display.print("the day");

    display.setCursor(0, 20);
    display.print("Back");

    display.setCursor(27, 20);
    display.print("Save");

    if (btn_Down == LOW)
    {
      _delay = 50;
      _day++;
      if (_day > 31)
        _day = 1;
    }

    if (btn_Up == LOW)
    {
      _delay = 50;
      _day--;
      if (_day < 1)
        _day = 31;
    }

    if (btn_Menu_Select == LOW)
    {
      set_Day = false;
    }

    if (btn_Down == HIGH && btn_Up == HIGH && btn_Menu_Select == HIGH)
    {
      _delay = 100;
    }

    Digital_Clock_and_Date(_hour24, _minute, _second, _dtw, _day, _month, _year);

    blink_cursor = !blink_cursor;
    if (blink_cursor == true)
    {
      display.drawBitmap(56, 15, Select_Buttom, 5, 3, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
    }
    else
    {
      display.drawBitmap(56, 15, Select_Buttom, 5, 3, BLACK); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
    }

    display.display();
    delay(_delay);
  }
  //----------------------------------------

  //----------------------------------------Looping to set Month
  while (set_Month == true)
  {
    read_button();
    display.clearDisplay();

    display.setCursor(0, 0);
    display.print("Set");

    display.setCursor(0, 10);
    display.print("the month");

    display.setCursor(0, 20);
    display.print("Back");

    display.setCursor(27, 20);
    display.print("Save");

    if (btn_Down == LOW)
    {
      _delay = 50;
      _month++;
      if (_month > 12)
        _month = 1;
    }

    if (btn_Up == LOW)
    {
      _delay = 50;
      _month--;
      if (_month < 1)
        _month = 12;
    }

    if (btn_Menu_Select == LOW)
    {
      set_Month = false;
    }

    if (btn_Down == HIGH && btn_Up == HIGH && btn_Menu_Select == HIGH)
    {
      _delay = 100;
    }

    Digital_Clock_and_Date(_hour24, _minute, _second, _dtw, _day, _month, _year);

    blink_cursor = !blink_cursor;
    if (blink_cursor == true)
    {
      display.drawBitmap(77, 15, Select_Buttom, 5, 3, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
    }
    else
    {
      display.drawBitmap(77, 15, Select_Buttom, 5, 3, BLACK); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
    }

    display.display();
    delay(_delay);
  }
  //----------------------------------------

  //----------------------------------------Looping to set Year
  while (set_Year == true)
  {
    read_button();
    display.clearDisplay();

    display.setCursor(0, 0);
    display.print("Set");

    display.setCursor(0, 10);
    display.print("the year");

    display.setCursor(0, 20);
    display.print("Back");

    display.setCursor(27, 20);
    display.print("Save");

    if (btn_Down == LOW)
    {
      _delay = 50;
      _year++;
      if (_year > 2100)
        _year = 2000;
    }

    if (btn_Up == LOW)
    {
      _delay = 50;
      _year--;
      if (_year < 2000)
        _year = 2100;
    }

    if (btn_Menu_Select == LOW)
    {
      set_Year = false;
    }

    if (btn_Down == HIGH && btn_Up == HIGH && btn_Menu_Select == HIGH)
    {
      _delay = 100;
    }

    Digital_Clock_and_Date(_hour24, _minute, _second, _dtw, _day, _month, _year);

    blink_cursor = !blink_cursor;
    if (blink_cursor == true)
    {
      display.drawBitmap(105, 15, Select_Buttom, 5, 3, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
    }
    else
    {
      display.drawBitmap(105, 15, Select_Buttom, 5, 3, BLACK); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
    }

    display.display();
    delay(_delay);
  }
  //----------------------------------------
}
//________________________________________________________________________

//________________________________________________________________________Subroutines for displaying a cursor to select settings
void Set_Time_Date_Display(byte slc)
{
  display.setTextSize(1);
  display.setTextColor(WHITE);

  if (slc == 1)
  {
    display.setCursor(0, 0);
    display.print("Set");
    display.setCursor(0, 10);
    display.print("the hour");
    display.drawBitmap(66, 10, Select_Top, 5, 3, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  }
  else if (slc == 2)
  {
    display.setCursor(0, 0);
    display.print("Set");
    display.setCursor(0, 10);
    display.print("the minute");
    display.drawBitmap(84, 10, Select_Top, 5, 3, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  }
  else if (slc == 3)
  {
    display.setCursor(0, 0);
    display.print("Set");
    display.setCursor(0, 10);
    display.print("the day");
    display.drawBitmap(56, 15, Select_Buttom, 5, 3, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  }
  else if (slc == 4)
  {
    display.setCursor(0, 0);
    display.print("Set");
    display.setCursor(0, 10);
    display.print("the month");
    display.drawBitmap(77, 15, Select_Buttom, 5, 3, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  }
  else if (slc == 5)
  {
    display.setCursor(0, 0);
    display.print("Set");
    display.setCursor(0, 10);
    display.print("the year");
    display.drawBitmap(105, 15, Select_Buttom, 5, 3, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  }
  else if (slc == 6)
  {
    display.setCursor(0, 0);
    display.print("Save");
    display.setCursor(0, 8);
    display.print("Settings");
    display.drawBitmap(36, 29, Select_Top, 5, 3, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  }
  else if (slc == 7)
  {
    display.drawBitmap(9, 29, Select_Top, 5, 3, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  }
}
//________________________________________________________________________

//________________________________________________________________________Subroutines for Alarm Settings
void Set_Alarm()
{
  read_button();

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);

  Set_Alarm_Display();

  //----------------------------------------Condition of Buttons for selecting Alarm Settings
  if (btn_Down == LOW)
  {
    Menu_Set_Alarm++;
    if (Menu_Set_Alarm > 5)
      Menu_Set_Alarm = 1;
  }

  if (btn_Up == LOW)
  {
    Menu_Set_Alarm--;
    if (Menu_Set_Alarm < 1)
      Menu_Set_Alarm = 5;
  }

  if (btn_Menu_Select == LOW && Menu_Set_Alarm == 1)
  {
    Set_hour_alarm = true;
  }

  if (btn_Menu_Select == LOW && Menu_Set_Alarm == 2)
  {
    Set_minute_alarm = true;
  }

  //~~~~~~~~~~~~~~~~~~~Button Condition to enable Alarm and save Alarm settings
  if (btn_Menu_Select == LOW && Menu_Set_Alarm == 3)
  {
    Display_to_save();
    Alarm_Stat = 1;
    EEPROM.update(Alarm_Stat_address, Alarm_Stat);
    delay(100);
    EEPROM.update(hour_alarm_address, hour_alarm);
    delay(100);
    EEPROM.update(minute_alarm_address, minute_alarm);
    delay(100);

    Alarm_Stat = EEPROM.read(Alarm_Stat_address);
    delay(100);
    hour_alarm = EEPROM.read(hour_alarm_address);
    delay(100);
    minute_alarm = EEPROM.read(minute_alarm_address);
    delay(100);
  }
  //~~~~~~~~~~~~~~~~~~~

  //~~~~~~~~~~~~~~~~~~~Button Condition to disable Alarm
  if (btn_Menu_Select == LOW && Menu_Set_Alarm == 4)
  {
    Display_to_save();

    Alarm_Stat = 0;
    hour_alarm = 0;
    minute_alarm = 0;
    EEPROM.update(Alarm_Stat_address, Alarm_Stat);
    delay(100);
    EEPROM.update(hour_alarm_address, hour_alarm);
    delay(100);
    EEPROM.update(minute_alarm_address, minute_alarm);
    delay(100);

    Alarm_Stat = EEPROM.read(Alarm_Stat_address);
    delay(100);
    hour_alarm = EEPROM.read(hour_alarm_address);
    delay(100);
    minute_alarm = EEPROM.read(minute_alarm_address);
    delay(100);
  }
  //~~~~~~~~~~~~~~~~~~~

  if (btn_Menu_Select == LOW && Menu_Set_Alarm == 5)
  {
    Menu_Set_Alarm = 0;
    Menu_Set = false;
  }
  //----------------------------------------

  //----------------------------------------Conditions for displaying the cursor position on the Alarm Settings
  if (Menu_Set_Alarm == 1)
  {
    Set_Alarm_Cursor(Menu_Set_Alarm);
  }
  else if (Menu_Set_Alarm == 2)
  {
    Set_Alarm_Cursor(Menu_Set_Alarm);
  }
  else if (Menu_Set_Alarm == 3)
  {
    Set_Alarm_Cursor(Menu_Set_Alarm);
  }
  else if (Menu_Set_Alarm == 4)
  {
    Set_Alarm_Cursor(Menu_Set_Alarm);
  }
  else if (Menu_Set_Alarm == 5)
  {
    Set_Alarm_Cursor(Menu_Set_Alarm);
  }
  //----------------------------------------

  //----------------------------------------Looping for setting the Hour for Alarm
  while (Set_hour_alarm == true && Menu_Set_Alarm == 1)
  {
    read_button();

    display.clearDisplay();

    if (btn_Down == LOW)
    {
      _delay = 50;
      hour_alarm++;
      if (hour_alarm > 23)
        hour_alarm = 0;
    }

    if (btn_Up == LOW)
    {
      _delay = 50;
      hour_alarm--;
      if (hour_alarm > 254)
        hour_alarm = 23;
    }

    if (btn_Menu_Select == LOW)
    {
      Set_hour_alarm = false;
    }

    if (btn_Down == HIGH && btn_Up == HIGH && btn_Menu_Select == HIGH)
    {
      _delay = 100;
    }

    Set_Alarm_Display();

    blink_cursor = !blink_cursor;
    if (blink_cursor == true)
    {
      display.drawBitmap(48, 18, Select_Top, 5, 3, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
    }
    else
    {
      display.drawBitmap(48, 18, Select_Top, 5, 3, BLACK); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
    }

    display.display();
    delay(_delay);
  }
  //----------------------------------------

  //----------------------------------------Looping for setting the Minute for Alarm
  while (Set_minute_alarm == true && Menu_Set_Alarm == 2)
  {
    read_button();

    display.clearDisplay();

    if (btn_Down == LOW)
    {
      _delay = 50;
      minute_alarm++;
      if (minute_alarm > 59)
        minute_alarm = 0;
    }

    if (btn_Up == LOW)
    {
      _delay = 50;
      minute_alarm--;
      if (minute_alarm > 254)
        minute_alarm = 59;
    }

    if (btn_Menu_Select == LOW)
    {
      Set_minute_alarm = false;
    }

    if (btn_Down == HIGH && btn_Up == HIGH && btn_Menu_Select == HIGH)
    {
      _delay = 100;
    }

    Set_Alarm_Display();

    blink_cursor = !blink_cursor;
    if (blink_cursor == true)
    {
      display.drawBitmap(66, 18, Select_Top, 5, 3, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
    }
    else
    {
      display.drawBitmap(66, 18, Select_Top, 5, 3, BLACK); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
    }

    display.display();
    delay(_delay);
  }
  //----------------------------------------

  display.display();
}
//________________________________________________________________________

//______________________Subroutines for the Alarm Settings display
void Set_Alarm_Display()
{
  if (Alarm_Stat == 0)
  {
    display.setCursor(15, 0);
    display.print("Status : Disable");
  }
  else
  {
    display.setCursor(18, 0);
    display.print("Status : Enable");
  }

  if (hour_alarm < 10)
  {
    display.setCursor(45, 9);
    display.print("0");
    display.print(hour_alarm);
  }
  else
  {
    display.setCursor(45, 9);
    display.print(hour_alarm);
  }
  display.print(":");
  if (minute_alarm < 10)
  {
    display.print("0");
    display.print(minute_alarm);
  }
  else
  {
    display.print(minute_alarm);
  }

  display.setCursor(5, 24);
  display.print("Enable");

  display.setCursor(50, 24);
  display.print("Disable");

  display.setCursor(100, 24);
  display.print("Back");
}
//________________________________________________________________________

//________________________________________________________________________Subroutines for display and cursor position in Alarm Settings
void Set_Alarm_Cursor(byte slc)
{
  if (slc == 1)
  {
    display.drawBitmap(48, 18, Select_Top, 5, 3, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  }
  else if (slc == 2)
  {
    display.drawBitmap(66, 18, Select_Top, 5, 3, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  }
  else if (slc == 3)
  {
    display.drawBitmap(0, 25, Select_Side, 3, 5, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  }
  else if (slc == 4)
  {
    display.drawBitmap(45, 25, Select_Side, 3, 5, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  }
  else if (slc == 5)
  {
    display.drawBitmap(95, 25, Select_Side, 3, 5, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  }
}
//________________________________________________________________________

//________________________________________________________________________Subroutine to display the Save
void Display_to_save()
{
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(33, 10);
  display.print("Saved");

  display.display();
}
//________________________________________________________________________
//___________________________________________Subroutines to turn the Buzzer on or off
void Button_Sound(byte snd)
{
  digitalWrite(Buzzer, snd);
}


// ____________________________________________
// _____________________________ Flappy Bird

