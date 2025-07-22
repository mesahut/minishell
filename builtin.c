#include "lexer.h"
#include <unistd.h>
#include <string.h>

// Forward declare builtins array
static t_builtin builtins[];


void    print_env(t_env *env)
{
    t_env *current = env;

    while (current)
    {
        if (current->value) // env sadece value'su olanları gösterir
            printf("%s=%s\n", current->key, current->value);
        current = current->next;
    }
}

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

int ft_exit(t_all *all, t_cmd *cmd)
{
    (void)all; // Suppress unused parameter warning
    if (cmd->args[1])
    {
        int status = atoi(cmd->args[1]);
        exit(status);
    }
    printf("%s\n", "exit");
    exit(all->exit_status);
}
void add_or_update_env(t_all *all, const char *key, const char *value)
{
    t_env *current = all->env;
    while (current)
    {
        if (strcmp(current->key, key) == 0)
        {
            free(current->value);
            current->value = strdup(value);
            return;
        }
        current = current->next;
    }
    current = all->env;
    while (current->next)
        current = current->next;
    t_env *new_node = malloc(sizeof(t_env));
    new_node->key = strdup(key);
    new_node->value = strdup(value);
    new_node->next = NULL;
    current->next = new_node;
    print_env(all->env);
}

int ft_export(t_all *all, t_cmd *cmd)
{
    int i = 1; // cmd->args[0] = "export"

    while (cmd->args[i])
    {
        char *equal_sign = strchr(cmd->args[i], '=');
        if (equal_sign)
        {
            *equal_sign = '\0'; // key'nin sonunu işaretle
            char *key = cmd->args[i];
            char *value = equal_sign + 1;
            add_or_update_env(all, key, value);
            *equal_sign = '='; // Geri düzelt
        }
        else
        {
            fprintf(stderr, "export: `%s`: not a valid identifier\n", cmd->args[i]);
        }
        i++;
    }
    return 0;
}

int ft_unset(t_all *all, t_cmd *cmd)
{
    int i = 1; // cmd->args[0] = "unset"

    while (cmd->args[i])
    {
        t_env *prev = NULL;
        t_env *curr = all->env;

        while (curr)
        {
            if (strcmp(curr->key, cmd->args[i]) == 0)
            {
                if (prev)
                    prev->next = curr->next;
                else
                    all->env = curr->next;
                free(curr->key);
                free(curr->value);
                free(curr);
                break;
            }
            prev = curr;
            curr = curr->next;
        }
        i++;
    }
    return 0;
}
int ft_env(t_all *all, t_cmd *cmd)
{
    (void)cmd;
    t_env *current = all->env;

    while (current)
    {
        if (current->value) // env sadece value'su olanları gösterir
            printf("%s=%s\n", current->key, current->value);
        current = current->next;
    }
    return 0;
}


// Built-in komut tablosu - static so it's only visible in this file
static t_builtin builtins[] = {
    {"echo", ft_echo},
    {"cd", ft_cd},
    {"pwd", ft_pwd},
    {"export", ft_export},
    {"unset", ft_unset},
    {"env", ft_env},
    {"exit", ft_exit},
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