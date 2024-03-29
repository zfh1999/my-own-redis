{
    depfiles_gcc = "main.o: src/main.cpp src/server/Server.h\
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
        "src/main.cpp"
    }
}