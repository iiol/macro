# String examples

Creating new string with fixed length
```c
string str;

str = str_new(5);
```

Creating new string with dynamic length
```c
string str;

str = str_new(0);
```
or
```c
string str;

str = str_set(NULL, "Hello World");
```

Appending to string
```c
string str;

str = str_set(NULL, "Hello ");
str = str_cat(str, "World");

printf("%s\n", str); // "Hello World"
```

Splitting string
```c
int i;
string path;
string *arr;

path = str_set(NULL, "/home/username/path/to/file");
arr = str_split(path, "/");

for (i = 0; arr[i] != NULL; ++i)
	printf("%s\n", arr[i]);

str_arr_free(arr);
str_free(path);
```

# Doubly linked list examples

**TODO**

# Logging macros

```c
debug(fmt, ...);            // print debug message
warning(fmt, ...);          // print warning message
error(exit_code, fmt, ...); // print error message and exit with exit_code
perror(str);
assert(exp);
```

# Other functions

```c
xmalloc(size_t size);
xrealloc(void *ptr, size_t size);
```

# Other macros

```c
sqr(x);                        // Square of x
SWAP(a, b);                    // Swap 2 variables
SWAP_MW(a, b);                 // Swap 2 machine word length varialbes
MAX(a, b);
MIN(a, b);
UNUSED(variable);
SYSCALL(exit_code, func, ...); // Call syscall and exit on failure with exit_code.
                               // 0 for exit_code is a special value mean
                               // don't call exit()
```
