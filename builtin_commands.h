#ifndef BUILTINCOMMANDS_H
#define BUILTINCOMMANDS_H

int (**builtin_commands)(int argc,char **argv);

int cd(int argc,char **argv);

#endif