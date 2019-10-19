// Joseph Schroedl
// joe.schroedl@outlook.com
// https://github.com/corndog2000
// 10/13/2019

//If true the arduino will attempt to load the midi file from the SD card.
//If false the arduino will expect input from a midi keyboard plugged into the usb host shield.
bool playFromSD = false;

//From MIDIFile_Play
#include <SdFat.h>
#include <MD_MIDIFile.h>

#define DEBUG(x)  Serial.print(x)
#define DEBUGX(x) Serial.print(x, HEX)
#define DEBUGS(s) Serial.print(F(s))

// SD chip select pin for SPI comms.
// Arduino Ethernet shield, pin 4.
// Default SD chip select is the SPI SS pin (10).
// Other hardware will be different as documented for that hardware.
#define  SD_SELECT  4

#define WAIT_DELAY    1000 // ms

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

// The files in the tune list should be located on the SD card
// or an error will occur opening the file and the next in the
// list will be opened (skips errors).
const char *tuneList[] =
{
  "scale.mid",
};

SdFat  SD;
MD_MIDIFile SMF;

//Servo Music Maker
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

void findServo();
void initialize_servos();
void servo_tap(String input);
void servo_on(String input);
void servo_off(String input);

void parseNotes(uint8_t bufMidi[]);

boolean bFirst;
uint16_t pid, vid;

//Array to hold all states of notes
//String note_que[119];

//String global_buf;
extern String hex_codes[];
extern String note_codes[];

//Change this constant to match the number of servo motors you have hooked up.
const int NUM_SERVOS = 12;
Servo servos[NUM_SERVOS];
int pins[NUM_SERVOS] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 34};  //, 35, 36 , 37
String notes[NUM_SERVOS] = {"A", "Bb", "B", "C", "C#", "D", "Eb", "E", "F", "F#", "G", "G#"};
int note_history[NUM_SERVOS];

//END OF GLOBAL VARIABLES

//Funciton that playes the notes
void parseNotes(uint8_t bufMidi[])
{
  int note;
  char strength[5];

  note = bufMidi[2] - 1;
  Serial.print(note);
  Serial.print(", ");

  sprintf(strength, "%02X", bufMidi[3]);
  Serial.println(strength);

  //Note finder

  if (String(strength) != "00")
  {
    Serial.print("Note_codes: ");
    Serial.println(String(note_codes[note]));
    servo_on(String(note_codes[note]));
  }
  else
  {
    servo_off(String(note_codes[note]));
  }
}

void parseNotesSD(int note, int strength)
{
  Serial.println();

  Serial.print(note);
  Serial.print(", ");

  Serial.println(strength);

  //Note finder

  if (strength != 0)
  {
    Serial.print("Note_codes: ");
    Serial.println(String(note_codes[note]));
    servo_on(String(note_codes[note]));
  }
  else
  {
    servo_off(String(note_codes[note]));
  }
}

//From MIDIFile_Play
void midiCallback(midi_event *pev)
// Called by the MIDIFile library when a file event needs to be processed
// thru the midi communications interface.
// This callback is set up in the setup() function.

//This is the code that prints out which notes are being played
{
  DEBUG("\n");
  DEBUG(millis());
  DEBUG("\tM T");
  DEBUG(pev->track);
  DEBUG(":  Ch ");
  DEBUG(pev->channel + 1);
  DEBUG(" Data ");
  for (uint8_t i = 0; i < pev->size; i++)
  {
    DEBUGX(pev->data[i]);
    DEBUG(' ');
  }

  parseNotesSD(pev->data[sizeof(pev->data) - 3], pev->data[sizeof(pev->data) - 2]);
}

void sysexCallback(sysex_event *pev)
// Called by the MIDIFile library when a system Exclusive (sysex) file event needs
// to be processed through the midi communications interface. Most sysex events cannot
// really be processed, so we just ignore it here.
// This callback is set up in the setup() function.
{
  DEBUG("\nS T");
  DEBUG(pev->track);
  DEBUG(": Data ");
  for (uint8_t i = 0; i < pev->size; i++)
  {
    DEBUGX(pev->data[i]);
    DEBUG(' ');
  }
}

void midiSilence(void)
// Turn everything off on every channel.
// Some midi files are badly behaved and leave notes hanging, so between songs turn
// off all the notes and sound
{
  midi_event ev;

  // All sound off
  // When All Sound Off is received all oscillators will turn off, and their volume
  // envelopes are set to zero as soon as possible.
  ev.size = 0;
  ev.data[ev.size++] = 0xb0;
  ev.data[ev.size++] = 120;
  ev.data[ev.size++] = 0;

  for (ev.channel = 0; ev.channel < 16; ev.channel++)
    midiCallback(&ev);
}

