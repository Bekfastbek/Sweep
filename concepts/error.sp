err {
    NUMARRAY_ERROR_OOM:
        // abort or just try and handle it safely by defining the behavior here
        // define the behavior here and write more cases for each error, err should be defined before you write any function ideally since it makes readable by not scrolling much
        // here it also allows the user to handle errors like if they want to skip the function, exit the program, handle and continue, etc
}

func vector_multi[2] -> double {
    if (vector_multi[1] % 2 == 0) {
        vector_multi[0] << vector_multi[1]; // I know the code is wrong I just wanted to show some example
    }
    vector_multi[0] * vector_multi[1];
}