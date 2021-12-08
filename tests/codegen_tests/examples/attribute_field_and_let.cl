class Main inherits IO {
    a: Int <- 6;

    main(): Object {
        foo(a * 2)
    };

    foo(b: Int): Object {
        let c: Int <- b * 2 in
            out_int(a + b + c).out_string("\n")
    };
};
