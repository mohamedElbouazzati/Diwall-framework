.PHONY: pyenv

THIS_DIR := $(dir $(abspath $(firstword $(MAKEFILE_LIST))))
#Juste changer la target
TARGET:=digilent_arty_cv32e41p

gateware:
	python3 -m boards.targets.$(TARGET) --variant="a7-100" --build

load_bitstream:
	python3 -m boards.targets.$(TARGET) --variant="a7-100" --load

docs:                         
	python3 -m boards.targets.$(TARGET) --doc

open_docs:
	microsoft-edge $(THIS_DIR)build/$(TARGET)/doc/_build/html/index.html

demo:
	make -C examples/demo

load_demo:
	litex_term /dev/ttyUSB1 --kernel examples/demo/demo.bin                              

python-deps: pyenv  # installs python dependencies inside virtual environment
	pip install -r requirements.txt

pyenv:  # creates virtual environment if it does not exist
	python3 -m venv venv/pyvenv
	source venv/pyenv/bin/activate

clean:
	cd $(THIS_DIR)examples/demo && $(MAKE) clean 
	cd $(THIS_DIR)examples/demo-lora && $(MAKE) clean
	rm -rf build
