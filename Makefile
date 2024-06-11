#· · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · ·#
#·                                                                                           ·#
#·                                        COLORS                                             ·#
#·                                                                                           ·#
#· · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · ·#

RESET   = \033[0m
BLACK   = \033[1;30m
RED     = \033[1;31m
GREEN   = \033[1;32m
YELLOW  = \033[1;33m
BLUE    = \033[1;34m
PURPLE  = \033[1;35m
CYAN    = \033[1;36m
WHITE   = \033[1;37m

#· · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · ·#
#·                                                                                           ·#
#·                                       COMMANDS                                            ·#
#·                                                                                           ·#
#· · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · ·#

CC      = cc
RM      = rm -rf
AR      = ar -rcs

#· · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · ·#
#·                                                                                           ·#
#·                                         FLAGS                                             ·#
#·                                                                                           ·#
#· · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · ·#

CFLAGS  = -g
INCLUDE = -I./includes
INCLUDE += -I/opt/homebrew/opt/readline/include


#· · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · ·#
#·                                                                                           ·#
#·                                        FOLDERS                                            ·#
#·                                                                                           ·#
#· · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · ·#

SRC_DIR = srcs
OBJ_DIR = obj
LIBFT   = libft
LIBFT_LIB = $(LIBFT)/libft.a
LIBS = -ltermcap -lncurses -lreadline
LIBS_MAC = -ltermcap -lncurses -L/opt/homebrew/opt/readline/lib -lreadline

#· · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · ·#
#·                                                                                           ·#
#·                                        FILES                                              ·#
#·                                                                                           ·#
#· · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · ·#

NAME    = minishell
SRC = $(shell find $(SRC_DIR) -name '*.c' -type f)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))


#· · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · ·#
#·                                                                                           ·#
#·                                         RULES                                             ·#
#·                                                                                           ·#
#· · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · ·#
OS := $(shell uname)

ifeq ($(OS), Darwin)
	LIBS_USED = $(LIBS_MAC)
else
	LIBS_USED = $(LIBS)
endif



all: $(NAME)
	clear
	@printf "\n"
	@echo $(G)"       _     _     _       _ _ "$(X)
	@echo $(G)" _____|_|___|_|___| |_ ___| | |"$(X)
	@echo $(G)"|     | |   | |_ -|   | -_| | |"$(X)
	@echo $(G)"|_|_|_|_|_|_|_|___|_|_|___|_|_|"$(X)
	@printf "\n\n"

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

commit:
	git add -A .
	git commit -m 'Automatic commit of successful build $(BUILDID)'

run: re
	@./$(NAME)

valgrind: re
	@clear
	@echo "[$(RED)looking for valgrind leaks$(RESET)] $(GREEN)$(RESET)"
	@sleep 1
	@valgrind -s --suppressions=readline.supp --leak-check=full --show-leak-kinds=all ./minishell

.PHONY: all clean fclean re
.SILENT:
