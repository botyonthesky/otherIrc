# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: botyonthesky <botyonthesky@student.42.f    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/17 17:22:21 by botyonthesk       #+#    #+#              #
#    Updated: 2024/08/01 17:53:24 by botyonthesk      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 			:= ircClient

CXX				:= g++
CXXFLAGS 		:= -std=c++98 -g -Wall -Wextra -Werror
CPPFLAGS		+= -I./include
CPPFLAGS		+= -MMD -MP -MF $(@:.o=.d)

SRC_DIR			:= ./src
OBJ_DIR			:= ./obj

ROOT_SRC		:= 	client.cpp \
					utils.cpp \
					main.cpp 

ROOT_DIR		:= $(SRC_DIR)
ROOT_OBJ		:= $(addprefix $(OBJ_DIR)/, $(ROOT_SRC:.cpp=.o))

SRC				:= $(ROOT_SRC)

OBJ				:= $(ROOT_OBJ)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
				@mkdir -p $(@D)
				@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

all:			$(NAME)

$(NAME):		$(OBJ)
				@echo "$(YELLOW) Compiling $(NAME) $(CXX) $(CXXFLAGS) ...$(END)"
				@echo "$(GREEN) $(NAME) compiled successfully.$(END)"
				@echo "$(YELLOW) Building $(NAME) ...$(END)" 
				@$(CXX) $(OBJ) -o $(NAME)
				@echo "$(GREEN) $(NAME) built successfully. $(END)"

clean:
				@echo "$(YELLOW) Cleaning object files and dependencies...$(END)"
				@rm -rf $(OBJ_DIR)
				@echo "$(GREEN) Clean complete.$(END)"

fclean: 		clean
				@echo "$(YELLOW) Cleaning object files, dependencies and removing $(NAME)...$(END)"
				@rm -rf $(NAME)
				@echo "$(GREEN) Full clean complete.$(END)"

re: 			fclean
				@$(MAKE) --no-print-directory all

.PHONY:			all clean fclean re 

-include $(OBJ:.o=.d)
YELLOW=\033[0;33m
GREEN=\033[0;32m
END=\033[0m
