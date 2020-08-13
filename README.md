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

# List examples

```c
list_new(type);			// return pointer to new alloced list
list_empty(type);		// return pointer to new empty alloced list (for defining local functions)

list_alloc_after(entry);	// return pointer to new alloced entry in list after 'entry'
list_alloc_before(entry);	// return pointer to new alloced entry in list before 'entry'
list_alloc_at_end(entry);	// return pointer to new alloced entry at tail of list
list_alloc_at_start(entry);	// return pointer to new alloced entry at head of list

list_get_head(entry);		// return head of list
list_get_tail(entry);		// return tail of list
list_get_prev(entry);		// return previous element in list
list_get_next(entry);		// return next element in list

list_foreach(start, entry);	// make cycle for each elements in list from 'start' to tail
list_rofeach(start, entry);	// make reverse cycle for each element in list from 'start' to head
list_foreachi(start, entry, i);	// like 'list_foreach', but with iterator 'i'
list_rofeachi(start, entry, i);	// like 'list_rofeach', but with iterator 'i'

list_search_by_elem(entry, elem, value); // return pointer to entry,
					 // where element 'elem' equal 'value'
list_search_by_str(entry, elem, str);	 // return pointer to entry,
					 // where string element 'elem' equal 'str'

list_dup(list);				// duplicate list (without calling user's copy function)
list_clone(list);			// clone list (with calling user's copy function)

list_reverse(list);			// reverse 'list' list
list_merge(alist, blist, pos);		// merge blist to alist at 'pos' position

list_free(entry);			// free entry 'entry' from list
					// and return pointer to new head
list_free_range(entry, from, count);	// free 'count' entries in list from 'from' element (counted from head)
					// and return pointer to new head
list_free_full(entry);			// free full list

list_destroy(entry);		// like 'list_free', but call destroy function before
list_destroy_range(entry);	// like 'list_free_range', but call destroy function before
list_destroy_full(entry);	// like 'list_free_full', but call destroy function before

list_setfunc_destroy(void (*func)(void *entry));		// set destroy function
list_setfunc_cmp(int (*func)(void *a, void *b));		// set compare function
list_setfunc_copy(void (*copy_node)(void *to, void *from));	// set copy function

list_setlfunc_destroy(void *entry, void (*func)(void *entry));			// local version of list_setfunc_destroy
list_setlfunc_cmp(void *entry, int (*func)(void *a, void *b));			// local version of list_setfunc_cmp
list_setlfunc_copy(void *entry, void (*copy_node)(void *to, void *from));	// local version of list_setfunc_copy
```

# Macros

## Logging macros

```c
debug(fmt, ...);            // print debug message
warning(fmt, ...);          // print warning message
error(exit_code, fmt, ...); // print error message and exit with exit_code
perror(str);
assert(exp);
```

## Misc macros

```c
sqr(x);                        // Square of x
SWAP(a, b);                    // Swap 2 variables
MAX(a, b);
MIN(a, b);
ARR_SZ(arr);                   // return count of elements in array 'arr'
UNUSED(variable);
SYSCALL(exit_code, func, ...); // Call syscall and exit on failure with exit_code.
                               // 0 for exit_code is a special value mean
                               // don't call exit()
```

# Malloc functions

```c
xmalloc(size_t size);
xzmalloc(size_t size);
xrealloc(void *ptr, size_t size);
```
