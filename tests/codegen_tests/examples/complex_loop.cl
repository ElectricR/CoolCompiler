class Main inherits IO {
    x: Int <- 0;

    main(): Object {
        while not (x = 42) loop {
            out_string("I'm still ").out_int(x).out_string("\n");
            x <- x + 1;
        } pool
    };
};
