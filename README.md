# README

Repository about a SoC with wireless capabilities build with LiTex for FPGA.

```
git clone https://ptanguy@git.renater.fr/authscm/ptanguy/git/stage-terrine/stage-terrine.git
git submodule init
git submodule update
```

Setup env:

```
make pyenv
source venv/pyenv/bin/activate
pip3 install -r requirements.txt
```

Fix picolibc missing data:
```
cd third_party/pythondata-software-picolibc
git submodule init
git submodule update
```

Install toolchain:

```
cd third_party/litex
./litex_setup.py --gcc=riscv
```

Install openocd:

```
sudo apt-get install openocd
```

Setup vars env:
```
export LITEX_ENV_VIVADO="INSTALL-PATH/Xilinx/Vivado/2020.2"                       
export PATH="INSTALL-PATH/riscv64-unknown-elf-gcc-8.3.0-2019.08.0-x86_64-linux-ubuntu14/bin:$PATH"
```

Generate bitstream:

```
make gateware CPU_TYPE=(cv32e41p/vexriscv) VARIANT=(standard/HIDS_SYNTH) 

```

Load bitstream:
```
make load_bitstream CPU_TYPE=(cv32e41p/vexriscv) VARIANT=(standard/HIDS_SYNTH)
```

Compile application:
```
make compile_app APPLICATION=(demo/demo-lora/demo-loramacnode)
```


Load application via serial

```
make load_app APPLICATION=(demo/demo-lora/demo-loramacnode)
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
* [] add support to manage Dragino LoRa shield v1.4 with Arty A7 board
* [] add submodule for LoRa-MAC node stack and fork it to  add support for LoRa
  Shield by following the steps details here https://github.com/Lora-net/LoRaMac-node/discussions/1101
  or by following the porting guide https://stackforce.github.io/LoRaMac-doc/LoRaMac-doc-v4.5.1/_p_o_r_t_i_n_g__g_u_i_d_e.html
## References : 
- Conference paper : [M. E. Bouazzati, R. Tessier, P. Tanguy and G. Gogniat, "A Lightweight Intrusion Detection System against IoT Memory Corruption Attacks," 2023 26th International Symposium on Design and Diagnostics of Electronic Circuits and Systems (DDECS), Tallinn, Estonia, 2023, pp. 118-123, doi: 10.1109/DDECS57882.2023.10139718.](https://ieeexplore.ieee.org/document/10139718)
- Journal paper : "under submission"

