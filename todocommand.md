# simulation :

litex_sim --integrated-rom-size=0x10000 --cpu-type=cv32e41p --cpu-variant=HIDS_SYNTH --rom-init=demo.bin --trace --trace-start=30000000000
litex_sim --integrated-rom-size=0x10000 --cpu-type=cv32e41p --cpu-variant=HIDS_SYNTH --rom-init=demo.bin --trace --trace-start=200000 --trace-end=230000
litex_sim --integrated-rom-size=0x10000 --cpu-type=cv32e41p --cpu-variant=HIDS_SYNTH --no-compile-gateware

# implementation :
./litex-boards/litex_boards/targets/digilent_arty_lora_tracer.py --variant=a7-100 --cpu-type=cv32e41p --cpu-variant=HIDS_SYNTH --csr-csv=csr.csv --build --load
./litex-boards/litex_boards/targets/digilent_arty_lora_tracer.py --variant=a7-100 --cpu-type=cv32e41p --cpu-variant=HIDS_SYNTH --sys-clk-freq=50e6 --csr-csv=csr.csv --load
 litex_term /dev/ttyUSB2 --kernel=demo.bin > Dataset_SO.csv

# litescope :
litex_server --uart --uart-port=/dev/ttyUSB0
litescope_cli --v basecoc_HPMcycles 18838


# simulation 
litex_sim --cpu-type=cv32e41p --cpu-variant=HIDS_SYNTH --with-sdram --integrated-sram-size=0x1000000 --no-compile-gateware
./demo.py --build-path /home/med/Desktop/workflow/build/sim
litex_sim --cpu-type=cv32e41p --cpu-variant=HIDS_SYNTH --with-sdram --integrated-sram-size=0x1000000 --sdram-init=demo.bin


# launch several simulation :


source ../../../../../../env/litex_tracer/bin/activate


#litescope +simulation :


litex_sim --cpu-type=cv32e41p --cpu-variant=HIDS_SYNTH --with-sdram --integrated-sram-size=0x1000000 --with-etherbone --with-analyzer --no-compile-gateware

make clean && make 


litex_sim --cpu-type=cv32e41p --cpu-variant=HIDS_SYNTH --with-sdram --integrated-sram-size=0x1000000 --with-etherbone --with-analyzer --sdram-init=demo.bin


# traffic generator : 

./traffic_generator.py -Pnr '2000' -PMaxSize '10' -Bo='0' -BoMaxSize='0'
