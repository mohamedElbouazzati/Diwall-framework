
.PHONY: pyenv
#Processor and firmware : 
CPU_TYPE?=cv32e41p 
VARIANT?=standard
APPLICATION?=demo

gateware:
	python3 -m boards.targets.digilent_arty_cv32e41p --variant=a7-100 --cpu-type=$(CPU_TYPE) --cpu-variant=$(VARIANT) --build
load_bitstream:
	python3 -m boards.targets.digilent_arty_cv32e41p --variant=a7-100 --cpu-type=$(CPU_TYPE) --cpu-variant=$(VARIANT) --load                           
compile_app:
	make -C examples/$(APPLICATION)
load_app:
	litex_term /dev/ttyUSB1 --kernel examples/$(APPLICATION)/demo.bin                              

python-deps: pyenv  # installs python dependencies inside virtual environment
	pip install -r requirements.txt

pyenv:  # creates virtual environment if it does not exist
	python3 -m venv venv/pyvenv
	source venv/pyenv/bin/activate

clean:
	make -C examples/$(APPLICATION) clean
	rm -rf examples/$(APPLICATION)/obj
	rm -rf build
	
	
	
