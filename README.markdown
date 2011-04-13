# node-cityhash #

## Description ##

A NodeJS binding for [Google CityHash](http://code.google.com/p/cityhash/).

### Install ###

Run:

	./install.sh

### Functions ###

	/**
	 * Compute hash for str by CityHash64().
	 * @param {string} str The string to compute hash.
	 * @param {unsigned long long} [seed0] The hash seed.
	 * @param {unsigned long long} [seed1] The hash seed.
	 * @return {string} 64-bit hash value, eg "6596376470467341850".
	**/
	function hash64(str, [seed0], [seed1]);

	/**
	 * Compute hash for str by CityHash128().
	 * @param {string} str The string to compute hash.
	 * @param {unsigned long long} [seed0] The hash seed.
	 * @return {string} 128-bit hash value, "low64,high64", eg "11793567364161249803,13788582196852422552".
	**/
	function hash128(str, [seed0]);

### Usage ###

	var cityhash = require('node-cityhash');
	var hash;

	hash = cityhash.hash64('Hello');
	// hash = "6596376470467341850"

	hash = cityhash.hash64('Hello', 87392039);
	// hash = "15738392108067291633"

	hash = cityhash.hash64('Hello', 87392039, 1230234);
	// hash = "16267654833214665223"

	hash = cityhash.hash128('Hello');
	// hash = "11793567364161249803,13788582196852422552"

	hash = cityhash.hash128('Hello', '12343,30293');
	// hash = "9958601979657309277,6783721701693091028"

### Benchmark ###

todo