# · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · ·#
#                                                                                           #
#                                        COLORS                                             #
#                                                                                           #
# · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · ·#

RESET   = \033[0m
BLACK   = \033[1;30m
RED     = \033[1;31m
GREEN   = \033[1;32m
YELLOW  = \033[1;33m
BLUE    = \033[1;34m
PURPLE  = \033[1;35m
CYAN    = \033[1;36m
WHITE   = \033[1;37m

# · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · ·#
#                                                                                           #
#                                       COMMANDS                                            #
#                                                                                           #
# · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · ·#

CC      = cc
RM      = rm -rf
AR      = ar -rcs

# · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · ·#
#                                                                                           #
#                                         FLAGS                                             #
#                                                                                           #
# · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · ·#

CFLAGS  = -g -Wall -Werror -Wextra
INCLUDE = -I./includes
INCLUDE += -I/opt/homebrew/opt/readline/include

# · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · ·#
#                                                                                           #
#                                        FOLDERS                                            #
#                                                                                           #
# · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · ·#

SRC_DIR = srcs
OBJ_DIR = obj
LIBFT   = libft
LIBFT_LIB = $(LIBFT)/libft.a
LIBS =  -lncurses -lreadline
LIBS_MAC = -ltermcap -lncurses -L/opt/homebrew/opt/readline/lib -lreadline

# · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · ·#
#                                                                                           #
#                                        FILES                                              #
#                                                                                           #
# · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · ·#

NAME    = minishell
SRC_FILES = builtins/cd.c builtins/echo.c builtins/env.c builtins/export.c \
		builtins/export_utils.c builtins/export_utils_2.c builtins/pwd.c \
		builtins/unset.c environment/environment.c \
		environment/environment_utils.c environment/environment_utils_2.c \
		execution/execution.c execution/execution_utils.c execution/redir_handler.c \
		execution/redir_handler_utils.c heredoc/heredoc.c heredoc/heredoc_utils.c \
		signals/signal.c utils/expander.c utils/expander_utils.c utils/expander_utils_1.c \
		utils/expander_utils_2.c utils/expander_utils_3.c utils/expander_utils_4.c utils/expander_utils_5.c \
		utils/free.c utils/split2.c utils/split2_utils.c utils/split2_utils_2.c \
		utils/token_utils.c utils/utils.c utils/utils1.c utils/utils2.c utils/utils3.c \
		utils/utils4.c utils/validations.c utils/validations_1.c \
		exit.c main.c parser.c parser_utils.c pipes.c redirections.c

SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))

OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · ·#
#                                                                                           #
#                                         RULES                                             #
#                                                                                           #
# · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · ·#
OS := $(shell uname)

ifeq ($(OS), Darwin)
	LIBS_USED = $(LIBS_MAC)
else
	LIBS_USED = $(LIBS)
endif

all: $(NAME)
#	@printf "\n"
#	@echo $(G)"       _     _     _       _ _ "$(X)
#	@echo $(G)" _____|_|___|_|___| |_ ___| | |"$(X)
#	@echo $(G)"|     | |   | |_ -|   | -_| | |"$(X)
#	@echo $(G)"|_|_|_|_|_|_|_|___|_|_|___|_|_|"$(X)
#	@printf "\n\n"

$(NAME): $(OBJ)
	@make -C $(LIBFT) --no-print-directory
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJ) $(LIBFT_LIB) $(LIBS_USED) -o $@
	@echo "$(GREEN)$(NAME)$(RESET) Created"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	@make clean -C $(LIBFT) --no-print-directory
	@$(RM) $(OBJ_DIR)
	@echo "$(YELLOW)$(NAME)$(RESET) Objects Deleted"

fclean: clean
	@make fclean -C $(LIBFT) --no-print-directory
	@$(RM) $(NAME)
	@echo "$(RED)$(NAME)$(RESET) Deleted"

re: fclean all

BUILDID=$(shell date +%Y%m%d-%H:%M:%S)

commit: fclean
	git add -A .
	git commit -m 'Automatic commit of successful build $(BUILDID)'
	git push origin main

run: re
	@./$(NAME)

valgrind: re
	@clear
	@echo "[$(RED)looking for valgrind leaks$(RESET)] $(GREEN)$(RESET)"
	@sleep 1
	@valgrind -s --suppressions=readline.supp --leak-check=full --show-leak-kinds=all ./minishell

.PHONY: all clean fclean re
#.SILENT:
