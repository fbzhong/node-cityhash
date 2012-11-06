{
  "targets": [
    {
      "target_name": "libcityhash",
      'type': 'static_library',
      "cxxflags": [
        '-msse4.2'
      ],
      "sources": [
          "city.h"
        , "citycrc.h"
        , "city.cc"
      ]
    }
  ]
}
