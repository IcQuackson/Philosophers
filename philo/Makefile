# Compiler settings
CC = cc
CFLAGS = -Wall -Wextra -Werror -g# -fsanitize=address

# Directories
SRCDIR = ./src
OBJDIR = ./objs

# Source Files

SRCS = $(SRCDIR)/main.c $(SRCDIR)/ft_atoi.c
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Targets
NAME = philo
ARGS = 5 1 1 1 1

all: 	$(NAME)

bonus:	all

clean:
		rm -f $(OBJDIR)/*.o

fclean:	clean
		rm $(NAME)

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
	$(CC) -Wall -Wextra -Werror -O3 -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean fclean bonus run
