class Foo {};

class Main inherits IO {
    main(): Object {{
        foo(new Int);
        foo(new Object);
        foo(new Foo);
        foo(self);
    }};

    foo(i: Object): Object {
        case i of 
            a: Int => out_string("Hello, ");
            a: IO => out_string("friend.\n");
            a: Foo => out_string("my old ");
            a: Object => out_string("darkness, ");
        esac
    };
};
