# esp8266-dev

## ONE TIME ENVIRONMENT SETUP
### Docs
https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/macos-setup.html#install-prerequisites
https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/index.html#get-started-get-esp-idf

### Prerequisites
sudo pip install pyserial

### Toolchain
Download: 
https://dl.espressif.com/dl/xtensa-lx106-elf-gcc8_4_0-esp-2020r3-macos.tar.gz

mkdir -p ~/esp
cd ~/esp

Expand xtensa-lx106-elf-macos-1.22.0-100-ge567ec7-5.2.0.tar.gz
(should decompress into xtensa-lx106-elf)

### Get ESP8266_RTOS_SDK
cd ~/esp
git clone --recursive https://github.com/espressif/ESP8266_RTOS_SDK.git


### Install RTOS prerequisites in virtualenv
pip install -r $IDF_PATH/requirements.txt


## FIRST PROJECT PATH

Copy an example project
cp -r $IDF_PATH/examples/get-started/hello_world .

Connect
cd hello_world
make menuconfig

make flash

