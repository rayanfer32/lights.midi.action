# light midi sequences playlist

import os
import random
import urllib.request as ur

#setup lightorgan
os.system("./log")

play_count = 0
# play random midi
while 1:
	# load random midi 
	random_song = random.choice(os.listdir("light_files/"))
	print(random_song)
	random_song = random_song[0:-4]
	midi_file = "midi_files/"+random_song+".mid"
	#light_file = "light_files/"+random_song+".mid"
	#mp3_file = "mp3_files/"+ random_song +".mp3"

	try:
		#send command to android phone running tasker
		ur.urlopen("http://192.168.1.106:5555/"+midi_file)
	except:
		#dont care errors
		print("sent play command")
	#play midi on raspi
	os.system("aplaymidi -p 14:0 "+midi_file)
	print("Playcount : "+str(play_count))
	play_count += 1
