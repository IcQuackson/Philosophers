# Compiler settings
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -pthread -fsanitize=thread

# Directories
SRCDIR = ./src
OBJDIR = ./objs

# Source Files
SRCS = $(SRCDIR)/main.c $(SRCDIR)/init.c $(SRCDIR)/utils.c $(SRCDIR)/philo_actions.c
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Targets
NAME = philo
#      n die eat slp [n_eat]
ARGS = 1 470 200 200

all: 	$(NAME)

bonus:	all

clean:
		rm -rf $(OBJDIR)

fclean:	clean
		rm -f $(NAME)

run:	all
		./$(NAME) $(ARGS)

re:		fclean all

lldb:	all
		lldb -- ./$(NAME) $(ARGS)

gdb:	all
		gdb --args $(NAME) $(ARGS)

valgrind: 	all
			valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) $(ARGS)

$(NAME): $(OBJS)
		$(CC) $(OBJS) $(CFLAGS) -o $(NAME)

%.o: %.c
	$(CC) -Wall -Wextra -Werror -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean fclean bonus run