void tickMetronome(void)
// flash a LED to the beat
{
  static uint32_t  lastBeatTime = 0;
  static boolean  inBeat = false;
  uint16_t  beatTime;

  beatTime = 60000 / SMF.getTempo();  // msec/beat = ((60sec/min)*(1000 ms/sec))/(beats/min)
  if (!inBeat)
  {
    if ((millis() - lastBeatTime) >= beatTime)
    {
      lastBeatTime = millis();
      inBeat = true;
    }
  }
  else
  {
    if ((millis() - lastBeatTime) >= 100) // keep the flash on for 100ms only
    {
      inBeat = false;
    }
  }
}

//End from MIDIFile_Play

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

    parseNotes(bufMidi);

    /*
      note = bufMidi[2] - 1;
      Serial.print(note);
      Serial.print(", ");

      sprintf(strength, "%02X", bufMidi[3]);
      Serial.println(strength);

      //Note finder

      if (String(strength) != "00")
      {
      Serial.print("Note_codes: ");
      Serial.println(String(note_codes[note]));
      servo_on(String(note_codes[note]));
      }
      else
      {
      servo_off(String(note_codes[note]));
      }
    */

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

//SETUP
void setup()
{
  Serial.begin(115200);

  DEBUG("\n[MidiFile Play List]");

  // Initialize SD
  if (!SD.begin(SD_SELECT, SPI_FULL_SPEED))
  {
    DEBUG("\nSD init fail!");
    while (true) ;
  }

  // Initialize MIDIFile
  SMF.begin(&SD);
  SMF.setMidiHandler(midiCallback);

  //Attach each servo object to its coresponding GPIO pin
  for (int i = 0; i < NUM_SERVOS; i++)
  {
    if (pins[i] != -1)
    {
      servos[i].attach(pins[i]);
    }
  }

  bFirst = true;
  vid = pid = 0;

  if (Usb.Init() == -1) {
    while (1); //halt
  }//if (Usb.Init() == -1...
  delay(200);

  initialize_servos();
  delay(200);
  initialize_servos();

  Serial.println("Initialized");
}

//MAIN LOOP
void loop()
{
  if (!playFromSD)  //Expect midi keyboard inputs
  {
    Usb.Task();
    if ( Usb.getUsbTaskState() == USB_STATE_RUNNING )
    {
      MIDI_poll();
    }
  }
  else  //Read the midi file from sd card
  {
    static enum { S_IDLE, S_PLAYING, S_END, S_WAIT_BETWEEN } state = S_IDLE;
    static uint16_t currTune = ARRAY_SIZE(tuneList);
    static uint32_t timeStart;

    switch (state)
    {
      case S_IDLE:    // now idle, set up the next tune
        {
          int err;

          DEBUGS("\nS_IDLE");

          // reset LEDs

          currTune++;
          if (currTune >= ARRAY_SIZE(tuneList))
            currTune = 0;

          // use the next file name and play it
          DEBUG("\nFile: ");
          DEBUG(tuneList[currTune]);
          SMF.setFilename(tuneList[currTune]);
          err = SMF.load();
          if (err != -1)
          {
            DEBUG(" - SMF load Error ");
            DEBUG(err);
            timeStart = millis();
            state = S_WAIT_BETWEEN;
            DEBUGS("\nWAIT_BETWEEN");
          }
          else
          {
            DEBUGS("\nS_PLAYING");
            state = S_PLAYING;
          }
        }
        break;

      case S_PLAYING: // play the file
        DEBUGS("\nS_PLAYING");
        if (!SMF.isEOF())
        {
          if (SMF.getNextEvent())
            tickMetronome();
        }
        else
          state = S_END;
        break;

      case S_END:   // done with this one
        DEBUGS("\nS_END");
        SMF.close();
        midiSilence();
        timeStart = millis();
        state = S_WAIT_BETWEEN;
        DEBUGS("\nWAIT_BETWEEN");
        break;

      case S_WAIT_BETWEEN:    // signal finish LED with a dignified pause
        if (millis() - timeStart >= WAIT_DELAY)
          state = S_IDLE;
        break;

      default:
        state = S_IDLE;
        break;
    }
  }
}
