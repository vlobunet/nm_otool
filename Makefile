NM_NAME = ft_nm

OTOOL_NAME = ft_otool

CC = gcc

CFLAGS = -Wall -Wextra -Werror -fsanitize=address,undefined

COMMON_SRC = print_info_1.c get_bytes.c print_error_attrlett.c \
			check_attributes.c main_check.c manager_x86_func.c \
			manager_x64_func.c section_character_table.c

NM_SRC = ft_nm.c sorted_func.c ${COMMON_SRC}

OTOOL_SRC = ft_otool.c ${COMMON_SRC}

SRCDIR = src

OBJDIR = objs

LIB = -Llibft/ -lft

INCLUDES = -Ilibft/includes/ -Iincludes/

DEP = includes/nm_otool.h libft/libft.a

### COLORS ###
NOC         = \033[0m
BOLD        = \033[1m
UNDERLINE   = \033[4m
BLACK       = \033[1;30m
RED         = \033[1;31m
GREEN       = \033[01;38;05;46m
YELLOW      = \033[38;05;226m
BLUE        = \033[03;38;05;21m
VIOLET      = \033[01;38;05;201m
CYAN        = \033[1;36m
WHITE       = \033[01;38;05;15m
RES			= \033[m
############################## RULES ###########################################

all: ${NM_NAME} ${OTOOL_NAME}


libft/%:
	@[[ -d libft ]] || (echo ${M}Cloning"   "[libft]...${X} && git clone https://github.com/vlobunet/libft &>/dev/null)
	@make -C libft

############################## NM ##############################################

NM_OBJ = $(addprefix ${OBJDIR}/, $(NM_SRC:.c=.o))

${NM_NAME}: ${NM_OBJ}
	@echo  "$(YELLOW) : OK$(RES)"
	@${CC} ${CFLAGS} ${INCLUDES} ${LIB} -o $@ ${NM_OBJ}


${OBJDIR}/%.o: ${SRCDIR}/%.c ${DEP}
	@/bin/mkdir -p ${OBJDIR}
	@${CC} ${CFLAGS} ${INCLUDES} -c -o $@ $<
	@echo  "$(VIOLET)█$(RES)\c)"

############################## OTOOL ###########################################

OTOOL_OBJ = $(addprefix ${OBJDIR}/, $(OTOOL_SRC:.c=.o))

${OTOOL_NAME}: ${OTOOL_OBJ}
	@echo  "$(YELLOW) : OK$(RES)"
	@${CC} ${CFLAGS} ${INCLUDES} ${LIB} -o $@ ${OTOOL_OBJ}

${OBJDIR}/%.o: ${SRCDIR}/%.c ${DEP}
	@/bin/mkdir -p ${OBJDIR}
	@${CC} ${CFLAGS} ${INCLUDES} -c -o $@ $<
	@echo  "$(VIOLET)█$(RES)\c)"

############################## GENERAL #########################################

clean:
	@echo objs: OK
	@make -C libft/ clean
	@/bin/rm -Rf ${OBJDIR}

fclean: clean
	@make -C libft/ fclean
	@echo [${NM_NAME}]: OK
	@/bin/rm -f ${NM_NAME}
	@/bin/rm -Rf ${NM_NAME}.dSYM
	@echo [${OTOOL_NAME}]: OK
	@/bin/rm -f ${OTOOL_NAME}
	@/bin/rm -Rf ${OTOOL_NAME}.dSYM
re: fclean all

build: libft ${NM_OBJ} ${OTOOL_OBJ}

############################## DECORATION ######################################

.PHONY: all clean fclean re