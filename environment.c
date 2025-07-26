#include "lexer.h"

void create_env(char **env_line, t_all *all)
{
    t_env *new_env;
    t_env *current;

    new_env = (t_env *)safe_malloc(all->collector, sizeof(t_env));
    new_env->key = strdup(env_line[0]);    // Duplicate key string
    if (env_line[1])
        new_env->value = strdup(env_line[1]);  // Duplicate value string (if exists)
    else
        new_env->value = NULL;
    new_env->next = NULL;

    if (all->env == NULL)
        all->env = new_env;
    else
    {
        current = all->env;
        while (current->next != NULL)
            current = current->next;
        current->next = new_env;
    }
}


void put_env(t_all *all, char **env_list)
{
    int i = 0;
    char *key;
    char *value;
    char *eq_pos;

    while (env_list[i])
    {
        eq_pos = ft_strchr(env_list[i], '=');
        if (eq_pos)
        {
            key = ft_substr(env_list[i], 0, eq_pos - env_list[i]);
            value = ft_strdup(eq_pos + 1);
        }
        else
        {
            key = ft_strdup(env_list[i]);
            value = NULL; // No value, maybe?
        }
        create_env((char *[3]){key, value, NULL}, all);
        free(key);
        free(value);
        i++;
    }
}
