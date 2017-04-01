/*
** init.c for lemipc in /home/scutar_n/rendu/PSU/PSU_2016_lemipc
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Fri Mar 31 15:04:14 2017 Nathan Scutari
** Last update Sat Apr  1 21:16:13 2017 Nathan Scutari
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
  t_player		*list;
  struct sembuf		sops;
  t_msg			msg;

  ids->key = key;
  if ((ids->shm_id = shmget(key, 30 * 50, SHM_R | SHM_W | IPC_CREAT)) == -1 ||
      (ids->msg_id = msgget(key, SHM_R | SHM_W | IPC_CREAT)) == -1 ||
      (ids->sem_id = semget(key, 1, SHM_R | SHM_W | IPC_CREAT)) == -1)
    return (NULL);
  semctl(ids->sem_id, 0, SETVAL, 1);
  sops.sem_num = 0;
  sops.sem_flg = 0;
  sops.sem_op = -1;
  semop(ids->sem_id, &sops, 1);
  *map = shmat(ids->shm_id, NULL, 0);
  bzero(*map, 30 * 50);
  sops.sem_op = 1;
  msg.source_id = 1;
  list = NULL;
  if ((msg.team = get_team(av[2])) == -1 ||
      register_player(&list, &msg))
    return (NULL);
  *map[0] = msg.team + 48;
  ids->my_p = 0;
  semop(ids->sem_id, &sops, 1);
  return (list);
}
