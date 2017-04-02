/*
** ia_choice.c for lemi in /home/scutar_n/rendu/PSU/PSU_2016_lemipc/src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Apr  2 16:14:00 2017 Nathan Scutari
** Last update Sun Apr  2 21:45:30 2017 Nathan Scutari
*/

#include "lemi.h"

int	move_in_map(char *map, int team, t_shared *ids)
{
  int	pos;

  if ((pos = find_nearest_ally(map, team, ids)) != -1)
    {
      if (distance_between_pos(ids->my_p, pos) >
	  15 - ids->caution)
	get_closer_to_pos(map, ids, team, pos);
      else
	attack_ennemy(map, team, ids);
    }
  else
    {
      ids->agressivity = 10;
      ids->caution = 0;
      attack_ennemy(map, team, ids);
    }
  return (0);
}

void	attack_ennemy(char *map, int team, t_shared *ids)
{
  int   agress_chance;
  int	observe_chance;
  int	agress_move;
  int	pos;

  pos = find_nearest_ennemy(map, team, ids);
  if (distance_between_pos(ids->my_p, pos) >
      15 - ids->agressivity)
    get_closer_to_pos(map, ids, team, pos);
  else
    {
      agress_chance = (ids->agressivity * 50) / 10;
      observe_chance = (ids->caution * 50) / 10;
      agress_move = 50 + agress_chance - observe_chance;
      if (rand() % (100 + 1) < agress_move)
	get_closer_to_pos(map, ids, team, pos);
      else
	observe_move(map, ids, team);
    }
}

void	observe_move(char *map, t_shared *ids, int team)
{
  int	move[4] = {1, -1, 50, -50};
  int	value;

  value = rand() % 4;
  if ((value == 0 && ids->my_p % 50 == 0) ||
      (value == 1 && ids->my_p % 50 == 49))
    return ;
  move_to_pos(map, ids, team, ids->my_p + move[rand() % 4]);
}

void	move_to_pos(char *map, t_shared *ids, int team, int to)
{
  int	x;
  int	y;

  x = to % 50;
  y = to / 50;
  if (x >= 0 && y >= 0 && x < 50 && y < 30 &&
      map[x + y * 50] == 0)
    {
      map[x + y * 50] = team + 48;
      map[ids->my_p] = 0;
      ids->my_p = to;
      ids->try = 0;
    }
  else if (ids->try < 20)
    {
      ids->try += 1;
      observe_move(map, ids, team);
    }
  else
    ids->try = 0;
}

void	get_closer_to_pos(char *map, t_shared *ids, int team, int pos)
{
  int	value_x;
  int	value_y;

  if ((value_x = ids->my_p % 50 - pos % 50) < 0)
    value_x *= -1;
  if ((value_y = ids->my_p / 50 - pos / 50) < 0)
    value_y *= -1;
  if ((value_x < value_y))
    {
      if (ids->my_p / 50 > pos / 50)
	move_to_pos(map, ids, team, ids->my_p - 50);
      else
	move_to_pos(map, ids, team, ids->my_p + 50);
    }
  else
    {
      if (ids->my_p % 50 > pos % 50)
	move_to_pos(map, ids, team, ids->my_p - 1);
      else
	move_to_pos(map, ids, team, ids->my_p + 1);
    }
}
