# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aquincho <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/03 12:00:00 by aquincho          #+#    #+#              #
#    Updated: 2022/11/03 14:25:00 by aquincho         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= philo_bonus

CC			= gcc
CFLAGS		= -Wall -Werror -Wextra -g
CSANIFLAGS	=  -fsanitize=leak -fsanitize=address #-fsanitize=thread
RM			= rm -f
RM_PATH		= rm -rf

HEAD_PATH	= ./includes/

SRCS_PATH		= ./srcs/
SRCS_FUNCTIONS	= philo_bonus ft_init ft_init_philos ft_supervisor \
ft_philo_life ft_actions ft_forks ft_msg ft_time ft_errors ft_free \
ft_utils
SRCS_FILES		= $(addsuffix .c, $(SRCS_FUNCTIONS))
SRCS = $(addprefix $(SRCS_PATH), $(addsuffix .c, $(SRCS_FUNCTIONS)))

HEAD_FLAGS		= -I $(HEAD_PATH)
LIBS_FLAGS		=  -lpthread -pthread

OBJS_PATH	= ./objs/
OBJS_FILES	= $(SRCS_FILES:.c=.o)
OBJS		= $(addprefix $(OBJS_PATH), $(OBJS_FILES))

$(OBJS_PATH)%.o: $(SRCS_PATH)%.c
	@mkdir -p $(OBJS_PATH)
	@$(CC) $(CFLAGS) $(HEAD_FLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LIBS_FLAGS) $(OBJS) -o $(NAME)
	@echo "\033[33;32m=== Compilation philo \t\t\t\tDONE\e[0m"

all: $(NAME)

sanitize: $(NAME)
	$(CC) $(CFLAGS) $(CSANIFLAGS) $(LIBS_FLAGS) $(OBJS) -o $(NAME)
	@echo "\033[33;32m=== Compilation philo with sanitize option \t\t\t\tDONE\e[0m"

clean:
	$(RM_PATH) $(OBJS_PATH)
	@echo "\033[33;32m=== philo object files deleted \t\t\t\tDONE\e[0m"

fclean:	clean
	$(RM) $(NAME)
	@echo "\033[33;32m=== philo bin file deleted \t\t\tDONE\e[0m"

re: fclean all

.PHONY: clean fclean re all
