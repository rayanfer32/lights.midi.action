# lights.midi
A simple midi playback software which outputs midi to GPIO ports using timidity and alsa audio playback! 

## Set static ip for Pi
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
