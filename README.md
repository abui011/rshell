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
exampls : echo "sentence" #this still outputs
