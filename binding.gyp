{
	"targets": [
		{
			"target_name": "node-cityhash",
			"include_dirs": ["cityhash/"],
			"dependencies": ["cityhash/cityhash.gyp:libcityhash"],
			"sources": [
				"binding.cc"
			]
		}
	]
}
