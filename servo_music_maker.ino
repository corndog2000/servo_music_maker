/*
 *******************************************************************************
   USB-MIDI dump utility
   Copyright (C) 2013-2017 Yuuichi Akagawa

   for use with USB Host Shield 2.0 from Circuitsathome.com
   https://github.com/felis/USB_Host_Shield_2.0

   This is sample program. Do not expect perfect behavior.
 *******************************************************************************
*/
#include <Servo.h>

#include <usbh_midi.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
//USBHub Hub(&Usb);
USBH_MIDI  Midi(&Usb);

void initialize_servos();
void servo_tap(String input);
void servo_on(String input);
void servo_off(String input);
void MIDI_poll();
void doDelay(uint32_t t1, uint32_t t2, uint32_t delayTime);

boolean bFirst;
uint16_t pid, vid;

//Array to hold all states of notes
String note_que[119];

String global_buf;
extern String hex_codes[];
extern String note_codes[];

Servo s22;
Servo s23;
Servo s24;
Servo s25;
Servo s26;

void setup()
{
  s22.attach(22); //C
  s23.attach(23); //D
  s24.attach(24); //E
  s25.attach(25); //F
  s26.attach(26); //G

  //initialize_servos();
  //servo22.attach(22);  // attaches the servo on pin 9 to the servo object

  bFirst = true;
  vid = pid = 0;
  Serial.begin(115200);

  if (Usb.Init() == -1) {
    while (1); //halt
  }//if (Usb.Init() == -1...
  delay(200);

  initialize_servos();
  delay(200);
  initialize_servos();

  Serial.println("Initialized");
}

void loop()
{
  Usb.Task();
  //uint32_t t1 = (uint32_t)micros();
  if ( Usb.getUsbTaskState() == USB_STATE_RUNNING )
  {
    MIDI_poll();
    //Check for key presses and actuate the coresponding servo motors.

    //Serial.println(global_buf);
  }
  //delay(1ms)
  //doDelay(t1, (uint32_t)micros(), 1000);
}

// Poll USB MIDI Controler and send to serial MIDI
void MIDI_poll()
{
  int note;
  char strength[5];
  uint8_t bufMidi[64];
  uint16_t  rcvd;

  if (Midi.vid != vid || Midi.pid != pid) {
    //sprintf(buf, "VID:%04X, PID:%04X", Midi.vid, Midi.pid);
    //Serial.println(buf);
    vid = Midi.vid;
    pid = Midi.pid;
  }
  if (Midi.RecvData( &rcvd,  bufMidi) == 0 ) {
    uint32_t time = (uint32_t)millis();
    //sprintf(buf, "%04X%04X: ", (uint16_t)(time >> 16), (uint16_t)(time & 0xFFFF)); // Split variable to prevent warnings on the ESP8266 platform
    //Serial.print(buf);
    //Serial.print(rcvd);
    //Serial.print(':');
    //Serial.print("$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
    //Serial.println(bufMidi[2]);
    //sprintf(note, "%02X", bufMidi[2]);
    note = bufMidi[2] - 1;
    Serial.print(note);
    Serial.print(", ");

    sprintf(strength, "%02X", bufMidi[3]);
    Serial.println(strength);

    //Note finder

    if (String(strength) != "00")
        {
          servo_on(String(note_codes[note]));
        }
        else
        {
          servo_off(String(note_codes[note]));
        }

  }
  //After the if statement
  /*
    for (int i = 0; i < 119; i++)
    {
    if (note_que[i] != (""))
    {
      servo_tap(note_que[i]);
      note_que[i] == ("");
    }
    }
  */
}

// Delay time (max 16383 us)
void doDelay(uint32_t t1, uint32_t t2, uint32_t delayTime)
{
  uint32_t t3;

  if ( t1 > t2 ) {
    t3 = (0xFFFFFFFF - t1 + t2);
  } else {
    t3 = t2 - t1;
  }

  if ( t3 < delayTime ) {
    delayMicroseconds(delayTime - t3);
  }
}
