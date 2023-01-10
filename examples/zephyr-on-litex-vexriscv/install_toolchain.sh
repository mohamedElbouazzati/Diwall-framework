
pip3 install meson ninja
mkdir -p toolchain && cd toolchain
../litex_lib/litex_setup.py --init first
../litex_lib/litex_setup.py --update
../litex_lib/litex_setup.py --gcc=riscv