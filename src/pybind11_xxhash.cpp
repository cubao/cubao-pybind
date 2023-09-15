#include <pybind11/pybind11.h>
#include <pybind11/iostream.h>

#define XXH_INLINE_ALL
#include "xxhash.h"

namespace py = pybind11;
using namespace pybind11::literals;
using rvp = py::return_value_policy;

// extracted from xxhsum.c
typedef union
{
    XXH32_hash_t hash32;
    XXH64_hash_t hash64; /* also for xxh3_64bits */
    XXH128_hash_t hash128;
} Multihash;
typedef enum
{
    algo_xxh32 = 0,
    algo_xxh64 = 1,
    algo_xxh128 = 2,
    algo_xxh3 = 3
} AlgoSelected;
inline AlgoSelected __algo(const int algo)
{
    if (algo == 32) {
        return AlgoSelected::algo_xxh32;
    } else if (algo == 64) {
        return AlgoSelected::algo_xxh64;
    } else if (algo == 128) {
        return AlgoSelected::algo_xxh128;
    } else if (algo == 3) {
        return AlgoSelected::algo_xxh3;
    }
}
inline std::string __hash(Multihash hash, AlgoSelected algo) { return ""; }

/*
 * XSUM_hashStream:
 * Reads data from `inFile`, generating an incremental hash of type hashType,
 * using `buffer` of size `blockSize` for temporary storage.
 */
static Multihash XSUM_hashStream(FILE *inFile, AlgoSelected hashType,
                                 void *buffer, size_t blockSize)
{
    XXH32_state_t state32;
    XXH64_state_t state64;
    XXH3_state_t state3;

    /* Init */
    (void)XXH32_reset(&state32, XXHSUM32_DEFAULT_SEED);
    (void)XXH64_reset(&state64, XXHSUM64_DEFAULT_SEED);
    (void)XXH3_128bits_reset(&state3);
    /* Load file & update hash */
    {
        size_t readSize;
        while ((readSize = fread(buffer, 1, blockSize, inFile)) > 0) {
            switch (hashType) {
            case algo_xxh32:
                (void)XXH32_update(&state32, buffer, readSize);
                break;
            case algo_xxh64:
                (void)XXH64_update(&state64, buffer, readSize);
                break;
            case algo_xxh128:
                (void)XXH3_128bits_update(&state3, buffer, readSize);
                break;
            case algo_xxh3:
                (void)XXH3_64bits_update(&state3, buffer, readSize);
                break;
            default:
                assert(0);
            }
        }
        if (ferror(inFile)) {
            return Multihash{0};
        }
    }

    {
        Multihash finalHash = {0};
        switch (hashType) {
        case algo_xxh32:
            finalHash.hash32 = XXH32_digest(&state32);
            break;
        case algo_xxh64:
            finalHash.hash64 = XXH64_digest(&state64);
            break;
        case algo_xxh128:
            finalHash.hash128 = XXH3_128bits_digest(&state3);
            break;
        case algo_xxh3:
            finalHash.hash64 = XXH3_64bits_digest(&state3);
            break;
        default:
            assert(0);
        }
        return finalHash;
    }
}

bool xxhashForFile(const std::string &path, Multihash &hash,
                   AlgoSelected algo = AlgoSelected::algo_xxh3)
{
    size_t const blockSize = 64 * (1 << 10); // 64 KB
    std::vector<uint8_t> buffer(blockSize);
    FILE *inFile = fopen(path.c_str(), "rt");
    if (!inFile) {
        return false;
    }
    bool succ = false;
    hash = XSUM_hashStream(inFile, algo, &buffer[0], buffer.size(), succ);
    fclose(inFile);
    return succ;
}

void bind_xxhash(py::module &m)
{
    m.def(
         "xxhash_for_file",
         [](const std::string &path, int algo) {
             Multihash hash;
             if (__xxhashForFile(path, hash, __algo(algo))) {
                 return __hash(hash, __algo(algo));
             }
             return "";
         },
         "path"_a)
        .def(
            "xxhash",
            [](const std::string &bytes) -> std::string {
                //  xxHash64(bytes, hash);
                return "";
            },
            "bytes"_a)
        //
        ;
}
