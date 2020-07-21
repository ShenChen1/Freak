#!/bin/sh

GSF_CPU_ARCH=${1:-'x86'}
export LD_LIBRARY_PATH=$(pwd)/lib/${GSF_CPU_ARCH}:${LD_LIBRARY_PATH}

mkdir -p /var/app/www/
mount -o bind $(pwd)/mod/webs/www /var/app/www/
mkdir -p /var/app/cfg/
mount -o bind $(pwd)/mod/mpp/${GSF_CPU_ARCH}/src/scene_auto/src/sensor/param/ /var/app/cfg/
mkdir -p /userdata/data
mount -o bind $(pwd)/../nniefacelib/data/ /userdata/data

cd $(pwd)/bin/${GSF_CPU_ARCH}
chmod +x *.exe
./bsp.exe bsp_def.json bsp_parm.json &
sleep 1
./codec.exe codec_parm.json &
sleep 1
./svp.exe svp_parm.json &
sleep 1
./rtsps.exe rtsps_parm.json &
sleep 1
./webs.exe webs_parm.json &
cd -