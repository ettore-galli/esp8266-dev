#
# Set development environment variables
#

# Toolchain
export ESP_BASE=~/esp
export ESP_BIN=$ESP_BASE/xtensa-lx106-elf/bin
export PATH=$PATH:$ESP_BIN

# Needed by the toolchain
export IDF_PATH=$ESP_BASE/ESP8266_RTOS_SDK

clear

. ./venv/bin/activate

echo ------------------------------------------------------
echo ESP8266 DEVELOPMENT ENVIRONMENT
echo ------------------------------------------------------
echo
echo Everything besides the virtualenv is installed in ESP_BASE:
echo $ESP_BASE
echo
echo ESP_BIN=$ESP_BIN
echo IDF_PATH=$IDF_PATH
echo 