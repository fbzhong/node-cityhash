# node-cityhash #

A NodeJS binding for [Google CityHash](http://code.google.com/p/cityhash/).

### Install ###

```
npm install cityhash
```

### Functions ###

    64-bit hash object:
    {
        low:    {unsigned long}, eg 1727229466.
        high:   {unsigned long}, eg 1535838579.
        uint64: {boolean} true if hash object is uint64. Otherwise undefined.
        value:  {string}, eg "6596376470467341850".
    }

    128-bit hash object:
    {
        low:     {64-bit hash object}.
        high:    {64-bit hash object}.
        uint128: {boolean} true if hash object is uint128. Otherwise undefined.
        value:   {string}, "low64,high64", eg "9138004313465017137,12242971252332641544".
    }

    /**
     * convert string or number to hash object.
     * @param {unsigned long long | string} the string or number to be convered to hash object.
     * @return {hash} 64-bit hash object or 128-bit hash object.
    **/
    function objectify(obj);

    /**
     * stringify the hash object.
     * @param {hash} 64-bit hash object or 128-bit hash object.
     * @return {string} the hash string value, eg "6596376470467341850" or "9138004313465017137,12242971252332641544".
    **/
    function stringify(obj);

    /**
     * Compute hash for str by CityHash64().
     * @param {string} str The string to compute hash.
     * @param {unsigned long long | string | 64-bit hash object} [seed0] The hash seed.
     * @param {unsigned long long | string | 64-bit hash object} [seed1] The hash seed.
     * @return {hash} 64-bit hash value.
    **/
    function hash64(str, [seed0], [seed1]);

    /**
     * Compute hash for str by CityHash128().
     * @param {string} str The string to compute hash.
     * @param {string | 128-bit hash object} [seed0] The hash seed.
     * @return {hash} 128-bit hash value.
    **/
    function hash128(str, [seed0]);

    /**
     * Compute hash crc for str by CityHashCrc128().
     * @param {string} str The string to compute hash.
     * @param {string | 128-bit hash object} [seed0] The hash seed.
     * @return {hash} 128-bit hash value.
    **/
    function crc128(str, [seed0]);

    /**
     * Compute hash crc for str by CityHashCrc256().
     * @param {string} str The string to compute hash.
     * @return {array} 4 64-bit hash object in an array, eg [hash1, hash2, hash3, hash4].
    **/
    function crc256(str);

### Usage ###

    var cityhash = require('node-cityhash');
    var hash;

    hash = cityhash.stringify({low:1727229466, high:1535838579});
    // hash = '6596376470467341850'

    hash = cityhash.objectify('6596376470467341850');
    // hash = {low: 1727229466, high: 1535838579, uint64: true}

    hash = cityhash.hash64('Hello');
    // hash = {
    //    low: 1727229466,
    //    high: 1535838579,
    //    uint64: true,
    //    value:'6596376470467341850'
    //    }

    hash = cityhash.hash64('Hello', 87392039);
    // hash = {
    //        low: 2569634289,
    //        high: 3664379964,
    //        uint64: true,
    //        value: '15738392108067291633'
    //    }

    hash = cityhash.hash64('Hello', cityhash.objectify(87392039));
    // hash = {
    //        low: 2569634289,
    //        high: 3664379964,
    //        uint64: true,
    //        value: '15738392108067291633'
    //    }

    hash = cityhash.hash64('Hello', 87392039, 1230234);
    // hash = {
    //        low: 2389520903,
    //        high: 3787608545,
    //        value: '16267654833214665223',
    //        uint64: true,
    //    }

    hash = cityhash.hash128('Hello');
    // hash = {
    //        "low": {
    //            "low": 68277041,
    //            "high": 2127607426,
    //            "value": "9138004313465017137",
    //            "uint64": true
    //        },
    //        "high": {
    //            "low": 3936042248,
    //            "high": 2850538876,
    //            "value": "12242971252332641544",
    //            "uint64": true
    //        },
    //        "value": "9138004313465017137,12242971252332641544",
    //        "uint128": true
    //    }

    hash = cityhash.hash128('Hello', '12343,30293');
    // hash = {
    //        "low": {
    //            "low": 3184066266,
    //            "high": 3674042232,
    //            "value": "15779891233746910938",
    //            "uint64": true
    //        },
    //        "high": {
    //            "low": 4196783977,
    //            "high": 3519958761,
    //            "value": "15118107765960464233",
    //            "uint64": true
    //        },
    //        "value": "15779891233746910938,15118107765960464233",
    //        "uint128": true
    //    }

    hash = cityhash.hash128('Hello', cityhash.objectify('12343,30293'));
    // hash = {
    //        "low": {
    //            "low": 3184066266,
    //            "high": 3674042232,
    //            "value": "15779891233746910938",
    //            "uint64": true
    //        },
    //        "high": {
    //            "low": 4196783977,
    //            "high": 3519958761,
    //            "value": "15118107765960464233",
    //            "uint64": true
    //        },
    //        "value": "15779891233746910938,15118107765960464233",
    //        "uint128": true
    //    }

    hash = cityhash.crc128('Hello');
    // hash = {
    //        "low": {
    //            "low": 68277041,
    //            "high": 2127607426,
    //            "value": "9138004313465017137",
    //            "uint64": true
    //        },
    //        "high": {
    //            "low": 3936042248,
    //            "high": 2850538876,
    //            "value": "12242971252332641544",
    //            "uint64": true
    //        },
    //        "value": "9138004313465017137,12242971252332641544",
    //        "uint128": true
    //    }

    hash = cityhash.crc128('Hello', '12343,30293');
    // hash = {
    //        "low": {
    //            "low": 3184066266,
    //            "high": 3674042232,
    //            "value": "15779891233746910938",
    //            "uint64": true
    //        },
    //        "high": {
    //            "low": 4196783977,
    //            "high": 3519958761,
    //            "value": "15118107765960464233",
    //            "uint64": true
    //        },
    //        "value": "15779891233746910938,15118107765960464233",
    //        "uint128": true
    //    }

### Benchmark ###

todo
