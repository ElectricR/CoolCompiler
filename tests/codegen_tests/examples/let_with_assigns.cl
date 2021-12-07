class Main inherits IO {
    main(): Object {
        let a: Int <- 42 in
            let b: Int <- a in 
                let c: Int <- a in {
                    out_int(a).out_string("\n");
                    out_int(b).out_string("\n");
                    out_int(c).out_string("\n");
                    a <- 6;
                    b <- a * 2;
                    c <- b * 2;
                    out_int(a + b + c).out_string("\n");
        }
    };
};
