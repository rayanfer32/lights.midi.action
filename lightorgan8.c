#include <alsa/asoundlib.h>
#include <wiringPi.h>
#include <limits.h>
#include <unistd.h>
static snd_seq_t *seq_handle;
static int in_port;

#define THRUPORTCLIENT 14
#define THRUPORTPORT 0
#define MY_NUM_PINS 8

void midi_open(void)
{
    snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_INPUT, 0);

    snd_seq_set_client_name(seq_handle, "LightOrgan8");
    in_port = snd_seq_create_simple_port(seq_handle, "listen:in",
                      SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
                      SND_SEQ_PORT_TYPE_APPLICATION);
 
    if( snd_seq_connect_from(seq_handle, in_port, THRUPORTCLIENT, THRUPORTPORT) == -1) {
       perror("Can't connect to thru port");
       exit(-1);
    } 
}

snd_seq_event_t *midi_read(void)
{
    snd_seq_event_t *ev = NULL;
    snd_seq_event_input(seq_handle, &ev);
    return ev;
}

//Currently playing note, by pin
int pinNotes[MY_NUM_PINS];

//Currently playing channel, by pin
int pinChannels[MY_NUM_PINS];

//Enabled channels
int playChannels[0];  //Changed from 16 to 0

void clearPinNotes() {
   int i;
   for(i=0; i<MY_NUM_PINS; i++) {
      pinNotes[i] = -1;
   }
}

void clearPinChannels() {
   int i;
   for(i=0; i<MY_NUM_PINS; i++) {
      pinChannels[i] = INT_MAX;
   }
}

void clearPinsState() {
  clearPinNotes();
  clearPinChannels();
}

void pinsOn() {
   int i;
   for(i=0; i<MY_NUM_PINS; i++) {
      digitalWrite(i, 1); 
   }
}

void pinsOff() {
   int i;
   for(i=0; i<MY_NUM_PINS; i++) {
      digitalWrite(i, 1); 
   }
}

void setPlayChannel(int channel, int val) {
  if(val == 0) printf("dont play channel : %2d\n", channel);
  playChannels[channel] = val;  
}

void resetPlayChannels() {
  int i = 0;
  for(i=0 ; i < 16; i++) {
    playChannels[i] = 1;
  }
}

void midi_process(snd_seq_event_t *ev)
{
    //If this event is a PGMCHANGE type, it's a request to map a channel to an instrument
    if( ev->type == SND_SEQ_EVENT_PGMCHANGE )  {
       //Clear pins state, this is probably the beginning of a new song
       clearPinsState();
       setPlayChannel(ev->data.control.channel, 1);
    }
    else if ( ((ev->type == SND_SEQ_EVENT_NOTEON)||(ev->type == SND_SEQ_EVENT_NOTEOFF)) ) { //Note on/off event

        int isOn = 1;
        //Note velocity == 0 means the same thing as a NOTEOFF type event
        if( ev->data.note.velocity == 0 || ev->type == SND_SEQ_EVENT_NOTEOFF) {
           isOn = 0;
        }

        //There are 12 different kinds of notes.  (I will only use the first 8 for my rig but I need to the modulus.  will be 0-11.)
        int pinIdx = ev->data.note.note % 12;
        
        //If pin is set to be turned on
        if( isOn ) {
           //If pin is currently available to play a note, or if currently playing channel can be overriden due to higher priority channel
           if( pinNotes[pinIdx] == -1 || pinChannels[pinIdx] > ev->data.note.channel )  {
              //Write to the pin, save the note to pinNotes
              digitalWrite(pinIdx, 1); 
              pinNotes[pinIdx] = ev->data.note.note;
              pinChannels[pinIdx] =  ev->data.note.channel;
           }
        }
        else {  //Pin is to be turned off
           //If this is the note that turned the pin on..
           if( pinNotes[pinIdx] == ev->data.note.note && pinChannels[pinIdx] == ev->data.note.channel ) {
              //Write to the pin, indicate that pin is available
              digitalWrite(pinIdx, 0); 
              pinNotes[pinIdx] = -1;
              pinChannels[pinIdx] = INT_MAX;
           }
        }
    }
    
    else {
       printf("Unhandled event %2d\n", ev->type);
    }
    snd_seq_free_event(ev);
}  // end of midi_process

int main()
{
    //Start as a daemon
    if( daemon(0,0) != 0) {
      exit(1);
    }
    
    //Setup wiringPi
    if( wiringPiSetup() == -1) {
      exit(1);
    }
   
    //Setup all the pins to use OUTPUT mode
    int i=0;
    for(i=0; i< MY_NUM_PINS; i++) {
      pinMode(i, OUTPUT);
    }

    clearPinsState();
    resetPlayChannels();

    //Open a midi port, connect to thru port also
    midi_open();

    //Process events forever
    while (1) {
       midi_process(midi_read());
    }

    return -1;
}
