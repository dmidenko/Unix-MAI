import sys
import time
import os


class Session:
    store_path = '/var/time_log/'

    def __init__(self):
        self._file_name = Session.store_path + str(int(time.time()))
        self._file = open(self._file_name, 'w')

    def destroy(self):
        self._file.close()
        os.remove(self._file_name)

    def close(self):
        self._file.write(str(int(time.time())))
        self._file.close()


ses = Session()


def change_state(code):
    global ses
    if ses is not None and (code == 20 or code == 40):
        ses.close()
        ses = None
    if code == 70:
        if ses is not None:
            ses.destroy()
        ses = Session()


if __name__ == '__main__':
    while True:
        msg_arr = sys.stdin.readline().split()
        for i in range(len(msg_arr)):
            if msg_arr[i] == 'uint32':
                print('got code:', msg_arr[i + 1])
                change_state(int(msg_arr[i + 1]))
