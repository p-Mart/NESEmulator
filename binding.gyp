    {
      "target_name": "test",
      "type": "executable",
      "sources": [
          # test filenames
      ],
      "include_dirs": [
        ".",
        "src",
        "<!(node -e \"require('cppunitlite')\")",
        "<!(node -e \"require('nan')\")"
      ],
      "dependencies": [
        "node_modules/cppunitlite/binding.gyp:CppUnitLite",
      ],
      "conditions": [
        ['OS=="win"', {
          }, {
            'cflags_cc': [ '-fexceptions' ]
          }
        ]
      ],
      # sample unit test
    }