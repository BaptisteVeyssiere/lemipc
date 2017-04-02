/*
** signal_catch.c for lemipc in /home/scutar_n/rendu/PSU/PSU_2016_lemipc/src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Apr  2 16:20:44 2017 Nathan Scutari
** Last update Sun Apr  2 16:21:20 2017 Nathan Scutari
*/

#include "lemi.h"

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

void	clean_ipcs(t_shared *ids)
{
  shmctl(ids->shm_id, IPC_RMID, NULL);
  msgctl(ids->msg_id, IPC_RMID, NULL);
  semctl(ids->sem_id, 0, IPC_RMID, NULL);
}
