class Main {
    c: Int;

    main(): Object {
        let a: Int <- 6 in
            let b: Int <- a * 2 in {
                c <- b * 2;
                (new IO).out_int(a + b + c).out_string("\n");
        }
    };
};
