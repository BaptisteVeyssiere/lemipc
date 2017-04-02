##
## Makefile for malloc in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_malloc
##
## Made by Baptiste Veyssiere
## Login   <veyssi_b@epitech.net>
##
## Started on  Tue Jan 24 16:16:32 2017 Baptiste Veyssiere
##

NAME	= lemipc

RM	= rm -f

CC	= gcc

MKDIR	= mkdir -p

SRC	= src/main.c \
	src/init_tools.c \
	src/dead_check.c \
	src/game_start_end.c \
	src/new_player_tools.c \
	src/distance_tools.c \
	src/ia_choice.c \
	src/main_process_ctl.c \
	src/signal_catch.c \
	src/game_loop.c \
	src/map_position.c

SRCDIR	= src

OBJDIR	= obj

OBJ	= $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

CFLAGS	= -W -Werror -Wextra -Wall -I./include

$(NAME): $(OBJ)
	@$(CC) -o $(NAME) $(OBJ)
	@echo "Linking complete!"

$(OBJ): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(MKDIR) $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

all: $(NAME)

clean:
	@$(RM) $(OBJ)
	@echo "Cleanup complete!"

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
