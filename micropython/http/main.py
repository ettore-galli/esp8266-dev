# This is your main script.

from _typeshed import SupportsDivMod
import machine
import time
from machine import Timer, ADC
import network

from wifi_credentials import ESSID,PASSWORD

class Worker(object):
    def __init__(self, delay:float) -> None:
        self.delay:float = delay
        self.timer = Timer(-1)
        self.adc = ADC(0)
        self.timer.init(period=100, mode=Timer.PERIODIC, callback=self.timer_callback)

    def init_wifi():
        sta_if = network.WLAN(network.STA_IF)
        ap_if = network.WLAN(network.AP_IF)
        sta_if.active(True)
        sta_if.connect(ESSID, PASSWORD)
 

    def timer_callback(self, arg):
        self.delay = self.adc.read()

    def loop(self):
        pin = machine.Pin(14, machine.Pin.OUT)
        
        while True:
            time_sleep = self.delay/1000
            pin.on()
            print("on", time_sleep)
            time.sleep(time_sleep)
            pin.off()
            print("off", time_sleep)
            time.sleep(time_sleep)


def init():
    Worker(1000).loop()
