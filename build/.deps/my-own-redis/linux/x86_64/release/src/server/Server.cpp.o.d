{
    values = {
        "/usr/bin/gcc",
        {
            "-m64",
            "-fvisibility=hidden",
            "-fvisibility-inlines-hidden",
            "-O3",
            "-DNDEBUG"
        }
    },
    depfiles_gcc = "Server.o: src/server/Server.cpp src/server/Server.h\
",
    files = {
        "src/server/Server.cpp"
    }
}