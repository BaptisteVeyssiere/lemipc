/*
** game_start_end.c for lemi in /home/scutar_n/rendu/PSU/PSU_2016_lemipc/src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Apr  2 16:22:51 2017 Nathan Scutari
** Last update Sun Apr  2 16:23:11 2017 Nathan Scutari
*/

#include "lemi.h"

int	enough_player(t_player	*list)
{
  t_player	*tmp;
  char		teams[10];
  int		i;
  int		valid_teams;

  bzero(teams, 10);
  if (!list)
    return (0);
  tmp = list;
  while (tmp)
    {
      teams[tmp->player_team] += 1;
      tmp = tmp->next;
    }
  i = -1;
  valid_teams = 0;
  while (++i < 10)
    {
      if (teams[i] >= 2)
	++valid_teams;
    }
  return ((valid_teams >= 2) ? 1 : 0);
}

int	look_for_end(char *map, t_player *list, t_shared *ids)
{
  int	i;
  int	teams;

  teams = -1;
  i = -1;
  while (++i < 30 * 50)
    {
      if (map[i] != 0 && teams == -1)
	teams = map[i] - 48;
      else if (map[i] != 0 && map[i] - 48 != teams)
	return (0);
    }
  while (list)
    {
      if (list->alive && list->player_id != 1)
	death_message(list, ids, &teams);
      list = list->next;
    }
  printf("The winner is team %d!\n", teams);
  clean_ipcs(ids);
  return (1);
}
