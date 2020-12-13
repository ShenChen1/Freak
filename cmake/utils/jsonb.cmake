macro(jsonb target config destination)
    add_custom_command(TARGET ${target} PRE_BUILD
        COMMAND python ${FREAK_TOOLS_PATH}/jsonb-c/src/jsonb_c.py --source=${config} --installdir=${destination})
    include_directories(${FREAK_TOOLS_PATH}/jsonb-c/inc/)
endmacro()