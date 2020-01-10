import ftplib
session = ftplib.FTP()
session.connect("192.168.1.101",2221)
session.login()
print("LOGIN SUCCESS")
up_file = open("/home/pi/midi_files/Feliz_Navidad.mid",'rb')
session.storbinary("STOR sdcard/FN.mid",up_file)
up_file.close()
session.quit()

