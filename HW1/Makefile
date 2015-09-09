CC = gcc
LN = gcc
All : prob1a prob1b
prob1a:
	@echo "****** Problem 1.a ******"
	@g++ -c hw1_helper.cpp
	@g++ -c p1_resize.cpp
	@g++ -o p1_resize p1_resize.o hw1_helper.o
	@echo "USAGE: ./p1_resize [inputImageName] [outputImageName] 3 512 512"
	@./p1_resize the_starry_night.raw output_p1a.raw 3 512 512
prob1b:
	@echo "****** Problem 1.b ******"
	g++ -c hw1_helper.cpp
	g++ -c p1_demosaicing.cpp
	g++ -o p1_demosaicing p1_demosaicing.o hw1_helper.o
	@echo "USAGE: ./p1_demosaicing [inputImageName] [outputImageName] 1 [width] [height]"
	./p1_demosaicing parrot_CFA.raw output_p1b.raw 1 424 636

clean:
	rm -f *.o p1_resize p1_demosaicing



