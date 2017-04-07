/*
** main.c for lemi in /home/scutar_n/rendu/PSU/PSU_2016_lemipc
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Wed Mar 29 13:54:51 2017 Nathan Scutari
** Last update Mon Apr  3 12:18:36 2017 Nathan Scutari
*/

#include "lemi.h"

int	access_map(int team, t_shared *ids, char *map)
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

void	modify_map(int pos, char *map, t_shared *ids)
{
  struct sembuf		sops;

  sops.sem_num = 0;
  sops.sem_flg = 0;
  sops.sem_op = -1;
  semop(ids->sem_id, &sops, 1);
  map[pos] = 0;
  sops.sem_op = 1;
  semctl(ids->sem_id, 0, SETVAL, 0);
  semop(ids->sem_id, &sops, 1);
}

int	init_map(char **map, t_shared *ids, struct sembuf *sops)
{
  semctl(ids->sem_id, 0, SETVAL, 1);
  sops->sem_num = 0;
  sops->sem_flg = 0;
  sops->sem_op = -1;
  semop(ids->sem_id, sops, 1);
  if ((*map = shmat(ids->shm_id, NULL, 0)) == NULL)
    return (1);
  bzero(*map, 30 * 50);
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
	{
	  printf("main\n");
	  main_process(key, av);
	}
      else
	{
	  printf("player\n");
	  add_player(key, av);
	}
    }
  return (0);
}
