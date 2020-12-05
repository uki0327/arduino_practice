// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"
#include <LiquidCrystal.h>

const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () {

  Serial.begin(57600);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.noCursor();

#ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

}

void loop () {
  DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  Serial.print(" since midnight 1/1/1970 = ");
  Serial.print(now.unixtime());
  Serial.print("s = ");
  Serial.print(now.unixtime() / 86400L);
  Serial.println("d");

  // calculate a date which is 7 days and 30 seconds into the future
  DateTime future (now + TimeSpan(7, 12, 30, 6));

  Serial.print(" now + 7d + 30s: ");
  Serial.print(future.year(), DEC);
  Serial.print('/');
  Serial.print(future.month(), DEC);
  Serial.print('/');
  Serial.print(future.day(), DEC);
  Serial.print(' ');
  Serial.print(future.hour(), DEC);
  Serial.print(':');
  Serial.print(future.minute(), DEC);
  Serial.print(':');
  Serial.print(future.second(), DEC);
  Serial.println();

  Serial.print("Temperature: ");
  Serial.print(rtc.getTemperature());
  Serial.println(" C");

  Serial.println();

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
  //  lcd.print(now.year(), DEC);
  //  lcd.print("/");
  //  lcd.print(now.month(), DEC);
  //  lcd.print("/");
  //  lcd.print(now.day(), DEC);
  //  lcd.print(" ");
  //  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
  //  lcd.print(rtc.getTemperature());
  //  lcd.print("c");

  lcd.print(now.toString("YY/MM/DD DDD "));
  lcd.print(rtc.getTemperature());

  lcd.setCursor(15, 0);
  lcd.write(B11011111); // 온도기호

  lcd.setCursor(0, 1);

  lcd.print("  ");
  if (now.twelveHour() < 10) {
    lcd.print("0");
    lcd.print(now.twelveHour());
  } else lcd.print(now.twelveHour());

  lcd.print(":");

  if (now.minute() < 10) {
    lcd.print("0");
    lcd.print(now.minute());
  } else lcd.print(now.minute());

  lcd.print(":");

  if (now.second() < 10) {
    lcd.print("0");
    lcd.print(now.second());
  } else lcd.print(now.second());

  lcd.print(" ");

  if (now.isPM())
    lcd.print("PM ");
  else lcd.print("AM ");

  //  lcd.print(now.hour(), DEC);
  //  lcd.print(":");
  //  lcd.print(now.minute(), DEC);
  //  lcd.print(":");
  //  lcd.print(now.second(), DEC);
  delay(950);
}
