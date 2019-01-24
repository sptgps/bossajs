{
  "targets": [{
    "target_name": "bossa",
    "include_dirs": [
      "src",
      "<!(node -e \"require('nan')\")"
    ],
    "sources": [
      "src/index.cc",
      "src/bossajs.cc"
    ]
  }]
}