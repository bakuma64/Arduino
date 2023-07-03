
// Keyboard Reprogram



#include <hidboot.h>
#include <usbhub.h>
#include <LCD_1602_RUS.h>

LCD_1602_RUS lcd(0x27, 16, 2);

//#include <Keyboard.h>
#include <HID-Project.h>
#include <HID-Settings.h>

// In this File:
// - KeyboardWrapper
// - Global USB Hub Variables
// - setup() & loop()

//==================
// KeyboardWrapper:
//==================
const int modifierKeys[] = {KEY_F13, KEY_F14, KEY_F15, KEY_F16, KEY_F17, KEY_F18, KEY_F19, KEY_F20, KEY_F21, KEY_F22, KEY_F23, KEY_F24};

// A KeyboardReportParser that relays all pressed keys to the PC
// with an assigned modifier/wrap Key that gets relayed along side the other keys
class KeyboardWrapper : public KeyboardReportParser
{
  public:
    KeyboardWrapper(char wrapKey);
  private:
    uint8_t wrapKey;
    bool normalKeyPressed;
    bool peremKEYsluzgebnie;

  protected:
    void OnKeyDown  (uint8_t mod, uint8_t key);
    void OnKeyUp  (uint8_t mod, uint8_t key);
    void OnControlKeysChanged(uint8_t before, uint8_t after);
    void PressWrapKey(uint8_t mod, uint8_t key);
};

KeyboardWrapper::KeyboardWrapper(char varWrapKey) {
  wrapKey = varWrapKey;
}

// Handle pressing/releasing modifier keys
void KeyboardWrapper::OnControlKeysChanged(uint8_t before, uint8_t after)
{
  // nothing on input is pressed anymore, release all for PC
  if (after == 0 && !normalKeyPressed) {
    //Keyboard.releaseAll();
    //normalKeyPressed = false; // on PC side this isn't the case anymore
    exit;
  }

  MODIFIERKEYS beforeMod;
  *((uint8_t*)&beforeMod) = before;

  MODIFIERKEYS afterMod;
  *((uint8_t*)&afterMod) = after;

  // This is the first key that gets pressed. Press the WrapKey first
  if (!normalKeyPressed && before == 0 && after != 0)
    PressWrapKey(after, 0);

  // pressing new keys
  if (beforeMod.bmLeftCtrl == 0 && afterMod.bmLeftCtrl == 1 ) Keyboard.press(KEY_LEFT_CTRL);
  if (beforeMod.bmLeftShift == 0 && afterMod.bmLeftShift == 1 ) Keyboard.press(KEY_LEFT_SHIFT);
  if (beforeMod.bmLeftAlt == 0 && afterMod.bmLeftAlt == 1 ) Keyboard.press(KEY_LEFT_ALT);
  if (beforeMod.bmLeftGUI == 0 && afterMod.bmLeftGUI == 1 ) Keyboard.press(KEY_LEFT_GUI);
  if (beforeMod.bmRightCtrl == 0 && afterMod.bmRightCtrl == 1 ) Keyboard.press(KEY_RIGHT_CTRL);
  if (beforeMod.bmRightShift == 0 && afterMod.bmRightShift == 1 ) Keyboard.press(KEY_RIGHT_SHIFT);
  if (beforeMod.bmRightAlt == 0 && afterMod.bmRightAlt == 1 ) Keyboard.press(KEY_RIGHT_ALT);
  if (beforeMod.bmRightGUI == 0 && afterMod.bmRightGUI == 1 ) Keyboard.press(KEY_RIGHT_GUI);

  // releasing old keys
  if (beforeMod.bmLeftCtrl == 1 && afterMod.bmLeftCtrl == 0 ) Keyboard.release(KEY_LEFT_CTRL);
  if (beforeMod.bmLeftShift == 1 && afterMod.bmLeftShift == 0 ) Keyboard.release(KEY_LEFT_SHIFT);
  if (beforeMod.bmLeftAlt == 1 && afterMod.bmLeftAlt == 0 ) Keyboard.release(KEY_LEFT_ALT);
  if (beforeMod.bmLeftGUI == 1 && afterMod.bmLeftGUI == 0 ) Keyboard.release(KEY_LEFT_GUI);
  if (beforeMod.bmRightCtrl == 1 && afterMod.bmRightCtrl == 0 ) Keyboard.release(KEY_RIGHT_CTRL);
  if (beforeMod.bmRightShift == 1 && afterMod.bmRightShift == 0 ) Keyboard.release(KEY_RIGHT_SHIFT);
  if (beforeMod.bmRightAlt == 1 && afterMod.bmRightAlt == 0 ) Keyboard.release(KEY_RIGHT_ALT);
  if (beforeMod.bmRightGUI == 1 && afterMod.bmRightGUI == 0 ) Keyboard.release(KEY_RIGHT_GUI);
}

