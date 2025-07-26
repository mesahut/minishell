#include "lexer.h"

void    create_env(char **env_line, t_all *all)
{
    t_env   *new_env;
    t_env   *current;

    new_env = (t_env *)safe_malloc(all->collector ,sizeof(t_env));

    new_env->key = env_line[0];
    new_env->value = env_line[1];
    new_env->next = NULL;
    if(all->env == NULL)
        all->env = new_env;
    else
    {
        current = all->env;
        while (current->next != NULL)
            current = current->next;
        current->next = new_env;
    }
}

void    put_env(t_all *all, char **env_list)
{
    int i;

    i = 0;
    while (env_list[i])
    {
        create_env(ft_split(env_list[i], '='), all);
        i++;
        //printf("Added env: %s=%s\n", env->key, env->value);
    }
}