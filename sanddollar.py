#!/bin/python3
import os
import sys
# script for compiling and running C based script

def convertscript(sourcescript):
    #read the script and strip off the shebang line
    os.system("mkdir -p ./.sanddollar")
    infile=open(sourcescript,'r')
    lines=infile.readlines()
    infile.close()
    outname=None
    if '#!' in lines[0]:
        outname=".sanddollar/{}.c".format(sourcescript)
        outfile=open("{}".format(outname),'w')
        outfile.writelines(lines[1:])
        outfile.close()
    else:
        print ("Bad command interpreter {}".format(lines[0]))
    return outname

def compilescript(infile):
    if(infile):
        os.system("gcc -o {}.out {}".format(infile,infile))
        return "{}.out".format(infile)
    else:
         return None


if __name__ == "__main__":
    target=convertscript(sys.argv[1])
    compiled=compilescript(target)
    paramsttring =' '.join(sys.argv[2:])
    os.system(compiled+' '+paramsttring)