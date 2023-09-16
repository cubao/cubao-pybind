import cubao_pybind as m
from cubao_pybind import xxhash, xxhash_for_file


def test_main():
    assert m.add(1, 2) == 3
    assert m.subtract(1, 2) == -1


def test_hash():
    print(xxhash_for_file(__file__))
    print(xxhash("hello"))
    print(xxhash("hello", algo=3))
    print(xxhash("hello", algo=128))
    print(xxhash("hello", algo=64))
    print(xxhash("hello", algo=32))
