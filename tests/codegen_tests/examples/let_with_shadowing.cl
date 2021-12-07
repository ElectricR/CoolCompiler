class Main inherits IO {
    s: String <- "Should not happen\n";

    main(): Object {
        let s: String <- "Should happen\n" in {
            out_string(s);
        }
    };
};
