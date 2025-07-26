#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexer.h"

static void print_export_list(t_all *all)
{
    t_env *env;

    env = all->env;
    while (env)
    {
        printf("declare -x %s=\"%s\"\n", env->key, env->value);
        env = env->next;
    }
}

static int is_valid_identifier(char *str)
{
    int i;

    if (!str || !*str || (!ft_isalpha(*str) && *str != '_'))
        return (0);
    i = 1;
    while (str[i])
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

static void add_or_update_env(t_all *all, char *key, char *value)
{
    t_env *env;
    t_env *new_env;

    env = all->env;
    while (env)
    {
        if (strcmp(env->key, key) == 0)
        {
            free(env->value);
            env->value = strdup(value);
            return;
        }
        env = env->next;
    }

    new_env = safe_malloc(all->collector ,sizeof(t_env));
    if (!new_env)
        return;
    new_env->key = strdup(key);
    new_env->value = strdup(value);
    new_env->next = all->env;
    all->env = new_env;
}

int ft_export(t_all *all, t_cmd *cmd)
{
    char *equal_sign;
    char *key;
    char *value;
    int i;

    if (!cmd->args[1])
    {
        print_export_list(all);
        return (0);
    }

    i = 1;
    while (cmd->args[i])
    {
        equal_sign = strchr(cmd->args[i], '=');
        if (!equal_sign)
        {
            if (!is_valid_identifier(cmd->args[i]))
            {
                printf("minishell: export: `%s': not a valid identifier\n", cmd->args[i]);
                return (1);
            }
        }
        else
        {
            key = strndup(cmd->args[i], equal_sign - cmd->args[i]);
            value = strdup(equal_sign + 1);
            
            if (!is_valid_identifier(key))
            {
                printf("minishell: export: `%s': not a valid identifier\n", key);
                free(key);
                free(value);
                return (1);
            }
            
            add_or_update_env(all, key, value);
            free(key);
            free(value);
        }
        i++;
    }
    return (0);
}
