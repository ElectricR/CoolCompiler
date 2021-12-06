class Main inherits IO {
    main(): Object {
        if "Object" = "Int" then 
            out_string("Should not happen\n")
        else
            out_string("Should happen\n")
        fi
    };
};
