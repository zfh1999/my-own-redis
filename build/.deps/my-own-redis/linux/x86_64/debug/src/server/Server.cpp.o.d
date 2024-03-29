{
    depfiles_gcc = "Server.o: src/server/Server.cpp src/server/Server.h\
",
    files = {
        "src/server/Server.cpp"
    },
    values = {
        "/usr/bin/gcc",
        {
            "-m64",
            "-g",
            "-O0"
        }
    }
}