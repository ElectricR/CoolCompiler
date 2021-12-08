class Main inherits IO {
    main(): Object {
        let i: IO in
            if isvoid i then
                out_string("Should happen\n")
            else
                out_string("Should not happen\n")
            fi
    };
};
