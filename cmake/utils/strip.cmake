macro(strip target destination)
    add_custom_command(TARGET "${target}" POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} --only-keep-debug $<TARGET_FILE_NAME:${target}> ${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_NAME:${target}>.debug
        COMMAND ${CMAKE_STRIP} -g $<TARGET_FILE_NAME:${target}>)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_NAME:${target}>.debug DESTINATION "${destination}")
endmacro()