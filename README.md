# Freak

![host-build](https://github.com/ShenChen1/Freak/workflows/host-build/badge.svg)
<a href="https://scan.coverity.com/projects/shenchen1-freak">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/22575/badge.svg"/>
</a>

## how to build
```
cmake -Bbuild -DCMAKE_TOOLCHAIN_FILE=./cmake/platforms/host.cmake
cmake --build .
```

## Directory structure
```
Freak
├── cmake
├── fw
│   ├── cjson - a JSON parser in ANSI C
|   |   (https://github.com/DaveGamble/cJSON.git)
│   ├── freetype - a freely available software library to render fonts
|   |   (https://gitlab.freedesktop.org/freetype/freetype.git)
│   ├── media-server - a streaming media server related library
|   |   (https://github.com/ireader/media-server.git)
│   ├── mongoose - embedded web server
|   |   (https://github.com/cesanta/mongoose.git)
│   ├── nanomsg - a simple high-performance implementation of several "scalability protocols"
|   |   (https://github.com/nanomsg/nanomsg.git)
│   ├── ufifo - an IPC library using share-memory
|   |   (https://github.com/ShenChen1/ufifo.git)
│   └── uthash - C macros for hash tables and more
|       (https://github.com/troydhanson/uthash.git)
├── mod
│   ├── alg-hi3516d
│   ├── alg-host
│   ├── app
│   ├── bsp
│   ├── media
│   ├── proto
│   ├── rtsp
│   ├── utils
│   ├── vsf
│   ├── vsf-hi3516d
│   ├── vsf-host
│   └── web
├── scripts
├── sdk
│   ├── hi3516d
│   └── host
└── tools
    └── jsonb-c
```
