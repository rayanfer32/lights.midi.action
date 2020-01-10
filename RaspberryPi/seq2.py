# light midi sequences playlist

import os
import random
import urllib.request as ur
import time

#setup lightorgan
os.system("./log")

play_count = 0
direc = "midi_files/"

print("Select play Mode:")
mode = input("1.Play All(loop)\n2.Play one\n")
midi_list = os.listdir(direc)

def play(midi_file):
	try:
		#send command to android phone running tasker
		ur.urlopen("http://192.168.1.106:5555/"+midi_file)
	except:
		#dont care errors
		print("sent play command")
	#play midi on raspi
	os.system("aplaymidi -p 14:0 "+midi_file)

if mode == "2":
	for song_id in range(len(midi_list)-1):
		print(song_id,midi_list[song_id])
	user_choice = midi_list[int(input("Enter song id:"))]
	midi_file = direc+user_choice
	play(midi_file)

elif mode == "1":
	while 1:
		#time.sleep(1)
		# load random midi 
		random_song = random.choice(midi_list)
		print(random_song)
		midi_file = direc+random_song
		#light_file = "light_files/"+random_song+".mid"
		#mp3_file = "mp3_files/"+ random_song +".mp3"
		play(midi_file)
		
		print("Playcount : "+str(play_count))
		play_count += 1
		time.sleep(1)

