/*
** errors.c for lemi in /home/scutar_n/rendu/PSU/PSU_2016_lemipc/src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Apr  2 20:18:08 2017 Nathan Scutari
** Last update Sun Apr  2 20:25:46 2017 Nathan Scutari
*/

#include "lemi.h"

int	print_err(char *str, int ret_value)
{
  fprintf(stderr, "%s", str);
  return (ret_value);
}
