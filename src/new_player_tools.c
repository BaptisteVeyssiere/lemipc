/*
** new_player_tools.c for lemipc in /home/scutar_n/rendu/PSU/PSU_2016_lemipc
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Apr  2 15:17:42 2017 Nathan Scutari
** Last update Sun Apr  2 16:28:29 2017 Nathan Scutari
*/

#include "lemi.h"

int	find_id(t_player *player)
{
  t_player	*tmp;
  int		id;
  int		id_ok;

  id = 2;
  while (1)
    {
      id_ok = 1;
      tmp = player;
      while (tmp)
	{
	  if (id == tmp->player_id)
	    id_ok = 0;
	  tmp = tmp->next;
	}
      if (id_ok)
	return (id);
      ++id;
    }
  return (-1);
}

int	get_team(char *arg)
{
  int	i;
  int	team;

  i = -1;
  while (arg[++i])
    {
      if (arg[i] < '0' || arg[i] > '9')
	return (-1);
    }
  team = atoi(arg);
  if (team < 0 || team > 9)
    return (-1);
  return (team);
}

int	team_on_the_map(char *map, int team)
{
  int	i;

  i = -1;
  while (++i < 30 * 50)
    {
      if (map[i] == team + 48)
	return (1);
    }
  return (0);
}

int	place_position(char *map, int team)
{
  if (team_on_the_map(map, team))
    return (place_nearby(map, team));
  else
    return (place_far_away(map, team));
  return (0);
}

int	register_player(t_player **player, t_msg *msg)
{
  t_player	*elem;
  t_player	*tmp;

  if ((elem = malloc(sizeof(t_player))) == NULL)
    return (1);
  elem->next = NULL;
  elem->player_id = msg->source_id;
  elem->player_team = msg->team;
  elem->alive = 1;
  if (*player == NULL)
    *player = elem;
  else
    {
      tmp = *player;
      while (tmp && tmp->next)
	tmp = tmp->next;
      tmp->next = elem;
    }
  return (0);
}
