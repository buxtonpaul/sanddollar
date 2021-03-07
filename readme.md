# Sanddollar

Sanddollar is a simple application to allow running C++ like code in script like fashion.That is to 
add a shebang line to the top of the file that points to the installed location (typically /use/local/bin/sanddollar) and set the permissions of the file to execute and you can run the file as with standard script languages.
The 'script' is compiled to a binary and cached locally to allow improved execution time. Scripts are limited to single files + headers.
Currently the C++ standard library is all that is available.

## Installation instructions

### Prerequisites
Currently the script requires g++9 and the boost libraries installed

### Build instructions

```bash
git clone https://github.com/buxtonpaul/sanddollar.git
cd sanddollar
mkdir build
cd build
cmake ..
make
sudo make install
```

You should now be able to run the samples
```
cd sanddollar
./samples/test.run
./samples/streamtest.cc
```

The first sample is a very simple hello world style example. Streamtest.cc demonstrates use of included headerfiles to provide additional functionality ( in thie case Johnathan Wakely's pstream library)


## Usage

You can run scripts on the commandline as you would any other script either relying on the shebang line to resolve to sanddollar handler. Or invoke sanddollar directly which allows a few options.

Options:

-f or --force , Forces recompilation of script regardless of cache state
-c or --check , Does not run the script only verifies it is syntactically correct.
-h or --help , Print out help message

## Attributions
Pstreams library is Copyright Johnathan Wakely under Boost License. https://sourceforge.net/projects/pstreams/


----
## Why
So why would someone want this? The main reason is that it seemed a bit of fun, and a way to play around with the boost libraries. But in addition to this I have come to the conclusion that C++ is a viable alternative to bash, python perl for scripting. 
Because C++ is compiled, you know the whole file is at least syntactically correct. With interpeted languages like bash and python it can be the case that a script appears to be working, but hits a syntax error when a new branch is executed due to different running conditions.

C++ has consistent and easily understood syntax.

Using the standard library's filesystem module provides filesystem interaction as complete as anything in python. With use of the pstream header library we have better process stream control than python. 

If we are scripting something it is because we expect to run it repeatedly. By compiling it to an optimised binary and cacheing it we can expect long term performance gains.

## Why bash Bash?
Is bash really that bad? Well if we look at the top 11 questions on stack overflow and organise them a little we can see the following breakdown.
### Confusing syntax
- When to wrap quotes around a shell variable
- Difference between single and double quotes
- Are shell scripts sensitive to encodings and line endings
- I just assigned a variable but echo $variable shows something else
- Why should there be spacesaround '[' and ']' in bash
- Command not found error in Bash variable assignment     

These are simply indicating how confusing the bash syntax can be. The reason for this is that bash and other shell scripting languages are designed first and foremost to be quick to use from the commandine for interacting with the file system. They do this well!
Entering a command on the commandline it determines if you are trying executing an inbuilt command, or trying to run a program with no special syntax to differentiate them. Which means you can quickly and easily explore the file system and run commands/applications.
Where it falls apart is that it is not a very clear or expressive language for more complex problems, and is reliante on other tools to do the heavy lifting. Primarily throught the GNU coreutils, but also using other scripting languages such as Python, Perl, Awk etc. 
Each of these tools have their own syntax and options.
When running as a script you often do not know if the code /options are legal until you run the script and hit the line in question.



### Actual problems
- Parsing JSON with Unix tools
- How can I get the source directory of a Bash script from within the script itself?
- How to set a variable to output of a command

### Misfiled?
- How to use variable in an awks script (why is this listed under bash)
