set(SDK_NAME sdk-hi3516d)

add_custom_target(${SDK_NAME})
include_directories(${FREAK_SYSROOT_PATH}/usr/local/include/mpp)
link_directories(${FREAK_SYSROOT_PATH}/usr/local/lib)
link_libraries(-lisp -l_hiae -l_hiawb -lhdmi -l_hidrc -l_hildci -l_hidehaze -lsecurec -live -lmpi -lVoiceEngine -ldnvqe -lupvqe)
add_dependencies(${SDK_NAME} build-${SDK_NAME})
