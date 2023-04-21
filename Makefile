COMP=g++
VERSION=-std=c++11
FLAGS=-Wall -g
OBJ=main.o Network.o User.o

full: $(OBJ)
	make $^
	make Output
	make clean

%.o: %.cpp
	$(COMP) -c $< $(VERSION)
Output: 
	$(COMP) *.o -o output $(VERSION)
tmp: 
	$(COMP) main.cpp Network.cpp User.cpp -o output $(VERSION)

clean:
	rm *.o



runRec:
	echo "selfLoop"
	make $@
