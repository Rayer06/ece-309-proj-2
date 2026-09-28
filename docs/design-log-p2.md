# Design Log — Project 2

(500–800 words total. See spec §5 for what each section must cover.)

## Growth factor and amortized cost
The chosen growth factor for the growable array is 2. This means that every time the
array is at capacity and a new item is to be appended, a new array double the size is
created, and all elements from the old array are copied to the new array. The old 
array is then deleted to avoid memory leaks. 

For example, if the array size is 4, and it is at capacity (meaning there are 4 
elements in it), adding a new element will double the array size to 8, and 5 elements
will be copied into it, including the new element.

When the `Conversation` container initializes, it has a size of 0 and no array
allocation. Adding the first element will create a 1-element array and copy the first
element. Both memory allocation and copying 1 element are operations which are of
constant time, so the time for this step is 1. 

Adding a second element will make a new array of size 2, since the previous array was 
at capacity with `size_ = 1` and `capacity_ = 1`. This step *is* dependent on the
size of the array before, giving a time of 2, as the old element and new element are
both copied into this array.

Adding a third element will also allocate a new array of 4, and copy all previous 
elements and append the new element, thus the time for this step is 3.

When we get to a fourth element however, it only needs to append 1 element to the
existing array, since it is not at capacity (`size_ = 3` and `capacity_ = 4`). Thus,
this step is *not* dependent on the size of the array, so the time for this step is 1.

This pattern continues, where each step that grows the array (at elements 5, 9, 17,
etc.) happens in O(n) time, and every other step happens in O(1) time. Using amortized
analysis, adding up all the steps that grow the array over $n$ push operations 
gives a value growing in $O(n)$ time. Similarly, adding all other operations also 
gives a value that grows in $O(n)$ time. Taking the average gives us an amortized cost
of $\frac{O(n)+O(n)}n = \frac{O(n)}n = O(1)$.

## Rule of Five evidence
The `Conversation` container uses the Rule of Five through its special member
functions: Destructor, Copy Constructor, Copy Assignment, Move Constructor, and Move
Assignment

The destructor simply deallocates the growable array using `delete[]`, preventing a 
memory leak. It also checks if the array is not yet allocated by checking if `size_`
is 0, to prevent a double-free memory bug.

The copy constructor first copies the `capacity_` and `size_` fields to the new 
`Conversation` object. Then it allocates a new array of the same size of the existing
array and does a per-element copy of each `Message` in the array. This ensures that 
there is an entirely different pointer to this array to avoid a double-free memory
bug, among other issues.

The assignment operator first deletes the current `Message` array, before copying 
everything over using the same logic as the copy constructor. This prevents a memory
leak from the existing array. The operator also returns `*this` to allow for chain
assignment.

The move constructor does not allocate or deallocate any arrays. Instead, it steals
the attributes of the source by directly copying all the fields. Then it leaves the
source in an empty state by setting `size_` and `capacity_` to 0, and `data_` to
`nullptr`. 

The move assignment also does the same thing, but it makes sure to deallocate the
current array before stealing attributes to avoid a memory leak. It also returns 
`*this` for chain assignment.

## Sentinel scanner: bounded pending_ proof
The program makes sure that the `pending_` attribute of `SentinelScanner` never
exceeds the size of the sentinel by explicitly checking for this condition at the
end of every `feed` operation. If it is longer, it removes the trailing characters
that cause it to exceed this length.


## What I would change differently
I would approach this project from a test-driven development perspective instead. I 
wrote the `Message`, `Conversation`, and `SentinelScanner` classes before I wrote
the tests for them. When I did get to the tests, they did not work, so I had to spend
more time debugging them, which was probably more than if I wrote the tests first and
then tested them each time I wrote a member function.