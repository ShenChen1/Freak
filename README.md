# Freak

![host-build](https://github.com/ShenChen1/Freak/workflows/host-build/badge.svg)
<a href="https://scan.coverity.com/projects/shenchen1-freak">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/22575/badge.svg"/>
</a>

## how to build
```
cmake -Bbuild -DCMAKE_TOOLCHAIN_FILE=./cmake/platforms/host.cmake
make -sC build
```
