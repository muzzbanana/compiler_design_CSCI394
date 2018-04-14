The frame keeps track of the stack as a double ended queue of integers, as well as the frame pointer, stack pointer, first temporary's address for each stack, and maps from the names of each variable of each type (locals, arguments, and temporaries), to their offset from that frame's frame pointer. 

pushframe() takes a vector mapping arguments passed names to their variables and a vector mapping locals names to their variables. popframe() pops the frame off the stack. 

The stack is a public variable so it can be directly manipulatable by other functions if necessary. Everything else is public because at that point might as well. 

However, temporaries can be added, looked up, and popped to directly and immediately grab their values. (This can also be modified to look up their stack address instead if that's more useful to you)