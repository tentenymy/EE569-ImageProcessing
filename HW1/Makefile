CC = gcc
LN = gcc
All : prob1a prob1b prob2a prob2b prob3a
prob1a:

prob1b:

prob2a:

prob2b:

prob3a:
	@echo "****** Problem 3.a ******"
	g++ -c hw1_helper.cpp
	g++ -c p3_gaussian.cpp
	g++ -o p3_gaussian p3_gaussian.o hw1_helper.o
	@echo "USAGE: ./p3_gaussian [inputImageName] [outputImageName] 3 [width] [height]"
	./p3_gaussian peppers_noisy.raw p3_gaussian.raw 3 512 512



clean:
	rm -f *.o p1_resize p1_bilinear p1_MHC p2_transfer p2_cumulative p2_oilpainting p3_gaussian



