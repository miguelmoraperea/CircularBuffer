# CircularBuffer

#### Who would use this?
This project is designed for embedded software engineers working in C language.

#### What does it do?
This project contains a module that initializes a circular buffer and allows the user to add and read values from the circular buffer. It also provide the following functionality:

-	Initialize an array of X size
-	Destroy the array and free the memory
-	Read an element from a specific index
-	Write an element to a specific index
-	Clear the entire buffer
-	Get the length of data stored in the buffer
-	Write an array into the buffer
-	Read all available data from the buffer

#### Why is this implementation good?
This implementation was developed using TDD, hence it includes all tests needed to confirm the correct functionality.

#### When to use it?
This implementation will be used when data needs to be handled in systems with constrained memory.

#### Where to use it?
This implementation can be compiled into any microcontroller using the appropriate compiler and linker.

#### How to use it?
Copy the _**inc**_ and _**src**_ folders to the project you wish to add this functionality on. Make sure to inform your compiler about the location of the new folders.