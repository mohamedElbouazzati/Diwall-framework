
.PHONY: pyenv

SHELL := /bin/bash 

#Processor and firmware : 
CPU_TYPE?=cv32e41p 
VARIANT?=HIDS_SYNTH
APPLICATION?=demo

gateware: 
	python3 -m boards.targets.digilent_arty_lora --variant=a7-100 --cpu-type=$(CPU_TYPE) --cpu-variant=$(VARIANT) --integrated-sram-size=16384  --build

load_bitstream:
	python3 -m boards.targets.digilent_arty_lora --variant=a7-100 --cpu-type=$(CPU_TYPE) --cpu-variant=$(VARIANT) --integrated-sram-size=16384 --load                           

compile_app:
	make -C examples/$(APPLICATION)

load_app:
	litex_term /dev/ttyUSB1 --kernel examples/$(APPLICATION)/demo.bin                              

python-deps: activate-pyenv  # installs python dependencies inside virtual environment
	pip install -r requirements.txt

activate-pyenv:  
	source venv/pyenv/bin/activate

pyenv:  
	python3 -m venv venv/pyenv

clean:
	rm -rf examples/*/obj
	rm -rf build
	
	
	
