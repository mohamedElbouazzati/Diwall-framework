# README

Repository about a SoC with wireless capabilities build with LiTex for FPGA.

```
git clone https://ptanguy@git.renater.fr/authscm/ptanguy/git/stage-terrine/stage-terrine.git
git submodule init
git submodule update
```

Setup env:

```
source venv/pyenv/bin/activate
pip3 install -r requirements.txt
cd third_party/litex
./litex_setup.py --gcc=riscv
sudo apt-get install openocd
```

Setup vars env:
```
export LITEX_ENV_VIVADO="INSTALL-PATH/Xilinx/Vivado/2020.2"                       
export PATH="INSTALL-PATH/riscv64-unknown-elf-gcc-8.3.0-2019.08.0-x86_64-linux-ubuntu14/bin:$PATH"
```

Generate bitstream:

```
python3 -m boards.targets.poto_digilent_basys3 --integrated-main-ram-size=0x20000 --build 
```

Load bitstream:
```
python3 -m boards.targets.poto_digilent_basys3 --load 
```

Load application via serial
```
litex_term /dev/ttyUSB1 --kernel examples/demo.bin
```

To start the loading process press button BTNC on basys3


## Read the doc

In the following we suppose that you have the Pip software for Python3.
If not on a debian you can install it such as:

```
sudo apt install python3-pip
```

Install dependancies:

```
pip install -r doc/requirements.txt
```

It will install Sphinx which is a tool that makes it easy to create
documentation.

To generate and read the doc:

```
make -C doc html
firefox doc/build/html/index.html
```

## Issues

* [] see issue with toolchain riscv64-unknown-elf-gcc-10.1.0-2020.08.2-x86_64-linux-ubuntu14
    * when demo is loaded it is stuck to --- liftoff ---
    
## ROADMAP
 
* [] add interrupt support to manage RFM95w interrupt


