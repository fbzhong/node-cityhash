#include <iostream>
#include <city.h>
#include <citycrc.h>

int main (int argc, const char * argv[])
{

    // insert code here...
    std::cout << "Hello, World!\n";

    std::cout << "CityHash64(\"Hello\") : " << CityHash64("Hello", strlen("Hello")) << std::endl;

    uint128 hash;

    hash = CityHash128("Hello", strlen("Hello"));
    std::cout << "CityHash128(\"Hello\") : " << hash.first << "," << hash.second << std::endl;

    hash = CityHash128("hello", strlen("hello"));
    std::cout << "CityHash128(\"hello\") : " << hash.first << "," << hash.second << std::endl;

    uint128 seed;
    seed.first = 12343;
    seed.second = 30293;
    hash = CityHash128WithSeed("Hello", strlen("Hello"), seed);
    std::cout << "CityHash128WithSeed(\"Hello\") : " << hash.first << "," << hash.second << std::endl;

    hash = CityHashCrc128("Hello", strlen("Hello"));
    std::cout << "CityHashCrc128(\"Hello\") : " << hash.first << "," << hash.second << std::endl;

    hash = CityHashCrc128WithSeed("Hello", strlen("Hello"), seed);
    std::cout << "CityHashCrc128WithSeed(\"Hello\") : " << hash.first << "," << hash.second << std::endl;

    char *longString = "CityHash provides hash functions for strings. The latest stable version is cityhash-1.0.1.tar.gz. The functions mix the input bits thoroughly but are not suitable for cryptography. We provide reference implementations in C++, with a friendly MIT license. To download the code with svn use these instructions. The README contains a good explanation of the various CityHash functions. However, here is a short summary: CityHash64() and similar return a 64-bit hash. Inside Google, where CityHash was developed starting in 2010, we use variants of CityHash64() mainly in hash tables such as hash_map<string, int>. CityHash128() and similar return a 128-bit hash and are tuned for strings of at least a few hundred bytes. Depending on your compiler and hardware, it may be faster than CityHash64() on sufficiently long strings. It is known to be slower than necessary on shorter strings, but we expect that case to be relatively unimportant. Inside Google we use variants of CityHash128() mainly for code that wants to minimize collisions. CityHashCrc128() and CityHashCrc256() and similar are additional variants, specially tuned for CPUs with SSE4.2.";
    hash = CityHashCrc128(longString, strlen(longString));
    std::cout << "CityHashCrc128(longString) : " << hash.first << "," << hash.second << std::endl;

    uint64 crc256_results[4];
    CityHashCrc256("Hello", strlen("Hello"), crc256_results);

    return 0;
}

