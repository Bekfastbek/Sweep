func add_vector[] -> double { // Array functions, if number inside the parameter is not defined then it's basically infinite, can also take references
    double result; // Init as 0 when i make the compiler and it wont store garbage value
    for (i32 i = 0; i < sizeof(add_vector) / sizeof(double); i++) { // the function also holds the actual size during compile time and the function name also initializes as array
        result += add_vector[i];
    }
    return result;
}

func loop { // Void and non parameter functions, i.e. block functions
    while (true) {
        loop.move_stack(); // Calling the stack of the block, compiler function embedded in the function name.
        // Basically if you use loop {i++} it would just wrap while(true){i++}
    }
}

func add(i64 a, i64 b) -> int { // Normal parameter functions
    int result = a + b;
    return result;
}