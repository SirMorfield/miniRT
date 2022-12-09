
NAME      		= miniRT

CC          	= clang++
CFLAGS      	= -Wall -Wextra  -pedantic -std=c++17 -Wshadow
CFLAGS      	+= -O3
# CFLAGS			+= -O0 -g3 -fsanitize=address

SRCEXT      	= cpp
SRCDIR      	= src
OBJEXT      	= o
HEADEREXT		= hpp
HEADERDIR		= include
BUILDDIR    	= obj

LIBS			=
INCLUDES		= -I$(HEADERDIR)
LINK			= -lpthread

OBJ_IN_DIR 		= $(BUILDDIR)/*.$(OBJEXT)
SRC 			= $(wildcard $(SRCDIR)/*.$(SRCEXT))
HEADERS 		= $(wildcard $(HEADERDIR)/*.$(HEADEREXT))
OBJ				= $(foreach src,$(SRC),$(BUILDDIR)/$(notdir $(src:.$(SRCEXT)=.$(OBJEXT))))

SILECE_MAKE 	= | grep -v -E ".*Leaving directory|.*Entering directory"
VPATH 			= $(shell find $(SRCDIR) -type d | tr '\n' ':' | sed -E 's/(.*):/\1/')
.SUFFIXES:

ifeq ($(uname -s),Linux)
	NCPU = $(shell nproc --all) $(NAME)
else
	NCPU = $(shell sysctl -n hw.ncpu) $(NAME)
endif
MAKEFLAGS+="j $(NCPU)"

all: $(NAME)

$(NAME): $(BUILDDIR)/ $(OBJ) $(HEADERS)
	@$(CC) $(CFLAGS) $(INCLUDES) $(OBJ) $(LIBS) -o $(NAME) $(LINK)
	@echo "$(NAME) compiled"

$(BUILDDIR)/%.$(OBJEXT): %.$(SRCEXT) $(HEADERS)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $(BUILDDIR)/$(notdir $@)
	@printf '\r\033[1A\033[0K$@ Compiled $(BUILDDIR)/$(notdir $@)\n'

.PHONY: test
test: $(NAME)
	@./$(NAME) rt/dragon.rt

clean:
ifneq ($(BUILDDIR),.)
	/bin/rm -rf $(BUILDDIR)/
endif

fclean: | clean
	/bin/rm -f $(NAME)

re: | fclean all

$(BUILDDIR)/:
	mkdir -p $(BUILDDIR)

silent:
	@$(MAKE) > /dev/null

.PHONY: all clean fclean re silent
