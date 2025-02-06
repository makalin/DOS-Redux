#ifndef COMMANDS_H
#define COMMANDS_H

typedef struct {
    const char* name;           // Command name
    const char* description;    // Command description
    int (*handler)(int argc, char** argv);  // Command handler function
} shell_command_t;

// Built-in commands
int cmd_dir(int argc, char** argv);
int cmd_cd(int argc, char** argv);
int cmd_copy(int argc, char** argv);
int cmd_del(int argc, char** argv);
int cmd_type(int argc, char** argv);
int cmd_help(int argc, char** argv);

#endif // COMMANDS_H 