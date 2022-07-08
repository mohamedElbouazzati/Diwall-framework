
.PHONY: pyenv

gateware:
	python3 -m boards.targets.poto_digilent_basys3 --integrated-main-ram-size=0x20000 --build

load_bitstream:
	python3 -m boards.targets.poto_digilent_basys3 --load                           

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
	rm -rf build
