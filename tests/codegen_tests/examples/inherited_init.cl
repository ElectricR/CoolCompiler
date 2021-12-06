class Foo {
    a: Int <- 6;
};

class Bar inherits Foo {
    b: Int <- a * 2;
};
    
class Main inherits Bar {
    c: Int <- b * 2;
    
    main(): Object {
        (new IO).out_int(a + b + c).out_string("\n")
    };
};
