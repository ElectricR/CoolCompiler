class Main inherits IO {
    main(): Object {{
        if 1 < 2 then 
            out_string("Should happen\n")
        else 
            out_string("Should not happen\n")
        fi;
        if 2 < 1 then 
            out_string("Should not happen\n")
        else 
            out_string("Should happen\n")
        fi;
    }};
};
