class Main inherits IO {
    a: Int <- 6;
    b: Int <- a * 2;
    c: Int <- b * 2;
    
    main(): Object {
        out_int(a + b + c).out_string("\n")
    };
};
