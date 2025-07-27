#include "lexer.h"
#include <unistd.h>
#include <string.h>

// Forward declare builtins array
static t_builtin builtins[];

int cmp_env(const void *a, const void *b)
{
    const char *s1 = *(const char **)a;
    const char *s2 = *(const char **)b;
    return strcmp(s1, s2);
}
int env_list_size(t_env *env)
{
    int count = 0;
    while (env)
    {
        count++;
        env = env->next;
    }
    return count;
}

char **env_to_array(t_env *env_list)
{
    int     size = env_list_size(env_list);
    char    **arr = malloc(sizeof(char *) * (size + 1));
    int     i = 0;
    char    *tmp;

    if (!arr)
        return NULL;
    while (env_list)
    {
        if (env_list->value)
        {
            tmp = malloc(strlen(env_list->key) + strlen(env_list->value) + 2);
            sprintf(tmp, "%s=%s", env_list->key, env_list->value);
        }
        else
        {
            tmp = strdup(env_list->key);
        }
        arr[i++] = tmp;
        env_list = env_list->next;
    }
    arr[i] = NULL;
    return arr;
}
void print_sorted_env(t_env *env_list)
{
    int size = env_list_size(env_list);
    char **keys = malloc(sizeof(char *) * (size + 1));
    t_env *current = env_list;
    int i = 0;

    if (!keys)
        return;

    // Sadece key'leri kopyala
    while (current)
    {
        keys[i++] = strdup(current->key);
        current = current->next;
    }
    keys[i] = NULL;

    // Alfabetik sırala
    qsort(keys, size, sizeof(char *), cmp_env);

    // Sıralı olarak yazdır
    for (int j = 0; j < size; j++)
    {
        t_env *node = env_list;
        while (node)
        {
            if (strcmp(node->key, keys[j]) == 0)
            {
                if (node->value)
                    printf("declare -x %s=\"%s\"\n", node->key, node->value);
                else
                    printf("declare -x %s\n", node->key);
                break;
            }
            node = node->next;
        }
        free(keys[j]);
    }
    free(keys);
}



void print_env(t_all *all)
{
    t_env *current = all->env;
    while (current)
    {
        printf("%s=%s\n", current->key, current->value);
        current = current->next;
    }
}

int flag_check(char *arg)
{
    int i;

    i = 1;
    if(arg[0] != '-')
        return (0);
    while (arg[i])
    {
        if (arg[i] != 'n')
            return (0);
        i++;
    }
    return (1);
}
int ft_echo(t_all *all, t_cmd *cmd)
{
    (void)all;
    int i = 1;
    int newline = 1;

    if (!cmd->args || !cmd->args[0])
        return (printf("\n"), 0);

    // NULL kontrolü eklendi:
    if (!cmd->args[1])
        return (printf("\n"), 0);

    if (cmd->args[i] && strcmp(cmd->args[i], "-n") == 0)
    {
        newline = 0;
        i++;
    }

    while (cmd->args[i])
    {
        // Redirect token'larını görmezden gel (güvenli yaklaşım)
        if (strcmp(cmd->args[i], "<") == 0 || strcmp(cmd->args[i], ">") == 0)
            break;

        printf("%s", cmd->args[i]);

        if (cmd->args[i + 1] &&
            strcmp(cmd->args[i + 1], "<") != 0 &&
            strcmp(cmd->args[i + 1], ">") != 0)
            printf(" ");

        i++;
    }

    if (newline)
        printf("\n");

    return 0;
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

    t_env *new_node = safe_malloc(all->collector, sizeof(t_env));
    new_node->key = strdup(key);
    new_node->value = strdup(value);
    new_node->next = NULL;

    if (!all->env)
    {
        all->env = new_node;
        return;
    }

    // 4. Değilse sona ekle
    current = all->env;
    while (current->next)
        current = current->next;
    current->next = new_node;
}


int ft_export(t_all *all, t_cmd *cmd)
{
    int i = 1;

    if (cmd->args[1] == NULL)
    {
        print_sorted_env(all->env);
        return 0;
    }

    while (cmd->args[i])
    {
        char *equal_sign = strchr(cmd->args[i], '=');
        if (equal_sign && equal_sign != cmd->args[i])
        {
            char *arg_copy = ft_strdup(cmd->args[i]);
            if (!arg_copy)
                return 1; // malloc hatası

            char *equal_pos = strchr(arg_copy, '=');
            *equal_pos = '\0';
            char *key = arg_copy;
            char *value = equal_pos + 1;
            add_or_update_env(all, key, value);
            free(arg_copy);
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