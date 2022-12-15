

# Prerequisites

```
$ sudo apt -y install gcc-arm-none-eabi
$ sudo apt install libusb-1.0 stlink-tools
$ cd LoRaMac-node
$ git submodule update --init
```


# Getting started

Build the firmware

```
mkdir build
BOARD=B-L072Z-LRWAN1
cmake
-DCMAKE_TOOLCHAIN_FILE="../LoRaMac-node/cmake/toolchain-arm-none-eabi.cmake" -DAPPLICATION="dummyMac" -DBOARD="$BOARD" ..
make
```

Flash the firmware

```
st-info --probe
st-flash write dummyMac/dummyMac.bin 0x8000000
```

Check the output of the application

```
minicom -D /dev/ttyACM0 -b 921600
```


The dummyMac communication use the following parameters:

* Channel: 868.3 MHz
* SF12
* SYNC network public at true: Ox34

# Test ping-pong default app

$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_TOOLCHAIN_FILE="../cmake/toolchain-arm-none-eabi.cmake" \
        -DAPPLICATION="ping-pong" \
        -DMODULATION:"LORA" \
        -DREGION_EU868="ON" \
        -DREGION_US915="OFF" \
        -DREGION_CN779="OFF" \
        -DREGION_EU433="OFF" \
        -DREGION_AU915="OFF" \
        -DREGION_AS923="OFF" \
        -DREGION_CN470="OFF" \
        -DREGION_KR920="OFF" \
        -DREGION_IN865="OFF" \
        -DREGION_RU864="OFF" \
        -DBOARD="B-L072Z-LRWAN1" \
$ make
$ st-flash write dummyMac/dummyMac.bin 0x8000000
$ 


