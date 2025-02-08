# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: victor <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/12 10:39:30 by victor            #+#    #+#              #
#    Updated: 2025/02/08 20:27:59 by victor           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

# List all your source files here
SRCS = src/philo.c src/philo_utils.c

# Set the object directory
OBJ_DIR = ./obj
# Replace .c with .o for object files
OBJ = $(SRCS:src/%.c=$(OBJ_DIR)/%.o)

# Include directories
INCLUDE = inc/philo.h 

# Compiler settings
CC = cc
CFLAGS = -Wall -Wextra -Werror -I inc -fsanitize=address -fsanitize=undefined -fsanitize=leak -g

# Linker flags (add any required libraries here)
LDFLAGS = -pthread

# Colors for output
GREEN_DARK = \033[0;32m    # Green
WHITE = \033[0m            # White
RED = \033[0;91m           # Red
RESET = \033[0m            # Reset

# Default target to build the project
all: libs $(NAME)

# Create the executable
$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS) $(LIBFT)
	@echo "$(GREEN_DARK)Executable $(NAME) created successfully!$(RESET)"

# Compile source files into object files
$(OBJ_DIR)/%.o: src/%.c $(INCLUDE) Makefile
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREEN_DARK)Compiled: $<$(RESET)"

# Clean up object files
clean:
	rm -rf $(OBJ_DIR)
	@echo "$(RED)Cleaned up object files.$(RESET)"

# Remove executable and cleaned files
fclean: clean
	rm -f $(NAME)
	@echo "$(RED)Executable $(NAME) removed.$(RESET)"

# Rebuild the project from scratch
re: fclean all

# Phony targets
.PHONY: all libs clean fclean re
