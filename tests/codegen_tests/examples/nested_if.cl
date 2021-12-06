class Main inherits IO {
    main(): Object {
        if false then {
            out_string("Should not happen\n");
        } else if true then {
            out_string("Should happen\n");
        } else {
            out_string("Should not happen\n");
        } fi fi
    };
};
