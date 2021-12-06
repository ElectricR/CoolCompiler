class Alice inherits IO {
    responce(): Object {
        out_string("Don't want to talk to you, John.\n")
    };
};

class Bob inherits IO {
    responce(): Object {
        out_string("Oh, hi, John!\n")
    };
};

class John inherits IO {
    greet(): Object {
    {
        out_string("Hi all!\n");
        (new Alice).responce();
        (new Bob).responce();
    }
    };
};

class Main {
    main(): Object {
        (new John).greet()
    };
};
