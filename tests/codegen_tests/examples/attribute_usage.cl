class Main inherits IO {
    main(): Object {
        foo(42)
    };

    foo(x: Int): Object {
        out_int(x).out_string("\n")
    };
};
