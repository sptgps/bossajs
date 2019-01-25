{
  "targets": [{
    "target_name": "bossa",
    "include_dirs": [
      "src",
      "extern/bossa/src",
      "<!(node -e \"require('nan')\")"
    ],
    "cflags_cc": [ "-std=c++17" ],
    "cflags_cc!": [ "-fno-exceptions" ],
    "defines": [
      "VERSION=FIXME"
    ],
    "sources": [
      "src/index.cc",
      "src/bossajs.cc",
      "extern/bossa/src/WordCopyArm.cpp",
      "extern/bossa/src/Samba.cpp",
      "extern/bossa/src/Flash.cpp",
      "extern/bossa/src/D5xNvmFlash.cpp",
      "extern/bossa/src/D2xNvmFlash.cpp",
      "extern/bossa/src/EfcFlash.cpp",
      "extern/bossa/src/EefcFlash.cpp",
      "extern/bossa/src/Applet.cpp",
      "extern/bossa/src/WordCopyApplet.cpp",
      "extern/bossa/src/Flasher.cpp",
      "extern/bossa/src/Device.cpp"
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
      }]
    ]
  }]
}