#include "lexer.h"
#include <unistd.h>
#include <string.h>

// Forward declare builtins array
static t_builtin builtins[];

// echo komutu
int ft_echo(t_all *all, t_cmd *cmd)
{
    int i;
    int newline;
    (void)all; // Suppress unused parameter warning

    i = 1;
    newline = 1;
    
    if (cmd->args[1] && !strcmp(cmd->args[1], "-n"))
    {
        newline = 0;
        i++;
    }
    
    while (cmd->args[i])
    {
        printf("%s", cmd->args[i]);
        if (cmd->args[i + 1])
            printf(" ");
        i++;
    }
    printf("\n");
    if (newline)
        printf("\n");
    return (0);
}

// cd komutu
int ft_cd(t_all *all, t_cmd *cmd)
{
    (void)all; // Suppress unused parameter warning
    if (!cmd->args[1])
        return (chdir(getenv("HOME")));
    return (chdir(cmd->args[1]));
}

// pwd komutu
int ft_pwd(t_all *all, t_cmd *cmd)
{
    char cwd[1024];
    (void)all; // Suppress unused parameter warning
    (void)cmd; // Suppress unused parameter warning
    
    if (getcwd(cwd, sizeof(cwd)))
    {
        printf("%s\n", cwd);
        return (0);
    }
    return (1);
}

// Built-in komut tablosu - static so it's only visible in this file
static t_builtin builtins[] = {
    {"echo", ft_echo},
    {"cd", ft_cd},
    {"pwd", ft_pwd},
    {NULL, NULL}
};

// Export these functions for use in other files
int is_builtin(char *cmd)
{
    int i;

    i = 0;
    while (builtins[i].name)
    {
        if (!strcmp(builtins[i].name, cmd))
            return (1);
        i++;
    }
    return (0);
}

int exec_builtin(t_all *all, t_cmd *cmd)
{
    int i;

    i = 0;
    while (builtins[i].name)
    {
        if (!strcmp(builtins[i].name, cmd->args[0]))
            return (builtins[i].func(all, cmd));
        i++;
    }
    return (1);
}