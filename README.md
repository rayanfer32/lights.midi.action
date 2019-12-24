# lights.midi.action
A simple midi playback program which outputs midi to GPIO ports using timidity and alsa audio playback! 

lightsequence  working but instructions are not yet complete.
=============

Code to toggle relays via RasPi GPIO using Midi input

This is more of a bank of knowledge for me to recreate this project should anything happen to corrupt my set up.  But ya'll are more than welcome to the info.

Derived from lightorgan.c which is used to control GPIO pins with midi playback in ALSA.  The source and instructions are available at: http://chivalrytimberz.wordpress.com/2012/12/03/pi-lights/

# Compilation
`gcc -lwiringPi -lasound -g -Wall lightorgan8.c`

lightorgan8.c  - Modified to use 1st eight pitch of scale rather than approximating across the entire 12 pitch scale.

lightsequence.c - modified further to allow user to indicate 8 or 16 relay/pin control.  Uses midi channels 0 for 8ssr and 1 for 16ssr.

Starting from scratch on the Raspi, you need to install:

	Raspi OS (Raspbian, Debian, etc.) (www.raspberrypi.org/downloads)
	timidity (apt-get install timidity)
	ALSA Devel libs (should already be present)
	Gordon's wiringPi library (https://projects.drogon.net/raspberry-pi/wiringpi/)
(git clone git://git.drogon.net/wiringPi)
		Build it from the wiringPi directory:

```sh
			cd wiringPi
			./build

		Test it:

			gpio -v
			gpio readall


Need GIT? - (sudo apt-get install git-core)

Optionally, if you want the original lightorgan.  You will need subversion to grab a copy.
	subversion (apt-get install subversion)
	lightorgan (svn checkout http://pi-lightorgan.googlecode.com/svn/trunk/ pi-lightorgan-read-only)
```
Timidity stock settings are too aggressive for the Pi, modify the /etc/timidity/timidity.cfg file.  Uncomment these lines and restart Timidity.
```sh
opt EFresamp=d          #disable resampling
opt EFvlpf=d            #disable VLPF
opt EFreverb=d          #disable reverb
opt EFchorus=d          #disable chorus
opt EFdelay=d           #disable delay
opt anti-alias=d        #disable sample anti-aliasing
opt EWPVSETOZ           #disable all Midi Controls
opt p32a                #default to 32 voices with auto reduction
opt s32kHz              #default sample frequency to 32kHz
```

Hardware:

Pin chart at: https://projects.drogon.net/raspberry-pi/wiringpi/pins/

Connect pins as appropriate to your setup.

<b>MIDI file creation:</b>

You should be able to use any midi file but the purpose of this program is to design a specific light sequence.  I've been using the open source program, Aria Maestosa, to drop notes into patterns on channel 0 on the 8 notes starting at C.  Each note toggles a corresponding relay on/off.  The midi sounds terrible when played but gives me a complete control of the pattern.  An MP3 is played to the speaker and the Midi file is played to the GPIO.  Details at ...coming soon at http://hobbies.boguerat.com/?page_id=139

<b>Test files included in the repo:</b>

midilighttest8.mid - Tests playing first 8 notes on Channel 0.  Cycles pins from 0 to 7 in progressively faster scales.  1/4 second notes through 1/32 second notes.

midilighttest16.mid - Eight notes played on each Channel, 0 and 1.  Untested until I get another SSR board.
```sh
mpg123 mortalkombat2.mp3 & sleep 2.3 && aplaymidi –port 14:0 mcnodrum.mid
```

## Set static ip for Pi (Optional)
Before you begin with the assignment of a private IP address for Raspberry Pi, check whether DHCPCD is already activated using the following command:
```sh
sudo service dhcpcd status
```
In case it’s not, activate DHCPCD as follows:
```sh
sudo service dhcpcd start
sudo systemctl enable dhcpcd
```
For the editing of the activated DHCPCDs, start by opening the configuration file /etc/dhcpcd.confand running the following command:

`sudo nano /etc/dhcpcd.conf`

Modify the static server addresses:

```sh
interface usb0
static ip_address=192.168.42.111/24
static routers=192.168.42.1
```

## I'd like to move ssh to a different port. For some reason I'm having trouble doing this... I ran
Code: Select all

`sudo nano /etc/ssh/sshd_config`

Uncomment port and set it to `22222`

Download fwd (Port fowarding app):
configuration:
![fwd](fwd.jpg | width=100)


### my pi0 didnot have wifi module inbulit so i had to go through all this stuff!
### I wanted the song to play on my android device over lan(spare old samsung duos) by the way it works fine.... just outdated
## Tasker http server (plugin)
```sh
configure tasker server to recieve all the messages (intent) of the commands sent from the pi0 
```
# Finally
```sh
python3 seq.py
```

