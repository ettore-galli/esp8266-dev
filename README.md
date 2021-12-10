# esp8266-dev

## ONE TIME ENVIRONMENT SETUP
### Docs
https://www.espressif.com/en/products/socs/esp8266
https://www.espressif.com/sites/default/files/documentation/2a-esp8266-sdk_getting_started_guide_en.pdf
https://github.com/espressif/ESP8266_RTOS_SDK
https://github.com/espressif/ESP8266_RTOS_SDK/issues/1028

### 1. Get toolchain (see repo readme)
Expand directory into ~/esp

### 2. Get and install RTOS SDK

```bash
cd ~/esp
git clone https://github.com/espressif/ESP8266_RTOS_SDK.git
```

### 3. Define proper paths (devenv.sh)

```bash
# Toolchain
export ESP_BASE=~/esp
export ESP_BIN=$ESP_BASE/xtensa-lx106-elf/bin
export PATH=$PATH:$ESP_BIN

# Needed by the toolchain
export IDF_PATH=$ESP_BASE/ESP8266_RTOS_SDK
```

### 4. Virtualenv and prerequisites
```bash
python3.8 -m venv venv
pip install -r $ESP_BASE/ESP8266_RTOS_SDK/requirements.txt
```

### 5. First project by copy and USB setup
```bash
cp -R ~/esp/ESP8266_RTOS_SDK/examples/get-started/hello_world .
. ./devenv.sh 
cd hello_world/
make menuconfig

# In the menu, navigate to Serial flasher config > Default serial port to configure the serial port, where # project will be loaded to. Confirm selection by pressing enter, save configuration by selecting < Save > # and then exit application by selecting < Exit >.

# USB: 
ls /dev | grep cu
# e.g. cu.usbserial-14130

```

## ACTUAL USAGE


## Older docs

    ```txt
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
    ```
