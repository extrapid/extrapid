add_rules("mode.debug", "mode.release")
set_optimize("fastest")
add_defines("VERSION=\"v$(env VERSION)\"", "LOG_USE_COLOR")

target("log")
    set_kind("shared")
    add_files("log.c")

