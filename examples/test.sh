
cd ./zephyr-on-litex-vexriscv/zephyrproject/zephyr

west build -b litex_vexriscv samples/hello_world

#si ça ne fonctionne pas suivre exemple 
#export ZEPHYR_TOOLCHAIN_VARIANT=cross-compile
#export ZEPHYR_SDK_INSTALL_DIR="~/zephyr-sdk-0.15.0/" #"~/zephyr-sdk-0.14.2"
#export CROSS_COMPILE="/home/grannhom/riscv64-unknown-elf-gcc-8.3.0-2019.08.0-x86_64-linux-ubuntu14/bin/riscv64-unknown-elf-gcc"
#source zephyr/zephyr-env.sh

#./configure --prefix=/home/riscv --with-arch=rv32i --with-abi=ilp32
#make linux