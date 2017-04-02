/*
** game_loop.c for lemipc in /home/scutar_n/rendu/PSU/PSU_2016_lemipc/src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Apr  2 16:00:57 2017 Nathan Scutari
** Last update Sun Apr  2 21:41:05 2017 Nathan Scutari
*/

#include "lemi.h"

int	play_game_main(t_shared *ids, t_player *list, char *map, int team)
{
  t_player	*tmp;
  int		turn;

  tmp = NULL;
  turn = 0;
  while (1)
    {
      tmp = next_player(list, tmp);
      if (tmp->player_id != 1)
	player_turn(tmp->player_id, ids, list);
      else
	{
	  if (tmp->alive)
	    main_turn(ids, team, map);
	  ++turn;
	}
      usleep(75000);
      look_for_dead(map, tmp->player_team, list, ids);
      print_game_state(map, turn);
      if (look_for_end(map, list, ids))
	return (0);
      if (check_signal(ids))
	return (1);
    }
  return (0);
}

int	play_game(t_shared *ids, int id, int team, char *map)
{
  int	alive;
  t_msg	msg;

  alive = 1;
  while (alive)
    {
      if (msgrcv(ids->msg_id, &msg, sizeof(msg) - sizeof(long), id, 0) == -1)
	exit(1);
      if (msg.kill)
	check_if_dead(team, ids, map, &alive);
      else if (msg.end)
	return (0);
      else
	{
	  access_map(team, ids, map);
	  msg.mtype = 1;
	  msg.source_id = id;
	  msg.new_player = 0;
	  msg.team = team;
	  msgsnd(ids->msg_id, &msg, sizeof(msg) - sizeof(long), 0);
	}
      if (check_signal(ids))
	return (1);
    }
  return (0);
}

int	player_turn(int id, t_shared *ids, t_player *list)
{
  t_msg		msg;

  msg.source_id = 1;
  msg.mtype = id;
  msg.kill = 0;
  msg.end = 0;
  if (msgsnd(ids->msg_id, &msg, sizeof(msg) - sizeof(long), 0) == -1)
    exit(1);
  while (get_msg(ids->msg_id, &list));
  return (0);
}

int	main_turn(t_shared *ids, int team, char *map)
{
  struct sembuf		sops;

  sops.sem_num = 0;
  sops.sem_flg = 0;
  sops.sem_op = -1;
  semop(ids->sem_id, &sops, 1);
  move_in_map(map, team, ids);
  sops.sem_op = 1;
  semctl(ids->sem_id, 0, SETVAL, 0);
  semop(ids->sem_id, &sops, 1);
  return (0);
}

t_player	*next_player(t_player *list, t_player *tmp)
{
  if (tmp == NULL || tmp->next == NULL)
    return (list);
  tmp = tmp->next;
  while (tmp && tmp->alive == 0)
    tmp = tmp->next;
  if (tmp == NULL)
    return (list);
  return (tmp);
}
