.. _create_board:

Create your board in latex
~~~~~~~~~~~~~~~~~~~~~~~~~~
.. contents:: Table of Contents 
    :depth: 2


In this part, i describe how i **create my board and my customs pins** inspired by the files already written. I create customs copy of **Digilent basys3** .
The objective of this is to use one pin for OUTPIN another INPUT
(see picture)

.. panels::

    :img-top: image/s1.png

    Datasheet


Step 1: Define file constraint in platforms:
"""""""""""""""""""""""""""""""""""""""""""""

You should describe the constraint file at ``<dir_project>/litex_boards/platforms/<board_name>.py``.
I use the  ``<board_name>.py``.   


.. dropdown:: :fa:`eye,mr-1` Platforms file description

    .. tabbed:: Part of customs description

        .. code-block:: python

            # Custom peripherique
            ("custom_led", 0, Pins("J1"), IOStandard("LVCMOS33")),
            ("custom_btn", 0, Pins("L2"), IOStandard("LVCMOS33")),

    .. tabbed::  Full file

        .. literalinclude:: poto_basys3_plat.py
            :language: python


Step 2: Define pins to register system in targets:
"""""""""""""""""""""""""""""""""""""""""""""""""""""

To create automaticly generating register for your pin, you should add in your file target at ``<dir_project>/litex_boards/targets/<board_name>.py`` a new **submodules**.
You don't need to create a driver, the **gpio.py** driver match with what we need. Here is an example of what I added:

.. dropdown:: :fa:`eye,mr-1` Targets file description

    .. tabbed:: Part of customs description

        .. code-block:: python

            # Custom pin -------------------------------------------------------------------------------
            if with_custom_pin:
                self.submodules.gpio_led = GPIOOut(pads = platform.request("custom_led"))#("custom_led", 0, Pins("J1"), IOStandard("LVCMOS33")),
                self.submodules.gpio_bp = GPIOIn( pads = platform.request("custom_btn")) #("custom_btn", 0, Pins("L2"), IOStandard("LVCMOS33")),

    .. tabbed::  Full file

        .. literalinclude:: poto_basys3_targ.py
            :language: python



Step 3: Define your constructor name (Optional):
""""""""""""""""""""""""""""""""""""""""""""""""

If you dont use actual constructor names, you should create your's at ``<dir_project>/llitex-boards/litex_boards/__init__.py``.

.. dropdown:: :fa:`eye,mr-1` Init file description

    .. tabbed:: Part of constructor name description

        .. code-block:: python

            # Boards Vendors.
            vendors = [
                "poto"
            ]

    .. tabbed::  Full file init

        .. literalinclude:: poto__init__.py
            :language: python



Reference `create_board`_.