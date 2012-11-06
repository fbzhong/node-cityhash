{
  "targets": [
    {
      "target_name": "libcityhash",
      'type': 'static_library',
      'toolsets': ['host','target'],
      "sources": [
          "city.h"
        , "citycrc.h"
        , "city.cc"
      ]
    }
  ]
}
