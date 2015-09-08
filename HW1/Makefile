CC = gcc
LN = gcc

prob1:
	@echo "****** Problem 1.a ******"
	g++ -c hw1_helper.cpp
	g++ -c p1_resize.cpp
	g++ -o p1_resize p1_resize.o hw1_helper.o
	@echo "USAGE: ./p1_resize [inputImageName] [outputImageName] 3 512 512"
	./p1_resize the_starry_night.raw output_p1a.raw 3 512 512

clean:
	rm -f *.o p1_resize



