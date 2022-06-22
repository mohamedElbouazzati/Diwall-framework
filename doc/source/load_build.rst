
.. _load_build:

Take charge of the liteX
~~~~~~~~~~~~~~~~~~~~~~~~~
.. contents:: Table of Contents 
    :depth: 2
    
Step 1 : Build and load for one target.
""""""""""""""""""""""""""""""""""""""""

Here, you can build and load for one target. The script will create or search for the build at the location where you typed the command.

.. tabs::  

    .. tab:: build & load

        .. code-block:: bash

            python3 -m litex_boards.targets.<BOARD> --build --load
    
    .. tab:: build

        .. code-block:: bash

            python3 -m litex_boards.targets.<BOARD> --build

    .. tab:: load

        .. code-block:: bash

            python3 -m litex_boards.targets.<BOARD> --load

For more informations you can execute this.

.. code-block:: bash

    python3 -m litex_boards.targets.<BOARD> --help
             
Step 2 : Simulation build without device.
""""""""""""""""""""""""""""""""""""""""""

After you have build, you can simulate your architecture without board. Try this command in repositories you have build.

.. code-block:: bash

    litex_sim



Step 3 : Boot demo throght rom.
""""""""""""""""""""""""""""""""

When you load demo to rom, system automaticly boot to demo.bin, you can try this commande.

.. code-block:: bash

    python3 -m litex_boards.targets.<name_board> --integrated-rom-init=demo.bin --build --load

Step 4 : Boot demo throght serial port.
"""""""""""""""""""""""""""""""""""""""

(Fix), if the boot stuck at (**Liftoff!**), you should delete ``-flto`` to ``<dir_name>/litex/litex/soc/software/common.mak`` file, after **recompile demo** that it should work.

.. dropdown:: :fa:`eye,mr-1` Stuck boot ! 

    .. image:: ./image/c7.png

Execute these commands to compile demo and load the boot throght ``litex_term`` .

.. code-block:: bash

    litex_bare_metal_demo --build-path=build/<name_board> 
    litex_term /dev/ttyUSB3 --kernel=demo.bin

(Fix), if you can't boot, and you have **CRC errors**. It's because you ``main_ram`` don't exist. 
When you boot throght serial port, your bin file will be stored in ``main_ram``.

You have 2 solutions for resolve this problem.

.. tabs::  

    .. tab:: Solution 1

        Build on command-line with main_ram option.

        .. code-block:: bash

           python3 -m litex_boards.targets.<name_board> --integrated-main-ram-size=0x10000 --build --load
    
    .. tab:: Solution 2

        Modifie file to take initial value ``0x10000`` on ``main_ram`` option.
        Go on ``soc_core.py`` file, and modify **MAIN_RAM line** .
        
        .. code-block:: python

            # MAIN_RAM parameters
            soc_group.add_argument("--integrated-main-ram-size", default=0x20000, type=auto_int, help="size/enable the integrated main RAM.")



Reference `load_build`_.
