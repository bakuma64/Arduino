
// Keyboard Reprogram


#include <hidboot.h>
#include <usbhub.h>
#include <Adafruit_NeoPixel.h>

//#include <Keyboard.h>
#include <HID-Project.h>
#include <HID-Settings.h>

#define PIN        7
#define NUMPIXELS 8
int inByte = 0;
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int demor = 0;
int  r;
int g;
int b;



void Demo(int r, int g, int b);

unsigned long timing; // Переменная для хранения точки отсчета

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
    bool raskl;

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

  
    // инициализация раскладки
  if (c == 180 && mod ==4 )
  {
    raskl = !raskl;
    
  }
    
  // скопировать CTRL+C
  if (c == 142)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('c');
    Keyboard.releaseAll();

  }

  // вставить CTRL+V
  if (c == 161)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('v');
    Keyboard.releaseAll();
  }

  // вырезать CTRL+X
  if (c == 163)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('x');
    delay(100);
    Keyboard.releaseAll();
  }

  // смена раскладки ALT+SHIFT
  if (c == 180)
  {
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_LEFT_SHIFT);
   
    {
  Serial.begin(9600);
  
  if (raskl == 1) { // RU
    for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...
      pixels.setPixelColor(i, pixels.Color(0, 255, 5));
      pixels.show();   // Send the updated pixel colors to the hardware.
    }
    delay(10); // Pause before next pass through loop
  }
  else if (raskl == 0) { // EN
    for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...

      pixels.setPixelColor(i, pixels.Color(255, 0, 5));
      pixels.show();   // Send the updated pixel colors to the hardware.
      delay(10); // Pause before next pass through loop
    }
  }
}
raskl = !raskl;
    Keyboard.releaseAll();
  }

  // назад CTRL+Z
  if (c == 165)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('z');
    Keyboard.releaseAll();
  }

  // вперед CTRL+Y
  if (c == 141)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('y');
    Keyboard.releaseAll();
  }

  // свернуть SHIFT+CTRL+NUMPAD(-)
  if (c == 140)
  {
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEYPAD_SUBTRACT);
    Keyboard.releaseAll();
  }

  // развернуть SHIFT+CTRL+NUMPAD(+)
  if (c == 158)
  {
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEYPAD_ADD);
    Keyboard.releaseAll();
  }

  // результат F2
  if (c == 144)
  {
    Keyboard.press(KEY_F2);
    Keyboard.releaseAll();
  }

  // SHIFT+F9
  if (c == 157)
  {
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_F9);
    Keyboard.releaseAll();
  }

  // закрыть-открыть служ.сообщения
  if (c == 166 && peremKEYsluzgebnie == false && mod == 0)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press('z');
    peremKEYsluzgebnie = true;
    Keyboard.releaseAll();
  }
  else if (c == 166 && peremKEYsluzgebnie == true && mod == 0)

  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press('o');
    peremKEYsluzgebnie = false;
    Keyboard.releaseAll();
  }

  // очистить служ.сообщения
  if (c == 167 && mod == 0)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press('z');
    Keyboard.releaseAll();
  }

  // форматирование
  if (c == 168 && mod == 0)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('a');
    Keyboard.releaseAll();

    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press('f');
    Keyboard.releaseAll();

    Keyboard.press(KEY_LEFT);
    Keyboard.releaseAll();
  }

  // проверка синтаксиса
  if (c == 169)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_F7);
    Keyboard.releaseAll();
  }

  // F10
  if (c == 195)
  {
    Keyboard.press(KEY_F10);
    Keyboard.releaseAll();
  }

  // F11
  if (c == 198)
  {
    Keyboard.press(KEY_F11);
    Keyboard.releaseAll();
  }

  // помощь
  if (c == 194)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_F1);
    Keyboard.releaseAll();
  }

  // F7
  if (c == 170)
  {
    Keyboard.press(KEY_F7);
    Keyboard.releaseAll();
  }

  // F5
  if (c == 171)
  {
    Keyboard.press(KEY_F5);
    Keyboard.releaseAll();
  }

  // шаблон
  if (c == 156)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('q');
    Keyboard.releaseAll();
  }

  // табло
  if (c == 162)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press('w');
    Keyboard.releaseAll();
  }

  // ALT+F4
  if (c == 159)
  {
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_F4);
    Keyboard.releaseAll();
  }

  // выбрать все
  if (c == 145)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('a');
    Keyboard.releaseAll();
  }

  // backspace
  if (c == 146)
  {
    Keyboard.press(KEY_BACKSPACE);
    Keyboard.releaseAll();
  }

  // print
  if (c == 177)
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('p');
    Keyboard.releaseAll();
  }

  // vol+
  if (c == 179 && mod == 0)
  {
    Consumer.write(MEDIA_VOL_UP);
    Consumer.write(MEDIA_VOL_UP);
    Consumer.write(MEDIA_VOL_UP);
    Keyboard.releaseAll();
  }

  //vol-
  if (c == 193 && mod == 0)
  {
    Consumer.write(MEDIA_VOLUME_DOWN);
    Consumer.write(MEDIA_VOLUME_DOWN);
    Consumer.write(MEDIA_VOLUME_DOWN);
    Keyboard.releaseAll();
  }

  //mute
  if (c == 189 && mod == 0)
  {
    Consumer.write(MEDIA_VOL_MUTE);
    Keyboard.releaseAll();
  }

  //next track ctrl+TAB
  if (c == 179  && mod == 1 )
  {
    Consumer.write(MEDIA_NEXT);
    Keyboard.releaseAll();
  }

  //PREVIOUS track ctrl + CAPS LOCK
  if (c == 193  && mod == 1 )
  {
    Consumer.write(MEDIA_PREVIOUS);
    Keyboard.releaseAll();
  }

  //включение красного
  if (c == 166  && mod == 1)
  {
    Keyboard.releaseAll();
    r = 255;
    g = 0;
    b = 0;

    Demo(r, g, b);
  }



  //включение зеленого
  if (c == 167  && mod == 1)
  {
    Keyboard.releaseAll();
    r = 0;
    g = 255;
    b = 0;

    Demo(r, g, b);
  }

  //включение синего
  if (c == 168  && mod == 1)
  {
    Keyboard.releaseAll();
    r = 0;
    g = 0;
    b = 255;

    Demo(r, g, b);
  }

  //включение фиолетого
  if (c == 169  && mod == 1)
  {
    Keyboard.releaseAll();
    r = 255;
    g = 0;
    b = 255;

    Demo(r, g, b);
  }

  //включение светло голубого
  if (c == 170  && mod == 1)
  {
    Keyboard.releaseAll();
    r = 0;
    g = 255;
    b = 255;

    Demo(r, g, b);
  }

  //включение желтого
  if (c == 171  && mod == 1)
  {
    Keyboard.releaseAll();
    r = 255;
    g = 255;
    b = 0;

    Demo(r, g, b);
  }

  //выключение кубика
  if (c == 143  && mod == 1)
  {
    Keyboard.releaseAll();
    r = 0;
    g = 0;
    b = 0;

    Demo(r, g, b);
  }



  //запуск yandex music
  // для запуска необходимо в свойствах ярлыка выбрать в поле Быстрый вызов нажать клавишу q
  if (c == 189  && mod == 1 )
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press('q');
    Keyboard.releaseAll();
  }

  // PLAY MEDIA на клавишу F3
  if (c == 196)
  {
    Consumer.write(MEDIA_PLAY_PAUSE);
    Keyboard.releaseAll();
  }
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
  demor = 0;
 

  // Keyboard.press(wrapKey);
 // Serial.println(c);
  
// Serial.println(mod);
  
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

  bool raskl = false;

  // safe guard to prevent not beeing able to reprogram arduino
  // on some arduino types that can happen if serial, keyboard and/or mouse input gets sent to pc constantly directly after boot
  pinMode(LED_BUILTIN, OUTPUT);
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  for (int i = 1; i < 8; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000 - (50 * i));
    digitalWrite(LED_BUILTIN, LOW);
    pixels.setPixelColor(i, pixels.Color(0, i * 30, i * 30));
    pixels.show();
    delay(50 * i);
    if (i == 7) {
      pixels.fill(pixels.Color(0, 150, 255));
      pixels.show();
    }
  }
  {
    Serial.begin(9600);
    Serial.println("Ready");
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

void Demo(int r, int g, int b) {

  for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(r, g, b));
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(10); // Pause before next pass through loop
  }
}
