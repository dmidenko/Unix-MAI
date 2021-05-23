from subprocess import PIPE, Popen
import time
import os
import csv

# where we store usage log
TIME_FILENAME = 'times.csv'
# path where this py file exists
SCRIPT_PATH = os.path.dirname(os.path.realpath(__file__))
# if we ran script and internet is connected
START_TIME = time.time()

# now all ready to spawn new process running dbus and parse stdout of first one
dbus_proc = Popen(["dbus-monitor", "--system",
                   "sender=org.freedesktop.NetworkManager, path=/org/freedesktop/NetworkManager, member=StateChanged"],
                  stdout=PIPE,
                  stderr=PIPE)
while True:
    # read line from dbus process
    k = dbus_proc.stdout.readline()
    # we get bytes from pipe, so decode it and split( https://pythonz.net/references/named/str.split/ )
    words = k.decode('UTF-8').split()
    # connection code comes from word 'uint32' sooo lets find index of its element in array
    try:
        element_index = words.index('uint32')
    except ValueError:
        continue

    # code goes after index, so we should take index+1 and makes convert it to integer
    conn_code = int(words[element_index + 1])
    print('Got change state with code:', conn_code)

    # we have 2 main codes that re 70 - connected and 20 - disconnected
    if conn_code == 70:
        START_TIME = time.time()
    # if 70 we should write log to file
    if (conn_code == 20 or conn_code == 40) and START_TIME is not None:
        # open file with flag append (read here https://www.programiz.com/python-programming/methods/built-in/open)
        output_file = open(os.path.join(SCRIPT_PATH, TIME_FILENAME), 'a')
        # creating csv writer for opened file
        csv_writer = csv.writer(output_file)
        csv_writer.writerow([START_TIME, time.time()])
        START_TIME = None
        # append changes from buffer
        output_file.flush()
        # don`t forget to close file on exit
        output_file.close()
