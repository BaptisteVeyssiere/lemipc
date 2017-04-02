/*
** distance_tools.c for lemipc in /home/scutar_n/rendu/PSU/PSU_2016_lemipc
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Apr  2 15:21:48 2017 Nathan Scutari
** Last update Sun Apr  2 15:38:23 2017 Nathan Scutari
*/

#include "lemi.h"

int	distance_between_pos(int pos_a, int pos_b)
{
  int	distance_x;
  int	distance_y;
  int	x_a;
  int	y_a;
  int	x_b;
  int	y_b;

  x_a = pos_a % 50;
  y_a = pos_a / 50;
  x_b = pos_b % 50;
  y_b = pos_b / 50;
  distance_x = x_a - x_b;
  distance_x *= (distance_x < 0) ? -1 : 1;
  distance_y = y_a - y_b;
  distance_y *= (distance_y < 0) ? -1 : 1;
  return (distance_x + distance_y);
}

int	get_distance(int x, int y, int pos_x, int pos_y)
{
  int	dist_x;
  int	dist_y;

  dist_x = (pos_x - x) < 0 ? (pos_x - x) * -1 : pos_x - x;
  dist_y = (pos_y - y) < 0 ? (pos_y - y) * -1 : pos_y - y;
  return (dist_x + dist_y);
}

int	find_nearest_ally(char *map, int team, t_shared *ids)
{
  int	save_pos;
  int	value;
  int	min_distance;
  int	i;

  i = -1;
  min_distance = 500;
  save_pos = -1;
  while (++i < 30 * 50)
    {
      if (map[i] == team + 48 && i != ids->my_p)
	if ((value = distance_between_pos(i, ids->my_p)) < min_distance)
	  {
	    save_pos = i;
	    min_distance = value;
	  }
    }
  return (save_pos);
}

int	find_nearest_ennemy(char *map, int team, t_shared *ids)
{
  int	save_pos;
  int	value;
  int	min_distance;
  int	i;

  i = -1;
  min_distance = 500;
  save_pos = -1;
  while (++i < 30 * 50)
    {
      if (map[i] != 0 && map[i] != team + 48 && i != ids->my_p)
	if ((value = distance_between_pos(i, ids->my_p)) < min_distance)
	  {
	    save_pos = i;
	    min_distance = value;
	  }
    }
  return (save_pos);
}
