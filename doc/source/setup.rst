Config and setup LiteX on Linux
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.. contents:: Table of Contents 
    :depth: 2


LiteX sources
=============

You can find LiteX sources and others informations on them github repositories. I set the link below, you can clicked on the button.

.. link-button:: https://github.com/enjoy-digital/litex
    :text: Click here
    :classes: btn-success

Start to setup LiteX
====================
Step 1 : Download and run litex_setup with config.
"""""""""""""""""""""""""""""""""""""""""""""""""""

On third instruction on command-line **write** for user ``(--user to install to user directory)`` and config ``(minimal, standard, full)``.
I suggest you to install without ``--user`` and take full install on ``litex_setup``.

.. code-block:: bash
    :emphasize-lines: 3

    wget https://raw.githubusercontent.com/enjoy-digital/litex/master/litex_setup.py
    chmod +x litex_setup.py
    ./litex_setup.py --init --install --config=full
    ./litex_setup.py --init --install --user (--user to install to user directory) --config=(minimal, standard, full)
    

You should have this when running these commands:

.. tabs::
    .. tab:: cmd 1&2

        .. image:: ./image/c1.png

    .. tab:: cmd 3

        .. image:: ./image/c2.png

    .. tab:: result

        .. image:: ./image/c3.png


Step 2 : Update litex.
"""""""""""""""""""""""

Later, if you need to update all repositories use this syntaxe below.

.. code-block:: bash

    ./litex_setup.py --update


Step 3 : Download a RISC-V toolchain with litex_setup.
"""""""""""""""""""""""""""""""""""""""""""""""""""""""

Next, these commands download RISC-V toolchain

.. code-block:: bash

    pip3 install meson ninja
    ./litex_setup.py --gcc=riscv




Step 5 : Config Path and managing errors of compiling.
""""""""""""""""""""""""""""""""""""""""""""""""""""""""

If you have errors it's potentially because, you dont add directory of differents toolchains to your ENVIRONMENT. You can use anything other **gedit** .

.. code-block:: bash

    gedit ~/.bashrc

End at the end of bashrc file add **export** end replace ``(<dir_...>)``.

.. tabs::  

    .. tab:: VIVADO

        .. code-block:: bash

            export PATH="<dir_projet>/riscv64-unknown-elf-gcc-8.3.0-2019.08.0-x86_64-linux-ubuntu14/bin/:$PATH"
            export LITEX_ENV_VIVADO="<dir_VIVADO>/Xilinx/SDK/2019.1"
    
    .. tab:: ISE

        .. code-block:: bash

            export PATH="<dir_projet>/riscv64-unknown-elf-gcc-8.3.0-2019.08.0-x86_64-linux-ubuntu14/bin/:$PATH"
            export LITEX_ENV_ISE="<dir_ISE>"

Step 6 : Error correction, and install Openocd.
""""""""""""""""""""""""""""""""""""""""""""""""
(Fix):
Replace ``obuilder`` to ``builder`` if you have it at python file, it's an errors this variable is unknown. 

.. dropdown:: :fa:`eye,mr-1` Directory of python file
    
    ``<dir_projet>/litex_boards/targets/<name_board>.py``
    

It's the last commands, tou should install Openocd, to do this execute this command.

.. code-block:: bash

    sudo apt-get install openocd

If tou have this errors after, do the optional instructions on this step (Step 6 : Error correction, and install Openocd. ).

.. tabs::
    .. tab:: Errors displayed

        .. image:: ./image/c4.png

    .. tab:: Wrong code

        .. image:: ./image/c5.png

    .. tab:: Corrected code 

        .. image:: ./image/c6.png

Nice, you are finish to Setup LiteX, you can follow next page to learn how load and build for one target.

Reference `setup`_.


