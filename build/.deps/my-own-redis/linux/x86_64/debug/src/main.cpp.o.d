{
    depfiles_gcc = "main.o: src/main.cpp src/server/Server.h\
",
    files = {
        "src/main.cpp"
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