#!/usr/bin/env python3

#
# This file is part of LiteX-Boards.
#
# Copyright (c) 2015-2019 Florent Kermarrec <florent@enjoy-digital.fr>
# Copyright (c) 2020 Antmicro <www.antmicro.com>
# Copyright (c) 2022 Victor Suarez Rovere <suarezvictor@gmail.com>
# SPDX-License-Identifier: BSD-2-Clause

# Note: For now, with --toolchain=yosys+nextpnr, DDR3 should be disabled and sys_clk_freq lowered, ex:
# python3 -m litex_boards.targets.digilent_arty.py --sys-clk-freq=50e6 --integrated-main-ram-size=8192 --toolchain=yosys+nextpnr --build


from migen.genlib.cdc import MultiReg

from litex.soc.interconnect.csr import *

from litex.soc.interconnect.csr_eventmanager import *


from migen import *

from boards.platforms import digilent_arty_lora
from litex.build.xilinx.vivado import vivado_build_args, vivado_build_argdict

from litex.soc.cores.clock import *
from litex.soc.integration.soc import SoCRegion
from litex.soc.integration.soc_core import *
from litex.soc.integration.builder import *
from litex.soc.cores.led import LedChaser
from litex.soc.cores.gpio import GPIOTristate
from litex.soc.cores.gpio import *
from litedram.modules import MT41K128M16
from litedram.phy import s7ddrphy


# CRG ----------------------------------------------------------------------------------------------

class _CRG(Module):
    def __init__(self, platform, sys_clk_freq, with_dram=True, with_rst=True):
        self.rst = Signal()
        self.clock_domains.cd_sys       = ClockDomain()
        self.clock_domains.cd_eth       = ClockDomain()
        if with_dram:
            self.clock_domains.cd_sys4x     = ClockDomain(reset_less=True)
            self.clock_domains.cd_sys4x_dqs = ClockDomain(reset_less=True)
            self.clock_domains.cd_idelay    = ClockDomain()


        # # #

        # Clk/Rst.
        clk100 = platform.request("clk100")
        rst    = ~platform.request("cpu_reset") if with_rst else 0

        # PLL.
        self.submodules.pll = pll = S7PLL(speedgrade=-1)
        self.comb += pll.reset.eq(rst | self.rst)
        pll.register_clkin(clk100, 100e6)
        pll.create_clkout(self.cd_sys, sys_clk_freq)
        pll.create_clkout(self.cd_eth, 25e6)
        self.comb += platform.request("eth_ref_clk").eq(self.cd_eth.clk)
        platform.add_false_path_constraints(self.cd_sys.clk, pll.clkin) # Ignore sys_clk to pll.clkin path created by SoC's rst.
        if with_dram:
            pll.create_clkout(self.cd_sys4x,     4*sys_clk_freq)
            pll.create_clkout(self.cd_sys4x_dqs, 4*sys_clk_freq, phase=90)
            pll.create_clkout(self.cd_idelay,    200e6)

        # IdelayCtrl.
        if with_dram:
            self.submodules.idelayctrl = S7IDELAYCTRL(self.cd_idelay)

# BaseSoC ------------------------------------------------------------------------------------------

class BaseSoC(SoCCore):
    def __init__(self, variant="a7-35", toolchain="vivado", sys_clk_freq=int(50e6), with_lora=True,
                 with_ethernet=False, with_etherbone=False, eth_ip="192.168.1.50",
                 eth_dynamic_ip=False, with_led_chaser=True, with_jtagbone=True,
                 with_spi_flash=False, with_pmod_gpio=False, **kwargs):
        platform = digilent_arty_lora.Platform(variant=variant, toolchain=toolchain)

        # SoCCore ----------------------------------------------------------------------------------
        SoCCore.__init__(self, platform, sys_clk_freq,
            ident = "LiteX SoC on Arty A7",
            **kwargs)

        # CRG --------------------------------------------------------------------------------------
        self.submodules.crg = _CRG(platform, sys_clk_freq, with_dram=not self.integrated_main_ram_size)

        # DDR3 SDRAM -------------------------------------------------------------------------------
        if not self.integrated_main_ram_size:
            self.submodules.ddrphy = s7ddrphy.A7DDRPHY(platform.request("ddram"),
                memtype        = "DDR3",
                nphases        = 4,
                sys_clk_freq   = sys_clk_freq)
            self.add_sdram("sdram",
                phy           = self.ddrphy,
                module        = MT41K128M16(sys_clk_freq, "1:4"),
                l2_cache_size = kwargs.get("l2_size", 8192)
            )

        # Lora / SPI DIO ---------------------------------------------------------------------
        if with_lora:
            from litex.soc.cores.spi import SPIMaster         
            self.submodules.loraspi = SPIMaster(pads=platform.request("lora_spi"), data_width=8, sys_clk_freq=sys_clk_freq, spi_clk_freq=int(100e3), with_csr=True, mode="raw")
            #platform.add_extension(digilent_arty_poto.dio_methode("dio"))
            self.submodules.dio0 = GPIOIn(platform.request("dio0"), with_irq = self.irq.enabled )
            self.add_interrupt("dio0")
            self.add_csr("dio0")
            self.submodules.dio1 = GPIOIn(platform.request("dio1"), with_irq = self.irq.enabled )
            self.add_interrupt("dio1")
            self.add_csr("dio1")
            self.submodules.dio2 = GPIOIn(platform.request("dio2"), with_irq = self.irq.enabled )
            self.add_interrupt("dio2")
            self.add_csr("dio2")
            self.submodules.dio3 = GPIOIn(platform.request("dio3"), with_irq = self.irq.enabled )
            self.add_interrupt("dio3")
            self.add_csr("dio3")            
            #self.add_gpio(name="dio0",pads=platform.request("dio0"),with_irq=True)
            #self.add_gpio(name="dio1",pads=platform.request("dio1"),with_irq=True)
            #self.add_gpio(name="dio2",pads=platform.request("dio2"),with_irq=True)
            #self.add_gpio(name="dio3",pads=platform.request("dio3"),with_irq=True)
           # self.add_gpio(name="rst",pads=platform.request("rst"))
            # self.add_gpio(name="rst",pads=platform.request("rst"),with_irq=False)
            self.submodules.rst = GPIOOut(platform.request("rst"))
            self.add_csr("rst")

            #Adding Timer for Tests
            from litex.soc.cores.timer import Timer
            self.submodules.timer1 = Timer()
            self.add_interrupt("timer1")
            self.add_csr("timer1")

        # Jtagbone ---------------------------------------------------------------------------------
        if with_jtagbone:
            self.add_jtagbone()

        # SPI Flash --------------------------------------------------------------------------------
        if with_spi_flash:
            from litespi.modules import S25FL128L
            from litespi.opcodes import SpiNorFlashOpCodes as Codes
            self.add_spi_flash(mode="4x", module=S25FL128L(Codes.READ_1_1_4), rate="1:2", with_master=True)

        # Leds -------------------------------------------------------------------------------------
        if with_led_chaser:
            self.submodules.leds = LedChaser(
                pads         = platform.request_all("user_led"),
                sys_clk_freq = sys_clk_freq)

        # GPIOs ------------------------------------------------------------------------------------
        if with_pmod_gpio:
            platform.add_extension(digilent_arty_lora.raw_pmod_io("pmoda"))
            self.submodules.gpio = GPIOTristate(platform.request("pmoda"))

