import cubao_pybind as m
from cubao_pybind import xxhash, xxhash_for_file


def test_main():
    assert m.add(1, 2) == 3
    assert m.subtract(1, 2) == -1


print(xxhash_for_file(__file__, algo=64))
print(xxhash('hello'))
