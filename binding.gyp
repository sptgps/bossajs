{
  "targets": [{
    "target_name": "bossa",
    "include_dirs": [
      "src",
      "extern/bossa/src",
      "<!(node -e \"require('nan')\")"
    ],
    "defines": [
      "VERSION=bossajs"
    ],
    "sources": [
      "src/index.cc",
      "src/bossajs.cc",
      "src/observer.cc",
      "src/connectworker.cc",
      "src/closeworker.cc",
      "src/eraseworker.cc",
      "src/infoworker.cc",
      "src/readworker.cc",
      "src/verifyworker.cc",
      "src/writeworker.cc",
      "extern/bossa/src/Applet.cpp",
      "extern/bossa/src/D2xNvmFlash.cpp",
      "extern/bossa/src/D5xNvmFlash.cpp",
      "extern/bossa/src/Device.cpp",
      "extern/bossa/src/EefcFlash.cpp",
      "extern/bossa/src/EfcFlash.cpp",
      "extern/bossa/src/Flash.cpp",
      "extern/bossa/src/Flasher.cpp",
      "extern/bossa/src/Samba.cpp",
      "extern/bossa/src/WordCopyApplet.cpp",
      "extern/bossa/src/WordCopyArm.cpp"
    ],
    "conditions": [
      ['OS=="mac"', {
        "sources": [
          "extern/bossa/src/PosixSerialPort.cpp",
          "extern/bossa/src/OSXPortFactory.cpp"
        ],
        "xcode_settings": {
          "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
          "OTHER_CFLAGS": [ "-std=c++17" ]
        }
      }],
      ['OS=="win"', {
        "sources": [
          "extern/bossa/src/WinSerialPort.cpp",
          "extern/bossa/src/WinPortFactory.cpp"
        ],
        "libraries": [
          "-lsetupapi"
        ],
        # "msvs_configuration_platform": "win32",
        "msvs_settings": {
          "VCCLCompilerTool": {
            "AdditionalOptions": [
              "/EHsc",
              "/std:c++17"
            ]
          }
        }
      }],
      ['OS=="linux"', {
        "sources": [
          "PosixSerialPort.cpp",
          "LinuxPortFactory.cpp"
        ],
        "cflags_cc": [ "-std=c++17" ],
        "cflags_cc!": [ "-fno-exceptions" ]
      }]
    ]
  }]
}