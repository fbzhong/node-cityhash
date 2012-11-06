{
	"targets": [
		{
			"target_name": "node-cityhash",
			"include_dirs": ["cityhash/"],
			"dependencies": ["cityhash/binding.gyp:libcityhash"],
      'cxxflags': [
        '-msse4.2'
      ],
			"sources": [
				"binding.cc"
			]
		}
	]
}
