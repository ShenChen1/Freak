macro(jsonb target item config destination)
    add_custom_target(cfg-${target}-${item}
        COMMAND python ${FREAK_TOOLS_PATH}/jsonb-c/src/jsonb_c.py --source=${config} --installdir=${destination})
    include_directories(${FREAK_TOOLS_PATH}/jsonb-c/inc/)
    add_dependencies(${target} cfg-${target}-${item})
endmacro()