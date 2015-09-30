All : prob1a

LINK = -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui

FLAGS = -ggdb -Wall -Wno-sign-compare

prob1a:
	@echo "****** Problem 1.a ******"
	g++ $(FLAGS) -c p1a.cpp $(LINK)
	g++ $(FLAGS) -o p1a p1a.o $(LINK)
	./p1a

clean:
	rm -f *.o p1a



