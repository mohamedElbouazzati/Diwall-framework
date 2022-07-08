# README

Repository about a SoC with wireless capabilities build with LiTex for FPGA.

```
git clone https://ptanguy@git.renater.fr/authscm/ptanguy/git/stage-terrine/stage-terrine.git
git submodule init
git submodule update
```

## Read the doc

In the following we suppose that you have the Pip software for Python3.
If not on a debian you can install it such as:

```
sudo apt install python3-pip
```

Install dependancies:

```
pip install -r doc/requirements.txt
```

It will install Sphinx which is a tool that makes it easy to create
documentation.

To generate and read the doc:

```
make -C doc html
firefox doc/build/html/index.html
```