// On KeyDown relay key down events to PC
void KeyboardWrapper::OnKeyDown(uint8_t mod, uint8_t key)
{
  normalKeyPressed = true;

  uint8_t c = key + 136;

  if (c) {
    PressWrapKey(mod, key);
    // Keyboard.press(c);
  }
  // для считывания кодов - открыть монитор порта. после загрузки ардуино и появления надписи ready коды можно считать
  // коды клавиш находятся в файлах: коды клавиш и коды медиа клавиш
  // mod - считывает нажатие : ctrl-1, alt - 4, shift -2

  


  // скопировать CTRL+C
  if (c == 142)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('c');
    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Копировать");
  }

  // вставить CTRL+V
  if (c == 161)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('v');
    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Вставить");
  }

  // вырезать CTRL+X
  if (c == 163)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('x');
    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Вырезать");
  }

  // смена раскладки ALT+SHIFT
  if (c == 180)
  {
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_LEFT_SHIFT);
    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Смена");
    lcd.setCursor(4, 1);
    lcd.print("расладки");
  }

  // назад CTRL+Z
  if (c == 165)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('z');
    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Назад");

  }

  // вперед CTRL+Y
  if (c == 141)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('y');
    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Вперед");
  }

  // свернуть SHIFT+CTRL+NUMPAD(-)
  if (c == 140)
  {
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEYPAD_SUBTRACT);
    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Свернуть");
  }


  // развернуть SHIFT+CTRL+NUMPAD(+)
  if (c == 158)
  {
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEYPAD_ADD);
    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Развернуть");
  }

  // результат F2
  if (c == 144)
  {
    Keyboard.press(KEY_F2);
    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Результат");
  }


  // SHIFT+F9
  if (c == 157)
  {
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_F9);
    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("SHIFT+F9");
  }

  // закрыть-открыть служ.сообщения
  if (c == 166 && peremKEYsluzgebnie == false)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press('z');
    delay(100);
    peremKEYsluzgebnie = true;
    Keyboard.releaseAll();
  }
  else if (c == 166 && peremKEYsluzgebnie == true)

  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press('o');
    peremKEYsluzgebnie = false;
    delay(100);
    Keyboard.releaseAll();
  }

  // очистить служ.сообщения
  if (c == 167)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press('z');
    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Очистка");
    lcd.setCursor(3, 1);
    lcd.print("сообщений");
  }

  // форматирование
  if (c == 168)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('a');
    Keyboard.releaseAll();
    delay(100);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press('f');
    delay(100);
    Keyboard.releaseAll();
    Keyboard.press(KEY_LEFT);
    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Форматирование");
  }

  // проверка синтаксиса
  if (c == 169)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_F7);
    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Проверка");
    lcd.setCursor(3, 1);
    lcd.print("синтаксиса");

  }

  // F10
  if (c == 195)
  {
    Keyboard.press(KEY_F10);
    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Шагнуть через...");
  }

  // F11
  if (c == 198)
  {
    Keyboard.press(KEY_F11);
    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Шагнуть в...");
  }

  // помощь
  if (c == 194)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_F1);
    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Помощь");
  }

  // F7
  if (c == 170)
  {
    Keyboard.press(KEY_F7);
    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Сохранение");
  }

  // F5
  if (c == 171)
  {
    Keyboard.press(KEY_F5);
    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Продолжить");
  }

  // шаблон
  if (c == 156)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('q');
    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Шаблон");
  }

  // табло
  if (c == 162)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press('w');
    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Табло");
  }

  // ALT+F4
  if (c == 159)
  {
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_F4);
    delay(100);
    Keyboard.releaseAll();
  }

  // выбрать все
  if (c == 145)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('a');
    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Выбрать все");
  }

  // backspace
  if (c == 146)
  {
    Keyboard.press(KEY_BACKSPACE);
    delay(100);
    Keyboard.releaseAll();
  }

  // print
  if (c == 177)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('p');
    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Печать");
  }

  // vol+
  if (c == 179 && mod == 0)
  {
    Consumer.write(MEDIA_VOL_UP);
    Consumer.write(MEDIA_VOL_UP);
    Consumer.write(MEDIA_VOL_UP);

    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("VOLUME UP");
  }

  //vol-
  if (c == 193 && mod == 0)
  {
    Consumer.write(MEDIA_VOLUME_DOWN);
    Consumer.write(MEDIA_VOLUME_DOWN);
    Consumer.write(MEDIA_VOLUME_DOWN);

    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("VOLUME DOWN");
  }

  //mute
  if (c == 189 && mod == 0)
  {
    Consumer.write(MEDIA_VOL_MUTE);
    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("VOLUME MUTE");
  }

  //next track ctrl+TAB
  if (c == 179  && mod == 1 )
  {
    Consumer.write(MEDIA_NEXT);
    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("NEXT TRACK");
  }

  //PREVIOUS track ctrl + CAPS LOCK
  if (c == 193  && mod == 1 )
  {
    Consumer.write(MEDIA_PREVIOUS);
    delay(100);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("PREVIOUS");
    lcd.setCursor(6, 1);
    lcd.print("TRACK");

  }

  //запуск yandex music
  // для запуска необходимо в свойсвах ярлыка выбрать в поле Быстрый вызов нажать клавишу q
  if (c == 189  && mod == 1 )
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press('q');

    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Запуск");
    lcd.setCursor(2, 1);
    lcd.print("YANDEX MUSIC");

    delay(8000);

    Consumer.write(MEDIA_PLAY_PAUSE);
    Keyboard.releaseAll();
    delay(1000);

    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_DOWN_ARROW);
    delay(100);

    Keyboard.releaseAll();
    
  }

  // PLAY MEDIA на клавишу F3
  if (c == 196)
  {
    Consumer.write(MEDIA_PLAY_PAUSE);
    Keyboard.releaseAll();
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("PLAY/PAUSE");
  }

  /////////////////////////////////////////////////




}

