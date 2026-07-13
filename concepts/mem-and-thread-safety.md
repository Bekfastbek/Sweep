# Scope Based Lifetimes: A Zero-Cost Lifetime model without Lifetime Annotations

Jumping straight to the point. The core idea is a new block construct: `scope {}`. Variables explicitly declared with `scoped` qualifier inside a scope block participates in a **shared lifetime**. The shared data lives until the scope block ends, while non scoped data deallocate at their local scope. Ownership and borrowing concepts are still Rust-like. The below example is in my language's pseudocode since it might be easier showing how it would work:

```cpp
from "std/io" import print;
from "std/mem" import alloc, dealloc;

scope {
    struct MyStruct {
        owned_data: mut ptr i32;
        scoped shared_data: ptr i32; // explicitly shared within this scope

        MyStruct(single_val: i32, shared_ptr: ptr i32) {
            owned_data = alloc(single_val * sizeof(i32));
            shared_data = shared_ptr;
        }

        destructor {
            dealloc owned_data;  // freed at end of local scope
            dealloc shared_data; // freed at end of scope {}
        }
    };

    main {
        shared_dt: ptr i32 = alloc(100 * sizeof(i32);
        {
            MyStruct A(10, shared_dt);
            {
                MyStruct B(20, shared_dt);
            } // B's owned_data is freed, but shared_data and the struct itself stays alive

            print(*A.shared_data, '\n'); // safe, still in scope
        } // A's owned_data is freed, shared_data still alive
    }
} // shared_data is freed here, the scope block ends
```

Here are some rules to clarify further:

- **Single-owner variables** behave like normal RAII. They're destroyed at the end of their local scope.
- **Scoped variables** are destroyed at the end of the enclosing `scope {}` block, regardless of when the owning struct goes out of local scope.
- The `scoped` keyword can **only** appear inside a `scope {}` block. This is enforced by the compiler. You cannot have shared ownership outside of scope unless you use unsafe{} or Arc.
- If a single-owner variable is referenced by more than one owner outside its local scope, the compiler throws an error. No ambiguity.
- Referencing scoped data after the `scope {}` block ends is a compile-time error. You must either pass by value or just extend the scope boundary itself... or just nest scope{}. Here is an example of pass by value:

```cpp
val: mut int;
scope {
    scoped func some_function_with_logic() -> i32 {
        return value;
    }
    val: mut = some_function_with_logic();
}
```

You can also nest scope{} if you have varied lifetimes. The inner scope always destroys first but if you make a reference from the inner scope to the outer scope then the lifetime of that referenced variable gets promoted to the outer scope. So if for example if a function returns a reference, that reference would get promoted to the outer scope which is actually traceable during compile time. Here is an example:

```cpp
// Assume this scope is in a different file i.e. it is a library
scope {
    scoped func function_with_heap() -> str {
        //logic
        promote some_string;
        return some_string;
    } 
} // The logic of that function will all die including the shared variables but "some_string" will stay alive with a promoted lifetime because of the snippet below (assume that snippet is a different file)

scope {
    value: str = function_with_heap(); // The function's lifetime got promoted to this scope instead which only carries the heap allocated string
}
```

The destructor serves as normal RAII for owned variables but acts as destruction hint for shared variables:

```cpp
destructor {
    delete owned_data;  // compiler: "this dies at local scope end"
    delete shared_data; // compiler: "this is scoped, takes the method and order of destruction and run it once at scope {} end"
}
```

If you end up using Arc or Rc because the lifetime is dynamic, you must still wrap it in scope block to prevent potential memory leaks. Here is an example:

```cpp
scope {
    func function() -> i32 { 
        Rc value: i32;
    }
    // more logic and consider more functions
} // If for some reason the ref counter gets in a cycle, scope block will guarantee the destruction regardless 
  // since we know the variable has a ref counter we can do a check if the variable is already freed or not 
  // and if it's free we skip the destruction and move the function or struct out of stack otherwise we free the variable 
  // ensuring that memory leak is impossible 
```

And if the lifetimes are truly unknown, then the only option left is unsafe{} which no language can solve since the lifetime is handled by OS or FFI, etc.
Think of the ownership model as **upgraded RAII**. You write your code like normal RAII with destructors and if you want scoped variables then wrap the structs and functions with scope{} and use the scoped qualifier and declare the destructor then you can freely pass references of scoped variables or structs and the cleanup is automatic with no runtime cost. The scope block is the lifetime. Everything inside it shares that lifetime for `scoped` data. Everything outside cannot touch it. The compiler enforces this statically.


---

## Safety in Concurrency

The concurrency model is built around a single primitive: `spawn{}`. 
By default, every variable inside spawn is thread local, each thread gets its own copy. 
Shared state is only accessible inside a `merge{}` block, making data races structurally unrepresentable rather than just discouraged.

```cpp
    data[16384]: i32;
    threads: i16;
    @feature get_thread_count = threads;
    shared total: atomic i32 = 0;
    shared local_sum: atomic i32 = 0;
    spawn(threads, steal) {
        for i: mut i32 = 0; i < slice_size; i++ {
            local_sum: mut += data[i];
        }
        merge {
            combine local_sum;
            total: mut += local_sum;
        }
    }
```

- Thread local is the default, no exceptions
- Shared variables are only accessible inside merge
- Shared atomic and shared rw are merge strategies, not freely accessible state
- Structural deadlock detection via static wait graph analysis at compile time

An optional `steal` parameter enables dynamic load balancing. The runtime takes half the queue from an idle thread's pool and migrates tasks. Since data is local to the task not the thread ID, task migration preserves the thread local guarantee completely.

For IO, sockets, or event driven patterns that don't fit data parallelism, spawn supports manual mode with thread qualifiers:

```cpp
    shared buffer[1024]: rw i8;
    shared buffer_size: rw i16 = 0;
    shared connection: rw i32 = 0;

    spawn(2, manual) {
        _thread2 __pause;

         while _thread1 !connection {
            _thread1 connection = socket_accept();
        }
        _thread1 buffer_size: rw = socket_read(connection, buffer);
        while _thread1 buffer_size !> 0 {
            if _thread1 buffer_size > 0 { 
                _thread2 __continue;
            }
        }

        _thread2 socket_process(buffer);

        merge {
            buffer_size: mut = 0;
        }
    }
```

Thread qualifiers are bound to the spawn block and cannot escape it. 
Merge is still the only safe data handoff point even in manual mode. 
`__pause` and `__continue` give lifecycle control for waiting on external events and can also be used to implement your own AsyncI/O.

Data races are structurally impossible. The only access point between threads is `merge{}`, which is sequential. Thread local data is never shared between tasks regardless of which thread executes them. There is no code path where two threads can simultaneously access the same mutable state.

This also means we know at what points where threads share data therefore we can optimize the generated assembly with single threaded optimizations and the programmer can write `merge{}` in such a way that there are minimal aliasing or atomic checks. It could unlock performance which is comparable or even faster than unsafe C programs.
