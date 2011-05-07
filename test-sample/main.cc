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

    return 0;
}

