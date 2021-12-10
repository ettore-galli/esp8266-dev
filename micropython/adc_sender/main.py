# ADC Network sender main

import network
import time
import socket

from machine import Timer, ADC
import urequests

import gc

try:
    from network_definitions import (
        WIFI_ESSID,
        WIFI_PASSWORD,
        DATA_ENDPOINT,
    )
except:
    print(
        "Create  a file named network_definitions.py containing the following mandatory variables:"
    )
    print("WIFI_ESSID       = Wifi network ESSID")
    print("WIFI_PASSWORD    = Wifi network PASSWORD")
    print("DATA_ENDPOINT    = Endpoint for data transmission")


CONNECTION_MONITOR_DELAY = 0.5
READ_LOOP_SLEEP = 0.2
MAXIMUM_BUFFER_LENGTH = 1000


class AdcNetworkSender(object):
    def __init__(self, sleep: float) -> None:
        self.sleep = sleep
        self.adc = ADC(0)

        self.samples = []

        self.connect_to_wifi()  # TBD: Object attributes?

        self.timer = Timer(-1)
        self.timer.init(period=10, mode=Timer.PERIODIC, callback=self.sample_callback)

        gc.collect()

    def connect_to_wifi(self):
        self.sta_if = network.WLAN(network.STA_IF)
        if not self.sta_if.isconnected():
            print("connecting to network...")
            self.sta_if.active(True)
            self.sta_if.connect(WIFI_ESSID, WIFI_PASSWORD)
            while not self.sta_if.isconnected():
                print("connecting...")
                time.sleep(CONNECTION_MONITOR_DELAY)
            print("network config:", self.sta_if.ifconfig())

    def reconnect_to_wifi(self):
        if self.sta_if.isconnected():
            self.sta_if.disconnect()
            self.connect_to_wifi()

    def collect_available_samples(self):
        available_samples = len(self.samples)
        if available_samples > 0:
            export_samples = self.samples[:available_samples]
            self.samples = self.samples[available_samples:]
            gc.collect()
            return export_samples
        return None

    def sample_callback(self, *args, **kwargs):
        if len(self.samples) < MAXIMUM_BUFFER_LENGTH:
            self.samples.append(self.adc.read())

    def send_data(self, value, retry):
        try:
            request_data = str(value)
            r = urequests.post(DATA_ENDPOINT, data=request_data)
        except Exception as e:
            print("Error:", e)
            if retry:
                self.reconnect_to_wifi()
                self.send_data(value, False)

    def loop(self):
        while True:
            self.send_data(self.collect_available_samples(), True)
            gc.collect()
            time.sleep(self.sleep)


def init():
    AdcNetworkSender(READ_LOOP_SLEEP).loop()
