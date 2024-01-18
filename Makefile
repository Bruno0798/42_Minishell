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

CFLAGS  = #-Wall -Wextra -Werror
INCLUDE = -I./include

#· · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · ·#
#·                                                                                           ·#
#·                                        FOLDERS                                            ·#
#·                                                                                           ·#
#· · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · · ·#

SRC_DIR = srcs
OBJ_DIR = obj
LIBFT   = libft
LIBFT_LIB = $(LIBFT)/libft.a

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

all: $(NAME)

$(NAME): $(OBJ)
	@make -C $(LIBFT) --no-print-directory
	@$(CC) $(CFLAGS) $(INCLUDE) $(OBJ) $(LIBFT_LIB) -lreadline -o $@
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

.PHONY: all clean fclean re
.SILENT: