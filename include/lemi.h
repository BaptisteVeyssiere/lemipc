/*
** lemi.h for lemipc in /home/scutar_n/rendu/PSU/PSU_2016_lemipc
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Fri Mar 31 15:44:34 2017 Nathan Scutari
** Last update Sun Apr  2 16:52:52 2017 Nathan Scutari
*/

#ifndef LEMI_H_
# define LEMI_H_

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

/*
** dead_check.c
*/
int	send_kill_message(t_player *, t_shared *);
int	kill_player(int, t_player *, t_shared *, char *);
void	look_for_dead(char *, int, t_player *, t_shared *);
int	is_player_dead(char *, int, int);
void	check_if_dead(int, t_shared *, char *, int *);


/*
** game_start_end.c
*/
int	enough_player(t_player *);
int	look_for_end(char *, t_player *, t_shared *);

/*
** main.c
*/
int	init_map(char **, t_shared *, struct sembuf *);
int	access_map(int, t_shared *, char *);
void	modify_map(int, char *, t_shared *);

/*
** new_player_tools.c
*/
int	find_id(t_player *);
int	get_team(char *);
int	team_on_the_map(char *, int);
int	place_position(char *, int);
int	register_player(t_player **, t_msg *);

/*
** distance_tools.c
*/
int	distance_between_pos(int, int);
int	get_distance(int, int, int, int);
int	find_nearest_ally(char *, int, t_shared *);
int	find_nearest_ennemy(char *, int, t_shared *);

/*
** ia_choice.c
*/
int	move_in_map(char *, int, t_shared *);
void	attack_ennemy(char *, int, t_shared *);
void	observe_move(char *, t_shared *, int);
void	move_to_pos(char *, t_shared *, int, int);
void	get_closer_to_pos(char *, t_shared *, int, int);

/*
** main_process_ctl.c
*/
void	print_border(int);
void	print_game_state(char *, int);
int	get_msg(int, t_player **);
void	death_message(t_player *, t_shared *, int *);

/*
** signal_catch.c
*/
int	check_signal(t_shared *);
void	clean_ipcs(t_shared *);

/*
** game_loop.c
*/
int		play_game_main(t_shared *, t_player *, char *, int);
int		play_game(t_shared *, int, int, char *);
int		player_turn(int, t_shared *, t_player *);
int		main_turn(t_shared *, int, char *);
t_player	*next_player(t_player *, t_player *);

/*
** init_tools.c
*/
t_player	*init(t_shared *, key_t, char **, char **);
int		init_player(t_shared *, key_t, char **, char **);
int		init_signal(t_shared *);
int		main_process(key_t, char **);
int		add_player(key_t, char **);

/*
** map_position.c
*/
int	place_far_away(char *, int);
int	closest_ennemy(char *, int);
int	place_nearby(char *, int);

#endif /* !LEMI_H_ */
