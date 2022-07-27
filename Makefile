
NAME      		= miniRT

CC          	= clang++
CFLAGS      	= -Wall -Wextra  -pedantic -std=c++17 -Wshadow -O3
# CFLAGS      	= -Wall -Wextra -Werror -Wuninitialized -O3

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

all: $(NAME)
ifeq ($(uname -s),Linux)
	@$(MAKE) -j $(shell nproc --all) $(NAME)
else
	@$(MAKE) -j $(shell sysctl -n hw.ncpu) $(NAME)
endif

$(NAME): $(BUILDDIR)/ $(OBJ) $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJ_IN_DIR) $(LIBS) -o $(NAME) $(LINK)

$(BUILDDIR)/%.$(OBJEXT): %.$(SRCEXT) $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $(BUILDDIR)/$(notdir $@)
# sources


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
