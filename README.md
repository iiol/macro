# Strings

Create new strings with fixed length
```c
string str;

str = str_new(5);
```

Create new strings with dynamic length
```c
string str;

str = str_new(5);
```
or
```c
string str;

str = str_set(NULL, "Hello World");
```

Append to string
```c
string str;

str = str_set(NULL, "Hello ");
str = str_cat(str, "World");
```

Splitting strings
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

# Lists

TODO

# Logging macros

```c
debug(fmt, ...);
warning(fmt, ...);
error(exit_code, fmt, ...);
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
sqr(x);
SWAP(a, b);
SWAP_MW(a, b);
MAX(a, b);
MIN(a, b);
SYSCALL(exit_code, func, ...);
```
