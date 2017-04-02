/*
** dead_check.c for lemipc in /home/scutar_n/rendu/PSU/PSU_2016_lemipc/src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Apr  2 16:02:52 2017 Nathan Scutari
** Last update Sun Apr  2 16:06:20 2017 Nathan Scutari
*/

#include "lemi.h"

int	send_kill_message(t_player *list, t_shared *ids)
{
  t_msg	msg;

  msg.mtype = list->player_id;
  msg.source_id = 1;
  msg.kill = 1;
  msgsnd(ids->msg_id, &msg, sizeof(msg) - sizeof(long), 0);
  msgrcv(ids->msg_id, &msg, sizeof(msg) - sizeof(long), 41, 0);
  if (msg.kill == 1)
    list->alive = 0;
  return (0);
}

int	kill_player(int team, t_player *list, t_shared *ids, char *map)
{
  while (list != NULL)
    {
      if (list->player_team == team)
	{
	  if (list->player_id != 1 && list->alive)
	    send_kill_message(list, ids);
	  if (list->player_id == 1 && list->alive &&
	      is_player_dead(map, ids->my_p, 0))
	    {
	      list->alive = 0;
	      modify_map(ids->my_p, map, ids);
	    }
	}
      list = list->next;
    }
  return (0);
}

void	look_for_dead(char *map, int team_killing, t_player *list,
		      t_shared *ids)
{
  int	i;

  i = -1;
  while (++i < 30 * 50)
    {
      if (map[i] != 0 && map[i] - 48 != team_killing
	  && is_player_dead(map, i, team_killing))
	kill_player(map[i] - 48, list, ids, map);
    }
}

int	is_player_dead(char *map, int i, int team)
{
  int	y;
  int	x;
  char	teams[10];

  team = map[i] - 48;
  bzero(teams, 10);
  y = (i / 50) - 2;
  while (++y < (i / 50) + 2)
    {
      x = (i % 50) - 2;
      while (++x < (i % 50) + 2)
	{
	  if (x >= 0 && y >= 0 && x < 50 && y < 30 &&
	      map[x + y * 50] != 0 && map[x + y * 50] != team + 48)
	    teams[map[x + y * 50] - 48] += 1;
	}
    }
  i = -1;
  while (++i < 10)
    if (teams[i] >= 2)
      return (1);
  return (0);
}

void	check_if_dead(int team, t_shared *ids, char *map, int *alive)
{
  t_msg	msg;

  msg.mtype = 41;
  msg.source_id = 0;
  if (is_player_dead(map, ids->my_p, team))
    {
      msg.kill = 1;
      modify_map(ids->my_p, map, ids);
      *alive = 0;
    }
  else
    {
      *alive = 1;
      msg.kill = 0;
    }
  msgsnd(ids->msg_id, &msg, sizeof(msg) - sizeof(long), 0);
}
