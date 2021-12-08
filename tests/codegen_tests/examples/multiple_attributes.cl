class Main inherits IO {
    main(): Object {
        foo(6, 12, 24)
    };

    foo(a: Int, b: Int, c: Int): Object {
        out_int(a + b + c).out_string("\n")
    };
};
