class Main inherits IO {
    a: Int <- 6;

    main(): Object {
        foo(a * a)
    };

    foo(a: Int): Object {
        let a: Int <- a * a in
            out_int(a).out_string("\n")
    };
};
