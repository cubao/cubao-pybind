# cubao-pybind

Online document: **[readthedocs](http://cubao-pybind.readthedocs.io/)**

<!--intro-start-->

## Usage

Install:

```bash
python3 -m pip install cubao_pybind # install from pypi
python3 -c 'import cubao_pybind; print(cubao_pybind.add(1, 2))'
```

CLI interface: (created with [python-fire](https://github.com/google/python-fire))

```bash
python3 -m cubao_pybind add 1 2
python3 -m cubao_pybind subtract 9 4
```

Help:

```bash
$ python3 -m cubao_pybind --help

$ python3 -m cubao_pybind pure_python_func --help
```

More:

```bash
python3 -m cubao_pybind xxhash_for_file README.md
```

<!--intro-end-->
