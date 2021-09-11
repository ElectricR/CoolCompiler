class StackEntry {
    next : StackEntry;

    set_next(entry : StackEntry) : Object {
        next <- entry
    };
    
    get_next() : StackEntry {
        next
    };

    display(io : IO) : Object { 0 };
};

class PlusEntry inherits StackEntry {
    display(io : IO) : Object {
        {
            io.out_string("+\n");
            next.display(io);
        }
    };
};

class SwapEntry inherits StackEntry {
    display(io : IO) : Object {
        {
            io.out_string("s\n");
            next.display(io);
        }
    };
};

class IntEntry inherits StackEntry {
    value : Int;
    
    get_value() : Int {
        value
    };
    
    set_from_string(input : String) : Object {
        value <- (new A2I).a2i(input)
    };
    
    set_from_int(input : Int) : Object {
        value <- input
    };

    display(io : IO) : Object {
        {
            io.out_string((new A2I).i2a(value).concat("\n"));
            next.display(io);
        }
    };
};

class Stack {
    head : StackEntry <- new StackEntry;

    push(entry : StackEntry) : Object {
        let new_entry : StackEntry <- entry in {
            new_entry.set_next(head);
            head <- new_entry;
        }
    };

    pop() : StackEntry {
        let ret : StackEntry <- head in {
            head <- head.get_next();
            ret;
        }
    };

    display() : Object {
        head.display(new IO)
    };

    execute() : Object {
        case head of
            swap : SwapEntry => {
                pop();
                let a : StackEntry <- pop(),
                b : StackEntry <- pop() in {
                    push(a);
                    push(b);
                };
            };
            plus : PlusEntry => {
                pop();
                let a : IntEntry, b : IntEntry in {
                    case pop() of
                        int_entry : IntEntry => a <- int_entry;
                    esac;
                    case pop() of
                        int_entry : IntEntry => b <- int_entry;
                    esac;
                    let new_int : Int <- a.get_value() + b.get_value(),
                    new_int_entry : IntEntry <- new IntEntry in {
                        new_int_entry.set_from_int(new_int);
                        push(new_int_entry);
                    };
                };
            };
            o : Object => 0;
        esac
    };
};

class Main {
    is_interrupted : Bool <- false;
    io : IO <- new IO;
    stack : Stack <- new Stack;

    main(): Object {
        let input : String in
        while not is_interrupted loop {
            io.out_string(">");
            input <- io.in_string();
            if input = "x" then
                is_interrupted <- true
            else 
                if input = "+" then
                    stack.push(new PlusEntry)
                else
                    if input = "s" then
                        stack.push(new SwapEntry)
                    else
                        if input = "d" then
                            stack.display()
                        else
                            if input = "e" then
                                stack.execute()
                            else
                                let int_entry : IntEntry <- new IntEntry in {
                                    int_entry.set_from_string(input);
                                    stack.push(int_entry);
                                }
                            fi
                        fi
                    fi
                fi
            fi;
        } pool
    };
};
