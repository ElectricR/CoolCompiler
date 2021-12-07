class Main {
    main(): Object {
        let a: Int <- 6 in
            let b: Int <- a * 2 in
                let c: Int <- b * 2 in
                    (new IO).out_int(a + b + c).out_string("\n")
    };
};
