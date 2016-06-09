<h1>rshell</h2>
---------------

Introduction
------------

We are creating a copy of the bash script. Our main goals are to mimic the
ls, mkdir, and echo function of bash. This includes all of their errors,
specific cases, and general functionality. Along with mimicking these functions,
we are also required to include arguments. These are the &&, ||, and ; commands.



Our process
-----------

We had an initial process from our design patter, but that just proved to be a
really bad and hard way to do it. So we scrapped that idea and made it a lot more
simple. Without using any classes, we just had functions that alterred a given
string. From the string it would make two vectors, one of arguments, and one of
the commands and their parameters. The list of arguments would hold ints with
a certain code included in it. 1 would stand for &&, 2 for ||, and 3 for ;.
From this, we would go through the argument list and put each command through
execvp.


BUGS
----

We couldn't figure out a way to output specific codes. I think we needed something
other than perror/

We made it so that # would work if it's inside quotation marks, because of this,
it works for this example
example : echo "sentence" #this still outputs


PART 2
------

DESIGN
------
We worked off our previous design, to include test, we put it into the same format as our previous code. 
we took out flags and added them to a flag list and then push backed test and the path into our parsed list. 
In our execute we checked if the function being passed in was test, and if it was so we would go through a 
different process instead of execvp. In doing so our code worked. For brackets, we would replace the front 
bracket with test and delete the 2nd bracket. 
For parentheses, we replaced the front parentheses with par and the 2nd parentheses with par2. In doing so
we parsed it so that it would include par and stop at par2. The reason behind this was to recursively call 
everything that was inside the parentheses. It would go through every process that a normal string without
parentheses would have

BUGS
----

We weren't able to figure out what to do if there were double parentheses. Another problem would be if our
parentheses was at the end of the call. Ohterwise everything seems to work fine.

Part 3
-------

DESIGN
------
We worked off our previous design to include redirection, appending and piping. We will accomplish this by 
changing the folder descriptors using dup and writing into the redirected files and the cat command will be
handeled with execvp. We will be working off our previous code and will be updating the the parsing system
to take into account of '>', '<', ">>", and '|'.


BUGS
----

Our program does do piping correctly, another issue is that our program does not append correctly as well.
The translate command (tr) also does not work correctly in our program.