// On KeyUp we release all for PC (otherwise we would need to store a list of keys to release)
void KeyboardWrapper::OnKeyUp(uint8_t mod, uint8_t key)
{
  normalKeyPressed = false;
  // Keyboard.releaseAll();
}

// Press the assigned modifier/wrap key
// maybe have special handling depending on which Keyboard/Keys pressed
void KeyboardWrapper::PressWrapKey(uint8_t mod, uint8_t key)
{
  // for some reason the keys from KeyboardReportParser are offset by 136
  uint8_t c = key + 136;


  // Keyboard.press(wrapKey);
  Serial.println(c);
  Serial.println(mod);
  Serial.println(wrapKey);
  Serial.println(normalKeyPressed);








  // }
}

//===========================
// Global USB Hub Variables:
//===========================

USB     Usb;
USBHub     Hub1(&Usb);
USBHub     Hub2(&Usb);
USBHub     Hub3(&Usb);
USBHub     Hub4(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    Keyboard1(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    Keyboard2(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    Keyboard3(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    Keyboard4(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    Keyboard5(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    Keyboard6(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    Keyboard7(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    Keyboard8(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    Keyboard9(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    Keyboard10(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    Keyboard11(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    Keyboard12(&Usb);
KeyboardWrapper Wrapper1(modifierKeys[0]);
KeyboardWrapper Wrapper2(modifierKeys[1]);
KeyboardWrapper Wrapper3(modifierKeys[2]);
KeyboardWrapper Wrapper4(modifierKeys[3]);
KeyboardWrapper Wrapper5(modifierKeys[4]);
KeyboardWrapper Wrapper6(modifierKeys[5]);
KeyboardWrapper Wrapper7(modifierKeys[6]);
KeyboardWrapper Wrapper8(modifierKeys[7]);
KeyboardWrapper Wrapper9(modifierKeys[8]);
KeyboardWrapper Wrapper10(modifierKeys[9]);
KeyboardWrapper Wrapper11(modifierKeys[10]);
KeyboardWrapper Wrapper12(modifierKeys[11]);

//==================
// setup() & loop()
//==================
void setup()
{
  // safe guard to prevent not beeing able to reprogram arduino
  // on some arduino types that can happen if serial, keyboard and/or mouse input gets sent to pc constantly directly after boot
  pinMode(LED_BUILTIN, OUTPUT);
  for (int i = 1; i < 20; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000 - (50 * i));
    digitalWrite(LED_BUILTIN, LOW);
    delay(50 * i);
  }

  {
    Serial.begin(9600);
    delay(1000);
    Serial.println("Ready");
    lcd.init();
  lcd.backlight();
     lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Ready,steady");
    lcd.setCursor(7, 1);
    lcd.print("GO");

  

    Consumer.begin();
  }
  Usb.Init();
 

  Keyboard1.SetReportParser(0, &Wrapper1);
  Keyboard2.SetReportParser(0, &Wrapper2);
  Keyboard3.SetReportParser(0, &Wrapper3);
  Keyboard4.SetReportParser(0, &Wrapper4);
  Keyboard5.SetReportParser(0, &Wrapper5);
  Keyboard6.SetReportParser(0, &Wrapper6);
  Keyboard7.SetReportParser(0, &Wrapper7);
  Keyboard8.SetReportParser(0, &Wrapper8);
  Keyboard9.SetReportParser(0, &Wrapper9);
  Keyboard10.SetReportParser(0, &Wrapper10);
  Keyboard11.SetReportParser(0, &Wrapper11);
  Keyboard12.SetReportParser(0, &Wrapper12);
  Keyboard.begin();
}

void loop()
{
  Usb.Task();
}
