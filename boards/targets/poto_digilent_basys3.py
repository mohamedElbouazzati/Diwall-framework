#!/usr/bin/env python3

#
# This file is part of LiteX-Boards.
#
# Copyright (c) 2020-2021 Xuanyu Hu <xuanyu.hu@whu.edu.cn>
# SPDX-License-Identifier: BSD-2-Clause

import imp
from migen import *

from litex_boards.platforms import board

from litex.soc.cores.clock import *
from litex.soc.integration.soc import SoCRegion  #import des i
from litex.soc.integration.soc_core import *
from litex.soc.integration.builder import *
from litex.soc.cores.video import VideoVGAPHY
from litex.soc.cores.led import LedChaser
from litex.soc.cores.gpio import *


# CRG ----------------------------------------------------------------------------------------------

class _CRG(Module):
    def __init__(self, platform, sys_clk_freq):
        self.rst = Signal()
        self.clock_domains.cd_sys       = ClockDomain()
        self.clock_domains.cd_vga       = ClockDomain()

        self.submodules.pll = pll = S7MMCM(speedgrade=-1)
        self.comb += pll.reset.eq(platform.request("user_btnc") | self.rst)                 #<---inf

        pll.register_clkin(platform.request("clk100"), 100e6)
        pll.create_clkout(self.cd_sys, sys_clk_freq)
        pll.create_clkout(self.cd_vga, 40e6)
        platform.add_false_path_constraints(self.cd_sys.clk, pll.clkin) # Ignore sys_clk to pll.clkin path created by SoC's rst.
        #platform.add_platform_command("set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets clk100_IBUF]").

# BaseSoC ------------------------------------------------------------------------------------------

class BaseSoC(SoCCore):
    def __init__(self, sys_clk_freq=int(75e6), with_custom_spi=True,with_led_chaser=True, with_video_terminal=False, **kwargs):
        platform = board.Platform()

        # SoCCore ----------------------------------_-----------------------------------------------
        SoCCore.__init__(self, platform, sys_clk_freq,
            ident = "LiteX SoC on board",
            **kwargs)

        # CRG --------------------------------------------------------------------------------------
        self.submodules.crg = _CRG(platform, sys_clk_freq)

        # Video ------------------------------------------------------------------------------------
        if with_video_terminal:
            self.submodules.videophy = VideoVGAPHY(platform.request("vga"), clock_domain="vga")
            if with_video_terminal:
                self.add_video_terminal(phy=self.videophy, timings="800x600@60Hz", clock_domain="vga")

        # Leds -------------------------------------------------------------------------------------
        if with_led_chaser:
            self.submodules.leds = LedChaser(
                                                
                                                pads         = platform.request_all("user_led"),
                                                sys_clk_freq = sys_clk_freq
                                            )

        
  

        # Custom LORA-------------------------------------------------------------------------------
        self.add_timer(name="timer1")
        #if timer_uptime:
                #self.timer0.add_uptime()
        #FLASH SPI
        if with_custom_spi:
            from litex.soc.cores.spi import SPIMaster         
            self.submodules.loraspi = SPIMaster(pads=platform.request("lora_spi"), data_width=8, sys_clk_freq=sys_clk_freq, spi_clk_freq=int(100e3), with_csr=True, mode="raw")
            
            self.submodules.di=GPIOIn(pads = platform.request_all("di"),with_irq=True)
            self.submodules.dr = GPIOOut(pads=platform.request("rst"))
        
        #LITESPI
        #from litespi import LiteSPI    
        #if with_custom_pin:
        #    self.submodules.lite_spi =  LiteSPI(    clock_domain="sys",
        #                                            phy=platform.request("phy"),
        #                                            with_mmap=False,
        #                                            master_tx_fifo_depth=len(platform.request("miso_tx")),
        #                                            master_rx_fifo_depth=len(platform.request("mosi_rx")),
        #                                            with_csr=True
        #                                        )#LiteSPI( pads = platform.request("lite_spi") )
        #                                                    
        #                                         #("custom_led", 0, Pins("J1"), IOStandard("LVCMOS33")),
        #    #self.submodules.gpio_bp = GPIOIn( pads = platform.request("custom_btn")) #("custom_btn", 0, Pins("L2"), IOStandard("LVCMOS33")),
            

       
   


# Build --------------------------------------------------------------------------------------------  
def main():
    from litex.soc.integration.soc import LiteXSoCArgumentParser
    parser = LiteXSoCArgumentParser(description="LiteX SoC on board")
    target_group = parser.add_argument_group(title="Target options")
    target_group.add_argument("--build",               action="store_true", help="Build bitstream.")
    target_group.add_argument("--load",                action="store_true", help="Load bitstream.")
    target_group.add_argument("--sys-clk-freq",        default=75e6,        help="System clock frequency.")
    sdopts = target_group.add_mutually_exclusive_group()
    sdopts.add_argument("--with-spi-sdcard",     action="store_true", help="Enable SPI-mode SDCard support.")
    sdopts.add_argument("--with-sdcard",         action="store_true", help="Enable SDCard support.")
    target_group.add_argument("--sdcard-adapter",      type=str,            help="SDCard PMOD adapter (digilent or numato).")
    viopts = target_group.add_mutually_exclusive_group()
    viopts.add_argument("--with-video-terminal", action="store_true", help="Enable Video Terminal (VGA).")
    builder_args(parser)
    soc_core_args(parser)
    args = parser.parse_args()

    soc = BaseSoC(
        sys_clk_freq           = int(float(args.sys_clk_freq)),
        with_video_terminal    = args.with_video_terminal,
        **soc_core_argdict(args)
    )
    soc.platform.add_extension(board._sdcard_pmod_io)
    if args.with_spi_sdcard:
        soc.add_spi_sdcard()
    if args.with_sdcard:
        soc.add_sdcard()
    if args.with_spi_sdcard:
        soc.add_spi_sdcard()
    if args.with_sdcard:
        soc.add_sdcard()
    builder = Builder(soc, **builder_argdict(args))
    builder.build(run=args.build)

    if args.load:
        prog = soc.platform.create_programmer()
        prog.load_bitstream(builder.get_bitstream_filename(mode="sram"))

if __name__ == "__main__":
    main()
