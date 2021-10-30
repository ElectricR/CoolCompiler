class A {
    foo(b : Int) : B {
        new B
    };
};

class B {
    bar(s : String) : Bool {
        true
    };
};

class Main {
    main() : Object {
        (new A).foo(42).bar("goo")
    };
};
