# Build project

echo "START"
DEFAULT_DIR=$(pwd)

#install dépendance
sudo apt install --no-install-recommends git cmake ninja-build gperf \
  ccache dfu-util device-tree-compiler wget \
  python3-dev python3-pip python3-setuptools python3-tk python3-wheel xz-utils file \
  make gcc gcc-multilib g++-multilib libsdl2-dev libmagic1

sudo apt install python3-venv

sudo pip install --upgrade pip
#--Clone the repository and update all submodules:
git clone https://github.com/litex-hub/zephyr-on-litex-vexriscv.git
cd zephyr-on-litex-vexriscv
git submodule update --init --recursive
#--Next, get all required packages and run the install script:
sudo apt-get install build-essential bzip2 python3 python3-dev python3-pip
./install.sh

#Generate the bitstream:

source ./init
WHOAMI=$(whoami)
export INSTALL_DIR="/home/$WHOAMI/opt/f4pga"
FPGA_FAM="xc7"
export PATH="$INSTALL_DIR/$FPGA_FAM/install/bin:$PATH";
source "$INSTALL_DIR/$FPGA_FAM/conda/etc/profile.d/conda.sh"
conda activate $FPGA_FAM
./make.py --board=arty --variant="a7-100" --build --toolchain=vivado

#Download zephyr and west

#--Install west,
pip3 install west
#echo 'export PATH=~/.local/bin:"$PATH"' >> ~/.bashrc
#source ~/.bashrc
#--Get the Zephyr source code:
west init ./zephyr-on-litex-vexriscv/zephyrproject
cd ./zephyr-on-litex-vexriscv/zephyrproject
west update
#--Export a Zephyr CMake package. This allows CMake to automatically load boilerplate code required for building Zephyr applications.
west zephyr-export
#--Zephyr’s scripts/requirements.txt file declares additional Python dependencies. Install them with pip3.
pip3 install -r ../zephyrproject/zephyr/scripts/requirements.txt

#Install Zephyr SDK
#--Download and verify the latest Zephyr SDK bundle:
cd ~
wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.14.2/zephyr-sdk-0.14.2_linux-x86_64.tar.gz
wget -O - https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.14.2/sha256.sum | shasum --check --ignore-missing

#--Extract the Zephyr SDK bundle archive:
tar xvf zephyr-sdk-0.14.2_linux-x86_64.tar.gz

#--Run the Zephyr SDK bundle setup script:
cd zephyr-sdk-0.14.2
./setup.sh

#--Install udev rules, which allow you to flash most Zephyr boards as a regular user:
sudo cp ~/zephyr-sdk-0.14.2/sysroots/x86_64-pokysdk-linux/usr/share/openocd/contrib/60-openocd.rules /etc/udev/rules.d
sudo udevadm control --reload

cd $DEFAULT_DIR
echo "FINISH"