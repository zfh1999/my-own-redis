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
    depfiles_gcc = "main.o: src/main.cpp src/server/Server.h\
",
    files = {
        "src/main.cpp"
    }
}