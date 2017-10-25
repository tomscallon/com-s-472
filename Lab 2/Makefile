OBJS = main.o Game.o Player.o HumanPlayer.o AlphaBetaPlayer.o
CCOM = g++
CFLAGS = -Wall -c -std=c++14 $(DEBUG)
LFLAGS = -Wall $(DEBUG)

make: compile clean

run:
	./Othello

compile: $(OBJS)
	$(CCOM) $(OBJS) $(LFLAGS) -o Othello

%.o: %.cpp
	$(CCOM) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o
