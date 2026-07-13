// Consider this as main.sp, at the very top of the file you can declare an optional @project flag which manages the whole project and the build system

@project {
    project.name = "something_application";
    project.mode = embedded;
    build.stdlib = false;
    // if the project.mode is embedded, then the user can freely access safety options and can freely disable features
    safety.bounds_checking = false;
    safety.stack_overflow_check = false;
    safety.integer_overflow = false;
    safety.null_check = false;
    safety.heap_alloc = false; // pure stack, true will allow to write custom allocators for heap
    safety.multithreading = false;
    safety.simd = false;
    safety.uninitialized_read = true;
}

main {
    // Logic here
}