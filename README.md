 N-Way Pipe
------------

Developing a simple 1-to-N pipe structure just to prove to myself that it is not that tough to implement.
Will contain a very simple parser to parse a command similar to : 

        $ npipe "command1 with args" "command2 with args" "command3 with args" ...

the first command is the only writer, and all the other commands read from whatever the first command has written, so to say, a 1-to-N pipe is set up between command1 and command2, command3, ... so on.

Example
        
        $ ./npipe "printf Hello" "./utils/fcat output.txt" "grep H" "nl"

This will print the output from printf, run it through grep, and will also write it to "output.txt" in the current folder. Note that fcat is a simple cat like utility for writing to files, where the files are given as arguments.
 


