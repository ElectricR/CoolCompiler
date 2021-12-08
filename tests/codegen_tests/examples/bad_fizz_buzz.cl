class Main inherits IO {
    i: Int <- 0;

    main(): Object {
        while not (i = 100) loop {
            if i = 15 then 
                out_string("FizzBuzz\n")
            else
                if i = 3 then
                    out_string("Fizz\n")
                else
                    if i = 5 then
                        out_string("Buzz\n")
                    else
                        out_int(i).out_string("\n")
                    fi
                fi
            fi;
            i <- i + 1;
        } pool
    };
};
