CC = gcc
LN = gcc
All : prob1a prob1b prob2a
prob1a:
	@echo "****** Problem 1.a ******"
	g++ -c hw1_helper.cpp
	g++ -c p1_resize.cpp
	g++ -o p1_resize p1_resize.o hw1_helper.o
	echo "USAGE: ./p1_resize [inputImageName] [outputImageName] 3 512 512"
	./p1_resize the_starry_night.raw p1_resize.raw 3 512 512
prob1b:
	@echo "****** Problem 1.b ******"
	g++ -c hw1_helper.cpp
	g++ -c p1_bilinear.cpp
	g++ -c p1_MHC.cpp
	g++ -o p1_bilinear p1_bilinear.o hw1_helper.o
	g++ -o p1_MHC p1_MHC.o hw1_helper.o
	echo "USAGE: ./p1_bilinear [inputImageName] [outputImageName] 1 [width] [height]"
	echo "USAGE: ./p1_MHC [inputImageName] [outputImageName] 1 [width] [height]"
	./p1_bilinear parrot_CFA.raw p1_bilinear.raw 1 424 636
	./p1_MHC parrot_CFA.raw p1_MHC.raw 1 424 636
prob2a:


clean:
	rm -f *.o p1_resize p1_bilinear p1_MHC



