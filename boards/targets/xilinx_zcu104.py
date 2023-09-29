#!/usr/bin/env python3

#
# This file is part of LiteX-Boards.
#
# Copyright (c) 2020 Antmicro <www.antmicro.com>
# Copyright (c) 2019 David Shah <dave@ds0.me>
# SPDX-License-Identifier: BSD-2-Clause
from migen.genlib.cdc import MultiReg

from litex.soc.interconnect.csr import *

from litex.soc.interconnect.csr_eventmanager import *

from migen import *
from migen.genlib.resetsync import AsyncResetSynchronizer

from litex.gen import *

from litex_boards.platforms import xilinx_zcu104

from litex.soc.integration.soc import SoCRegion
from litex.build.xilinx.vivado import vivado_build_args, vivado_build_argdict

from litex.soc.cores.clock import *
from litex.soc.integration.soc import SoCRegion
from litex.soc.integration.soc_core import *
from litex.soc.integration.builder import *
from litex.soc.cores.led import LedChaser
from litex.soc.cores.gpio import GPIOIn, GPIOTristate
from litex.soc.cores.gpio import *
from litex.soc.cores.bitbang import I2CMaster

from litedram.modules import MTA4ATF51264HZ
from litedram.phy import usddrphy

# CRG ----------------------------------------------------------------------------------------------

class _CRG(LiteXModule):
    def __init__(self, platform, sys_clk_freq, with_rst=True):
        self.rst       = Signal()
        self.cd_sys    = ClockDomain()
        self.cd_sys4x  = ClockDomain()
        self.cd_pll4x  = ClockDomain()
        self.cd_idelay = ClockDomain()

        # # #
        # Rst
        rst    = platform.request("cpu_reset") if with_rst else 0

        self.pll = pll = USMMCM(speedgrade=-2)
        self.comb += pll.reset.eq(rst | self.rst)
        pll.register_clkin(platform.request("clk125"), 125e6)
        pll.create_clkout(self.cd_pll4x, sys_clk_freq*4, buf=None, with_reset=False)
        pll.create_clkout(self.cd_idelay, 500e6)
        platform.add_false_path_constraints(self.cd_sys.clk, pll.clkin) # Ignore sys_clk to pll.clkin path created by SoC's rst.

        self.specials += [
            Instance("BUFGCE_DIV",
                p_BUFGCE_DIVIDE=4,
                i_CE=1, i_I=self.cd_pll4x.clk, o_O=self.cd_sys.clk),
            Instance("BUFGCE",
                i_CE=1, i_I=self.cd_pll4x.clk, o_O=self.cd_sys4x.clk),
        ]

        self.idelayctrl = USIDELAYCTRL(cd_ref=self.cd_idelay, cd_sys=self.cd_sys)

# BaseSoC ------------------------------------------------------------------------------------------

class BaseSoC(SoCCore):
    def __init__(self, sys_clk_freq=125e6, with_led_chaser=True, with_lora=True, **kwargs):
        platform = xilinx_zcu104.Platform()

        # CRG --------------------------------------------------------------------------------------
        self.crg = _CRG(platform, sys_clk_freq)

        # SoCCore ----------------------------------------------------------------------------------
        SoCCore.__init__(self, platform, sys_clk_freq, ident="LiteX SoC on ZCU104", **kwargs)

	# Lora / SPI DIO ---------------------------------------------------------------------
        if with_lora:
            from litex.soc.cores.spi import SPIMaster         
            self.submodules.loraspi = SPIMaster(pads=platform.request("lora_spi"), data_width=8, sys_clk_freq=sys_clk_freq, spi_clk_freq=int(100e3), with_csr=True, mode="raw")
            #platform.add_extension(digilent_arty_poto.dio_methode("dio"))
            self.submodules.dio0 = GPIOIn(platform.request("dio0"), with_irq = self.irq.enabled )
            self.irq.add("dio0")
            self.add_csr("dio0")
            self.submodules.dio1 = GPIOIn(platform.request("dio1"), with_irq = self.irq.enabled )
            self.irq.add("dio1")
            self.add_csr("dio1")
            self.submodules.dio2 = GPIOIn(platform.request("dio2"), with_irq = self.irq.enabled )
            self.irq.add("dio2")
            self.add_csr("dio2")
            self.submodules.dio3 = GPIOIn(platform.request("dio3"), with_irq = self.irq.enabled )
            self.irq.add("dio3")
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
            self.irq.add("timer1")
            self.add_csr("timer1")

        # DDR4 SDRAM -------------------------------------------------------------------------------
        if not self.integrated_main_ram_size:
            self.ddrphy = usddrphy.USPDDRPHY(platform.request("ddram"),
                memtype          = "DDR4",
                sys_clk_freq     = sys_clk_freq,
                iodelay_clk_freq = 500e6)
            self.add_sdram("sdram",
                phy           = self.ddrphy,
                module        = MTA4ATF51264HZ(sys_clk_freq, "1:4"),
                size          = 0x40000000,
                l2_cache_size = kwargs.get("l2_size", 8192)
            )

        # Leds -------------------------------------------------------------------------------------
        if with_led_chaser:
            self.leds = LedChaser(
                pads         = platform.request_all("user_led"),
                sys_clk_freq = sys_clk_freq)

# Build --------------------------------------------------------------------------------------------

def main():
    from litex.build.parser import LiteXArgumentParser
    parser = LiteXArgumentParser(platform=xilinx_zcu104.Platform, description="LiteX SoC on ZCU104.")
    parser.add_target_argument("--sys-clk-freq", default=125e6, type=float, help="System clock frequency.")
    args = parser.parse_args()

    soc = BaseSoC(
        sys_clk_freq = args.sys_clk_freq,
        **parser.soc_argdict
    )
    builder = Builder(soc, **parser.builder_argdict)
    if args.build:
        builder.build(**parser.toolchain_argdict)

    if args.load:
        prog = soc.platform.create_programmer()
        prog.load_bitstream(builder.get_bitstream_filename(mode="sram"))

if __name__ == "__main__":
    main()
