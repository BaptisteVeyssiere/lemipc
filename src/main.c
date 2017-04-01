/*
** main.c for lemi in /home/scutar_n/rendu/PSU/PSU_2016_lemipc
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Wed Mar 29 13:54:51 2017 Nathan Scutari
** Last update Sat Apr  1 23:08:32 2017 Nathan Scutari
*/

#include <unistd.h>
#include <poll.h>
#include <signal.h>
#include <sys/signalfd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <strings.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
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
	return (1);
    }
  team = atoi(arg);
  if (team < 0 || team > 9)
    return (-1);
  return (team);
}

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
    }
  else
    observe_move(map, ids, team);
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

void	observe_move(char *map, t_shared *ids, int team)
{
  int	move[4] = {1, -1, 50, -50};

  move_to_pos(map, ids, team, ids->my_p + move[rand() % 4]);
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

int	access_map(int team, t_shared *ids, char *map)
{
  struct sembuf		sops;

  sops.sem_num = 0;
  sops.sem_flg = 0;
  sops.sem_op = -1;
  semop(ids->sem_id, &sops, 1);
  move_in_map(map, team, ids);
  sops.sem_op = 1;
  semop(ids->sem_id, &sops, 1);
  return (0);
}

void	modify_map(int pos, char *map, t_shared *ids)
{
  struct sembuf		sops;

  sops.sem_num = 0;
  sops.sem_flg = 0;
  sops.sem_op = -1;
  semop(ids->sem_id, &sops, 1);
  map[pos] = 0;
  sops.sem_op = 1;
  semop(ids->sem_id, &sops, 1);
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

int	get_distance(int x, int y, int pos_x, int pos_y)
{
  int	dist_x;
  int	dist_y;

  dist_x = (pos_x - x) < 0 ? (pos_x - x) * -1 : pos_x - x;
  dist_y = (pos_y - y) < 0 ? (pos_y - y) * -1 : pos_y - y;
  return (dist_x + dist_y);
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

int	place_position(char *map, int team)
{
  if (team_on_the_map(map, team))
    return (place_nearby(map, team));
  else
    return (place_far_away(map, team));
  return (0);
}

int	init_player(t_shared *ids, key_t key, char **av, char **map)
{
  int	pos;
  int	team;
  struct sembuf		sops;

  if ((ids->shm_id = shmget(key, 30 * 50, SHM_R | SHM_W)) == -1 ||
      (ids->msg_id = msgget(key, SHM_R | SHM_W)) == -1 ||
      (ids->sem_id = semget(key, 1, SHM_R | SHM_W)) == -1)
    return (-1);
  *map = shmat(ids->shm_id, NULL, 0);
  if ((team = get_team(av[2])) == -1)
    return (-1);
  ids->agressivity = rand() % (10 + 1) + 1;
  ids->caution = rand() % (10 + 1) + 1;
  sops.sem_num = 0;
  sops.sem_flg = 0;
  sops.sem_op = -1;
  semop(ids->sem_id, &sops, 1);
  pos = place_position(*map, team);
  ids->my_p = pos;
  sops.sem_op = 1;
  semop(ids->sem_id, &sops, 1);
  return (pos);
}

int	add_player(key_t key, char **av)
{
  t_shared	ids;
  int		id;
  int		team;
  int		pos;
  t_msg		msg;
  char		*map;

  if ((pos = init_player(&ids, key, av, &map)) == -1 ||
      init_signal(&ids))
    return (1);
  team = get_team(av[2]);
  msg.mtype = 1;
  msg.source_id = 0;
  msg.team = team;
  msgsnd(ids.msg_id, &msg, sizeof(msg) - sizeof(long), 0);
  msgrcv(ids.msg_id, &msg, sizeof(msg) - sizeof(long), 42, 0);
  id = msg.source_id;
  play_game(&ids, id, team, map);
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
  semop(ids->sem_id, &sops, 1);
  return (0);
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

int	kill_player(int team, t_player *list, t_shared *ids, char *map)
{
  t_msg		msg;

  while (list != NULL)
    {
      if (list->player_team == team)
	{
	  if (list->player_id != 1 && list->alive)
	    {
	      msg.mtype = list->player_id;
	      msg.source_id = 1;
	      msg.kill = 1;
	      msgsnd(ids->msg_id, &msg, sizeof(msg) - sizeof(long), 0);
	      msgrcv(ids->msg_id, &msg, sizeof(msg) - sizeof(long), 41, 0);
	      if (msg.kill == 1)
		list->alive = 0;
	    }
	  if (list->player_id == 1 && list->alive)
	    {
	      if (is_player_dead(map, ids->my_p, 0))
		{
		  list->alive = 0;
		  modify_map(ids->my_p, map, ids);
		}
	    }
	}
      list = list->next;
    }
  return (0);
}

void	look_for_dead(char *map, int team_killing, t_player *list, t_shared *ids)
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

void	clean_ipcs(t_shared *ids)
{
  shmctl(ids->shm_id, IPC_RMID, NULL);
  msgctl(ids->msg_id, IPC_RMID, NULL);
  semctl(ids->sem_id, 0, IPC_RMID, NULL);
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

int	check_signal(t_shared *ids)
{
  struct signalfd_siginfo	si;
  struct pollfd			pfd;

  pfd.fd = ids->sfd;
  pfd.events = POLLIN | POLLERR | POLLHUP;
  if (poll(&pfd, 1, 0))
    {
      read(ids->sfd, &si, sizeof(si));
      if (si.ssi_signo == SIGINT)
	{
	  clean_ipcs(ids);
	  return (1);
	}
    }
  return (0);
}

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

int	init_signal(t_shared *ids)
{
  int		sfd;
  sigset_t	mask;

  sigemptyset(&mask);
  sigaddset(&mask, SIGINT);
  if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0 ||
      (sfd = signalfd(-1, &mask, 0)) < 0)
    return (1);
  ids->sfd = sfd;
  return (0);
}

int	main_process(key_t key, char **av)
{
  int			id;
  t_shared		ids;
  t_msg			msg;
  t_player		*list;
  char			*map;
  int			team;

  if ((list = init(&ids, key, &map, av)) == NULL ||
      init_signal(&ids))
    return (1);
  team = get_team(av[2]);
  while (enough_player(list) == 0)
    {
      msgrcv(ids.msg_id, &msg, sizeof(msg) - sizeof(long), 1, 0);
      id = find_id(list);
      msg.mtype = 42;
      msg.source_id = id;
      msgsnd(ids.msg_id, &msg, sizeof(msg) - sizeof(long), 0);
      register_player(&list, &msg);
    }
  play_game_main(&ids, list, map, team);
  shmctl(ids.shm_id, IPC_RMID, NULL);
  msgctl(ids.msg_id, IPC_RMID, NULL);
  return (0);
}

int	main(int ac, char **av)
{
  key_t	key;
  int	shm_id;

  if ((key = ftok(av[1], 0)) == -1)
    return (1);
  srand(time(NULL));
  shm_id = shmget(key, 30 * 50, SHM_R | SHM_W);
  if (ac >= 3)
    {
      if (shm_id == -1)
	main_process(key, av);
      else
	add_player(key, av);
    }
  return (0);
}