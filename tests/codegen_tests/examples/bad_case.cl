class Main inherits IO {
    main(): Object {{
        foo(self);
    }};

    foo(i: Object): Object {
        case i of 
            a: Int => out_string("Hi!\n");
        esac
    };
};
