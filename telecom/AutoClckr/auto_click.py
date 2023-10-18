"""
python -m PyInstaller -F -w   -n auto_click ./auto_click.py
"""
import os
import signal
import time
import pyautogui
import keyboard
import sys
import threading


def exit_():
    keyboard.wait('esc')
    os.kill(os.getpid(), signal.SIGINT)


def loop_click(s=1):
    pyautogui.alert(text='Move the mouse to the place you want to click，\n press enter to start，\nesc to stop')
    keyboard.wait('enter')
    x, y = pyautogui.position()
    print(x, y)
    threading.Thread(target=exit_).start()
    while True:
        time.sleep(s)
        pyautogui.click(x, y)
        pyautogui.press('enter')


def get_delay():
    while True:
        s = pyautogui.prompt(text='Time Delay', title='AutoClicker', default=1)
        if not s:
            sys.exit(0)
        try:
            s = float(s)
            break
        except:
            pyautogui.alert(text='Error')
            continue
    return s


def main():
    s = get_delay()
    loop_click(s=s)


if __name__ == '__main__':
    main()
