/*
** init_tools.c for lemipc in /home/scutar_n/rendu/PSU/PSU_2016_lemipc
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Apr  2 15:39:08 2017 Nathan Scutari
** Last update Mon Apr  3 11:34:17 2017 Nathan Scutari
*/

#include <stdlib.h>
#include <strings.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include "lemi.h"

t_player	*init(t_shared *ids, key_t key, char **map, char **av)
{
  struct sembuf		sops;
  t_player		*list;
  t_msg			msg;

  ids->key = key;
  if ((ids->shm_id = shmget(key, 30 * 50, SHM_R | SHM_W | IPC_CREAT)) == -1 ||
      (ids->msg_id = msgget(key, SHM_R | SHM_W | IPC_CREAT)) == -1 ||
      (ids->sem_id = semget(key, 1, SHM_R | SHM_W | IPC_CREAT)) == -1)
    return (NULL);
  msg.source_id = 1;
  list = NULL;
  if (init_map(map, ids, &sops) ||
      (msg.team = get_team(av[2])) == -1 ||
      register_player(&list, &msg))
    return (NULL);
  *map[0] = msg.team + 48;
  ids->my_p = 0;
  ids->try = 0;
  ids->agressivity = rand() % (10 + 1) + 1;
  ids->caution = rand() % (10 + 1) + 1;
  sops.sem_op = 1;
  semctl(ids->sem_id, 0, SETVAL, 0);
  semop(ids->sem_id, &sops, 1);
  return (list);
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
  ids->try = 0;
  sops.sem_num = 0;
  sops.sem_flg = 0;
  sops.sem_op = -1;
  semop(ids->sem_id, &sops, 1);
  pos = place_position(*map, team);
  ids->my_p = pos;
  sops.sem_op = 1;
  semctl(ids->sem_id, 0, SETVAL, 0);
  semop(ids->sem_id, &sops, 1);
  return (pos);
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
      if ((msgrcv(ids.msg_id, &msg, sizeof(msg) - sizeof(long), 1, 0)) == -1)
	return (1);
      msg.source_id = find_id(list);
      msg.mtype = 42;
      if (msg.source_id != -1 && (msg.source_id = check_team(&msg, list)) != -1)
	register_player(&list, &msg);
      msgsnd(ids.msg_id, &msg, sizeof(msg) - sizeof(long), 0);
    }
  play_game_main(&ids, list, map, team);
  shmctl(ids.shm_id, IPC_RMID, NULL);
  msgctl(ids.msg_id, IPC_RMID, NULL);
  return (0);
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
  if ((id = msg.source_id) == -1)
    return (print_err("Can not join the arena, the game master refused\n", 0));
  play_game(&ids, id, team, map);
  return (0);
}
