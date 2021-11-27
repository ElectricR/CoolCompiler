class Main {
    main(): Object {
      abort()
    };

    foo(): String {
      type_name()
    };

    bar(): Object {
      copy()
    };

    -- test String methods
    sfoo(): Int {
      let str: String <- "hello" in
        str.length()
    };

    sbar(): String {
      let str: String <- "hello" in
        str.concat(" world")
    };

    sbaz(): String {
      let str: String <- "hello" in
        str.substr(0, 3)
    };
};

class A inherits IO {
    main(): Object {
      out_string("hello")
    };

    foo(): Object {
      out_int(1)
    };

    bar(): String {
      in_string()
    };

    baz(): Int {
      in_int()
    };
};
