##
## This file is part of upas.
## 
## upas is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
## 
## upas is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
##

CXX		=	gcc
NAME		=	test_upas

SRCS		=	main.cpp			\
			PortAudio.cpp			\
			Speex.cpp			\
			AudioStreamPortAudio.cpp	\
			AudioCodecSpeex.cpp		\
			AudioManager.cpp
OBJS		=	$(SRCS:.cpp=.o)

INCLUDES	=	-I ./include -I .

CXXFLAGS	+=	-W -Wall -Wextra
LDFLAGS		=	$(INCLUDES) -lportaudio -lspeex

RM		=	rm -rf

##
##

all		:	$(NAME)

$(NAME)		:	$(OBJS)
			$(CXX) $(OBJS) $(LDFLAGS) -o $(NAME)

.cpp.o		:
			$(CXX) -c $(INCLUDES) $(CXXFLAGS) $< -o $@

clean		:
			$(RM) $(OBJS)

fclean		:	clean
			$(RM) $(NAME)

re		:	fclean all

test		:	all
			./$(NAME)

.PHONY		:	all clean fclean re test
