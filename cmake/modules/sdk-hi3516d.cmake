set(SDK_NAME sdk-hi3516d)

add_custom_target(${SDK_NAME})
include_directories(${FREAK_SYSROOT_PATH}/usr/local/include/mpp)
link_directories(${FREAK_SYSROOT_PATH}/usr/local/lib/mpp)

list(APPEND LIB_SDK_NAME "aaccomm")
list(APPEND LIB_SDK_NAME "aacdec")
list(APPEND LIB_SDK_NAME "aacenc")
list(APPEND LIB_SDK_NAME "aacsbrdec")
list(APPEND LIB_SDK_NAME "aacsbrenc")
list(APPEND LIB_SDK_NAME "dnvqe")
list(APPEND LIB_SDK_NAME "hdmi")
list(APPEND LIB_SDK_NAME "_hiae")
list(APPEND LIB_SDK_NAME "_hiawb_natura")
list(APPEND LIB_SDK_NAME "_hiawb")
list(APPEND LIB_SDK_NAME "_hicalcflicker")
list(APPEND LIB_SDK_NAME "hi_cipher")
list(APPEND LIB_SDK_NAME "_hidehaze")
list(APPEND LIB_SDK_NAME "_hidrc")
list(APPEND LIB_SDK_NAME "hifisheyecalibrate")
list(APPEND LIB_SDK_NAME "_hiir_auto")
list(APPEND LIB_SDK_NAME "_hildci")
list(APPEND LIB_SDK_NAME "hive_AEC")
list(APPEND LIB_SDK_NAME "hive_AGC")
list(APPEND LIB_SDK_NAME "hive_ANR")
list(APPEND LIB_SDK_NAME "hive_common")
list(APPEND LIB_SDK_NAME "hive_EQ")
list(APPEND LIB_SDK_NAME "hive_HPF")
list(APPEND LIB_SDK_NAME "hive_record")
list(APPEND LIB_SDK_NAME "hive_RES_ext")
list(APPEND LIB_SDK_NAME "hive_RES")
list(APPEND LIB_SDK_NAME "isp")
list(APPEND LIB_SDK_NAME "ive")
list(APPEND LIB_SDK_NAME "md")
list(APPEND LIB_SDK_NAME "mpi")
list(APPEND LIB_SDK_NAME "nnie")
list(APPEND LIB_SDK_NAME "securec")
list(APPEND LIB_SDK_NAME "tde")
list(APPEND LIB_SDK_NAME "upvqe")
list(APPEND LIB_SDK_NAME "VoiceEngine")

add_dependencies(${SDK_NAME} build-${SDK_NAME})