source ./env.sh

esptool.py --port $PORT erase_flash
esptool.py --port $PORT --baud 460800 write_flash --flash_size=detect 0 $FIRMWARE

# screen /dev/cu.usbserial-14230 115200