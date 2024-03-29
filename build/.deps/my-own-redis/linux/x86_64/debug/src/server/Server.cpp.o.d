{
    depfiles_gcc = "Server.o: src/server/Server.cpp src/server/Server.h\
",
    values = {
        "/usr/bin/gcc",
        {
            "-m64",
            "-g",
            "-O0"
        }
    },
    files = {
        "src/server/Server.cpp"
    }
}