# Build --------------------------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(description="LiteX SoC on Arty A7")
    target_group = parser.add_argument_group(title="Target options")
    target_group.add_argument("--toolchain",           default="vivado",                 help="FPGA toolchain (vivado, symbiflow or yosys+nextpnr).")
    target_group.add_argument("--build",               action="store_true",              help="Build bitstream.")
    target_group.add_argument("--load",                action="store_true",              help="Load bitstream.")
    target_group.add_argument("--flash",               action="store_true",              help="Flash bitstream.")
    target_group.add_argument("--variant",             default="a7-35",                  help="Board variant (a7-35 or a7-100).")
    target_group.add_argument("--sys-clk-freq",        default=100e6,                    help="System clock frequency.")
    ethopts = target_group.add_mutually_exclusive_group()
    ethopts.add_argument("--with-ethernet",      action="store_true",              help="Enable Ethernet support.")
    ethopts.add_argument("--with-etherbone",     action="store_true",              help="Enable Etherbone support.")
    target_group.add_argument("--eth-ip",              default="192.168.1.50", type=str, help="Ethernet/Etherbone IP address.")
    target_group.add_argument("--eth-dynamic-ip",      action="store_true",              help="Enable dynamic Ethernet IP addresses setting.")
    sdopts = target_group.add_mutually_exclusive_group()
    sdopts.add_argument("--with-spi-sdcard",     action="store_true",              help="Enable SPI-mode SDCard support.")
    sdopts.add_argument("--with-sdcard",         action="store_true",              help="Enable SDCard support.")
    target_group.add_argument("--sdcard-adapter",      type=str,                         help="SDCard PMOD adapter (digilent or numato).")
    target_group.add_argument("--with-jtagbone",       action="store_true",              help="Enable JTAGbone support.")
    target_group.add_argument("--with-spi-flash",      action="store_true",              help="Enable SPI Flash (MMAPed).")
    target_group.add_argument("--with-pmod-gpio",      action="store_true",              help="Enable GPIOs through PMOD.") # FIXME: Temporary test.
    builder_args(parser)
    soc_core_args(parser)
    vivado_build_args(parser)
    args = parser.parse_args()

    assert not (args.with_etherbone and args.eth_dynamic_ip)

    soc = BaseSoC(
        variant        = args.variant,
        toolchain      = args.toolchain,
        sys_clk_freq   = int(float(args.sys_clk_freq)),
        with_ethernet  = args.with_ethernet,
        with_etherbone = args.with_etherbone,
        eth_ip         = args.eth_ip,
        eth_dynamic_ip = args.eth_dynamic_ip,
        with_jtagbone  = args.with_jtagbone,
        with_spi_flash = args.with_spi_flash,
        with_pmod_gpio = args.with_pmod_gpio,
        **soc_core_argdict(args)
    )
    if args.sdcard_adapter == "numato":
        soc.platform.add_extension(digilent_arty_lora._numato_sdcard_pmod_io)
    else:
        soc.platform.add_extension(digilent_arty_lora._sdcard_pmod_io)
    if args.with_spi_sdcard:
        soc.add_spi_sdcard()
    if args.with_sdcard:
        soc.add_sdcard()

    builder = Builder(soc, **builder_argdict(args))
    builder_kwargs = vivado_build_argdict(args) if args.toolchain == "vivado" else {}
    builder.build(**builder_kwargs, run=args.build)

    if args.load:
        prog = soc.platform.create_programmer()
        #prog.load_bitstream(builder.get_bitstream_filename(mode="sram"))
        prog.load_bitstream(os.path.join(builder.gateware_dir, soc.build_name + ".bit"))
    if args.flash:
        prog = soc.platform.create_programmer()
        prog.flash(0, builder.get_bitstream_filename(mode="flash"))

if __name__ == "__main__":
    main()
