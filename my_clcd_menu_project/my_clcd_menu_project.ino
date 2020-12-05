#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Bounce2.h>

#define SWITCH_PIN 8
#define DT_PIN 9
#define CLK_PIN 10

byte menuCursor[8] = {
  B01000, //  *
  B00100, //   *
  B00010, //    *
  B00001, //     *
  B00010, //    *
  B00100, //   *
  B01000, //  *
  B00000  //
};

char * menuItems[] = {"Alarm Setting  ", "Adjust Time    ", "Back to Main  "};
int menuNum = sizeof(menuItems) / sizeof(char *); // array size
int menuStep = 0;
int itemStep = 0;

int alarm = 0;
int alarmItemStep = 0;

int lcdCols = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, lcdCols, lcdRows); // Change to (0x27,16,2) for 16x2 LCD.
Bounce debouncer = Bounce();

void setup() {

  pinMode(SWITCH_PIN, INPUT_PULLUP);

  // Initializes and clears the LCD screen
  lcd.init();
  lcd.backlight();

  lcd.createChar(0, menuCursor);

  debouncer.attach(SWITCH_PIN);
  debouncer.interval(5); // interval in ms
}

void loop() {
  debouncer.update();  // Update the Bounce instance
  if (debouncer.fell()) menuStep++; // 버튼이 클릭되면 1씩 증가

  if (menuStep == 0) clockScreen();
  else if (menuStep > 0) displayMenu();

}


void clockScreen() {
  lcd.setCursor(0, 0); // 이곳에 메인 기능 (시계 디스플레이) 넣는다
  lcd.print("CLCD MENU TEST");
  lcd.setCursor(0, 1);
  lcd.print("           ");
}

void displayMenu() {
  int change = getEncoderTurn();
  itemStep = itemStep + change;
  change = 0;
  if (itemStep < 0) itemStep = 0;
  else if (itemStep >= menuNum) itemStep = menuNum - 1;

  if (menuStep == 1) {
    lcd.setCursor(0, 0);
    lcd.print(" << SETTINGS >> ");
    lcd.setCursor(0, 1);
    lcd.write(menuCursor);
    //lcd.print(" ");
    lcd.print(itemStep + 1);
    lcd.print(".");
    lcd.print(menuItems[itemStep]);

  } else if (menuStep == 2) {
    switch (itemStep) {
      case 0: // 알람 세팅
        setting_alarm();
        break;
      case 1: // 시간 세팅
        setting_time();
        break;
      case 2: // 초기 화면으로 돌아가기
      default:
        menuStep--;
        itemStep = 0;
        break;
    }

  } else {
    lcd.clear();
    menuStep--; // 전 메뉴로 돌아가기
    itemStep = 0;
  }

}


int setting_alarm() {             // 알람 셋팅 화면

  while (menuStep == 2) {
    int change = getEncoderTurn();
    alarmItemStep = alarmItemStep + change;
    change = 0;
    if (alarmItemStep < 0) alarmItemStep = 0;
    else if (alarmItemStep > 2) alarmItemStep = 2;

    {
      switch (alarmItemStep) { // 알람 셋팅 기능
        case 0: // 알람 ON/OFF 세팅
          

          break;
        case 1: // 알람 시간 세팅
          setting_alarm_time();
          break;

        case 2: // 전 메뉴로 돌아가기
          menuStep--;
          alarmItemStep = 0;
          itemStep = 0;

        default:

          break;
      }
    }

    switch (alarmItemStep) { // 알람 셋팅 디스플레이
      case 0: // 알람 ON/OFF 화면
        lcd.noAutoscroll();
        lcd.setCursor(0, 0);
        lcd.print("<<Alarm ON/OFF>>");
        lcd.setCursor(0, 1);
        lcd.write(menuCursor);
        lcd.print(" Alarm is: ");

        if (alarm == 0) lcd.print("OFF ");
        else lcd.print("ON  ");

        debouncer.update();
        if (debouncer.fell()) alarm = !alarm;

          break;

      case 1: // 알람 시간 조정
        lcd.setCursor(0, 0);
        lcd.print(" <<Alarm Time>> ");
        lcd.setCursor(0, 1);
        lcd.autoscroll();
        lcd.write(menuCursor);
        lcd.print(" Click to Change Time");
        break;

      case 2: // 전 메뉴로 돌아가기
        lcd.noAutoscroll();
        lcd.setCursor(0, 0);
        lcd.print("<<  Go Back!  >>");
        lcd.setCursor(0, 1);
        lcd.write(menuCursor);
        lcd.print(" Click to Back   ");
        break;

      default:
        break;
    }
  }

  return 0;
}

int setting_alarm_time() {
  return 0;
}

int setting_time() {
  return 0;
}

int getEncoderTurn() {  // 로터리 엔코더 값 반환 (-1, 0, +1)
  static int oldInc = LOW;
  static int oldDec = LOW;
  int result = 0;
  int newInc = digitalRead(CLK_PIN);
  int newDec = digitalRead(DT_PIN);

  if (newInc != oldInc || newDec != oldDec) {
    if (oldInc == LOW && newInc == HIGH) {
      result = -(oldDec * 2 - 1);
    }
  }

  oldInc = newInc;
  oldDec = newDec;

  return result;
}
