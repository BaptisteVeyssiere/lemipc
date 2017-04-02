/*
** main_process_ctl.c for lemipc in /home/scutar_n/rendu/PSU/PSU_2016_lemipc/src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Apr  2 16:18:26 2017 Nathan Scutari
** Last update Sun Apr  2 16:20:29 2017 Nathan Scutari
*/

#include "lemi.h"

void	print_border(int size)
{
  int	i;

  i = -1;
  while (++i < size)
    printf("*");
  printf("\n");
}

void	print_game_state(char *map, int turn)
{
  int	y;
  int	i;

  y = -1;
  print_border(52);
  while (++y < 30)
    {
      i = -1;
      printf("*");
      while (++i < 50)
	{
	  if (map[i + 50 * y] != 0)
	    {
	      printf("%c", map[i + 50 * y]);
	    }
	  else
	    printf(" ");
	}
      printf("*\n");
    }
  print_border(52);
  printf("Turn: %d\n", turn);
}

int	get_msg(int msg_id, t_player **list)
{
  int	id;
  t_msg	msg;

  if (msgrcv(msg_id, &msg, sizeof(msg) - sizeof(long), 1, 0) == -1)
    exit(1);
  if (msg.new_player)
    {
      id = find_id(*list);
      msg.mtype = 42;
      msg.source_id = id;
      if (msgsnd(msg_id, &msg, sizeof(msg) - sizeof(long), 0) == -1)
	exit(1);
      register_player(list, &msg);
      return (1);
    }
  return (0);
}

void	death_message(t_player *list, t_shared *ids, int *team)
{
  t_msg	msg;
  int	id;

  id = list->player_id;
  *team = list->player_team;
  msg.mtype = id;
  msg.source_id = 1;
  msg.team = 0;
  msg.new_player = 0;
  msg.kill = 0;
  msg.end = 1;
  msgsnd(ids->msg_id, &msg, sizeof(msg) - sizeof(long), 0);
}
