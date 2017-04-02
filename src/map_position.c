/*
** map_position.c for lemipc in /home/scutar_n/rendu/PSU/PSU_2016_lemipc
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Apr  2 15:20:34 2017 Nathan Scutari
** Last update Sun Apr  2 15:40:40 2017 Nathan Scutari
*/

#include "lemi.h"

int	place_far_away(char *map, int team)
{
  int	save;
  int	i;
  int	distance;
  int	value;

  distance = 0;
  i = -1;
  while (++i < 30 * 50)
    if ((value = closest_ennemy(map, i)) > distance)
      {
	distance = value;
	save = i;
      }
  map[save] = team + 48;
  return (save);
}

int	closest_ennemy(char *map, int position)
{
  int	distance;
  int	pos_x;
  int	pos_y;
  int	x;
  int	y;
  int	i;

  i = -1;
  pos_x = position % 50;
  pos_y = position / 50;
  distance = 500;
  while (++i < 30 * 50)
    {
      if (map[i] != 0)
	{
	  x = i % 50;
	  y = i / 50;
	  if (get_distance(x, y, pos_x, pos_y) < distance)
	    distance = get_distance(x, y, pos_x, pos_y);
	}
    }
  return (distance);
}

int	place_nearby(char *map, int team)
{
  int	i;
  int	x;
  int	y;
  int	my_x;
  int	my_y;

  i = -1;
  while (++i < 30 * 50 && map[i] != team + 48);
  x = i % 50;
  y = i / 50;
  while (1)
    {
      my_x = (rand() % ((x + 5) - (x - 5))) + (x - 5);
      my_y = (rand() % ((y + 5) - (y - 5))) + (y - 5);
      if (my_x >= 0 && my_y >= 0 && my_x < 50 &&
	  my_y < 30 && map[my_x + my_y * 50] == 0)
	{
	  map[my_x + my_y * 50] = team + 48;
	  return (my_x + my_y * 50);
	}
    }
}
