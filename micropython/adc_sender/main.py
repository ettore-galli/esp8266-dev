# ADC Network sender main

import network
import time
import socket

from machine import Timer, ADC
import urequests

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
READ_LOOP_SLEEP = 0.1


def connect_to_wifi():
    sta_if = network.WLAN(network.STA_IF)
    if not sta_if.isconnected():
        print("connecting to network...")
        sta_if.active(True)
        sta_if.connect(WIFI_ESSID, WIFI_PASSWORD)
        while not sta_if.isconnected():
            print("connecting...")
            time.sleep(CONNECTION_MONITOR_DELAY)
        print("network config:", sta_if.ifconfig())


def value_http_request(path: str, value: str) -> bytes:
    body = '{"adc_value": "%s"}' % value
    content_length = len(body)
    request = """POST /%s HTTP/1.1
Host: localhost:8080
Content-Type: application/json
Content-Length: %s

%s
""" % (
        path,
        content_length,
        body,
    )
    return bytes(request, "utf8")


def http_request(host, path, port, reconnect_retry, value):
    try:
        print("Init request attempt.")
        print("Get address info...")
        addr = socket.getaddrinfo(host, port)[0][-1]
        print("Instantiate Socket...")
        s = socket.socket()
        print("Connect to socket...")
        s.connect(addr)
        print("Prepare request...")
        request = value_http_request(path, value)
        print(request.decode("utf-8"))
        print("Send actual request...")
        s.send(request)
        data = s.recv(100)
        if data:
            print(str(data, "utf8"), end="")
        s.close()
    except Exception as e:
        print("Error", e)
        if reconnect_retry:
            print("retry...")
            work_with_wifi()
            http_request(host, path, port, False, value)


def send_data(value):
    try:
        request_data = str(value)
        r = urequests.post(DATA_ENDPOINT, data=request_data)
    except Exception as e:
        print("Error:", e)


class Worker(object):
    def __init__(self, sleep: float) -> None:
        self.sleep = sleep
        self.adc = ADC(0)

        self.timer = Timer(-1)
        self.timer.init(period=10, mode=Timer.PERIODIC, callback=self.sample_callback)

        self.samples = []

        connect_to_wifi()  # TBD: Object attributes?

    def collect_available_samples(self):
        available_samples = len(self.samples)
        if available_samples > 0:
            export_samples = self.samples[:available_samples]
            self.samples = self.samples[available_samples:]
            return export_samples
        return None

    def sample_callback(self, arg):
        self.samples.append(self.adc.read())

    def loop(self):
        while True:
            send_data(self.collect_available_samples())
            time.sleep(self.sleep)


def init():
    Worker(READ_LOOP_SLEEP).loop()
