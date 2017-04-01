/*
** lemi.h for lemipc in /home/scutar_n/rendu/PSU/PSU_2016_lemipc
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Fri Mar 31 15:44:34 2017 Nathan Scutari
** Last update Sat Apr  1 23:05:33 2017 Nathan Scutari
*/

#ifndef LEMI_H_
# define LEMI_H_

typedef struct	s_msg
{
  long	mtype;
  int	source_id;
  int	team;
  int	new_player;
  int	kill;
  int	end;
}		t_msg;

typedef struct	s_player
{
  int			player_id;
  int			player_team;
  int			alive;

  struct s_player	*next;
}		t_player;

typedef struct	s_shared
{
  int		shm_id;
  int		msg_id;
  int		sem_id;
  int		agressivity;
  int		sfd;
  int		caution;
  int		my_p;
  key_t		key;
}		t_shared;

int	get_team(char *);
int	register_player(t_player **, t_msg *);
t_player	*init(t_shared *, key_t, char **, char **);
int		is_player_dead(char *, int, int);
void		observe_move(char *, t_shared *, int);
int		init_signal(t_shared *);
int		check_signal(t_shared *);

#endif /* !LEMI_H_ */