# ADC Network sender main

import network
import time
import socket

from machine import ADC

try:
    from network_definitions import WIFI_ESSID, WIFI_PASSWORD, SERVER_HOST, SERVER_PORT, SERVER_PATH
except:
    print("Create  a file named network_definitions.py containing the following mandatory variables:")
    print("WIFI_ESSID       = Wifi network ESSID")
    print("WIFI_PASSWORD    = Wifi network PASSWORD")
    print("SERVER_HOST      = Server IP or hostname")
    print("SERVER_PORT      = Server port")
    print("SERVER_PATH      = Path for the request")


CONNECTION_MONITOR_DELAY = 0.5
READ_LOOP_SLEEP = 1

def work_with_wifi():
    sta_if = network.WLAN(network.STA_IF)
    if not sta_if.isconnected():
        print("connecting to network...")
        sta_if.active(True)
        sta_if.connect(WIFI_ESSID, WIFI_PASSWORD)
        while not sta_if.isconnected():
            print("connecting...")
            time.sleep(CONNECTION_MONITOR_DELAY)
        print("network config:", sta_if.ifconfig())


def http_request(host, path, port, reconnect_retry):
    try:
        addr = socket.getaddrinfo(host, port)[0][-1]
        s = socket.socket()
        s.connect(addr)
        s.send(bytes("GET /%s HTTP/1.0\r\nHost: %s\r\n\r\n" % (path, host), "utf8"))
        data = s.recv(100)
        if data:
            print(str(data, "utf8"), end="")
        s.close()
    except Exception as e:
        print("Error", e)
        if reconnect_retry:
            print("retry...")
            work_with_wifi()
            http_request(host, path, port, False)


class Worker(object):
    def __init__(self, sleep: float) -> None:
        self.sleep = sleep
        self.adc = ADC(0)
        work_with_wifi()  # TBD: Object attributes?

    def loop(self):
        while True:
            value = self.adc.read()
            http_request(SERVER_HOST, str(value), SERVER_PORT, True)
            time.sleep(self.sleep)

def init():
    Worker(READ_LOOP_SLEEP).loop()
