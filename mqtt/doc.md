# MQTT

## docs
https://antima.it/tutorial-utilizzare-mqtt-con-python-la-classe-client-parte-1/

https://antima.it/costruisci-la-tua-rete-dotomica-con-esp8266-e-raspberry-pi-protocolli-applicativi-e-rete-locale/

https://antima.it/tutorial-utilizzare-mqtt-con-python-parte-2-callback-e-loop/

## Mosquito
brew install mosquitto

mosquitto has been installed with a default configuration file.
You can make changes to the configuration by editing:
    /usr/local/etc/mosquitto/mosquitto.conf

To have launchd start mosquitto now and restart at login:
  brew services start mosquitto
Or, if you don't want/need a background service you can just run:
  mosquitto -c /usr/local/etc/mosquitto/mosquitto.conf

  /usr/local/Cellar/mosquitto/2.0.10_1/sbin/mosquitto  