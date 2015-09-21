CC = gcc
LN = gcc
All : prob1a prob1b prob2a prob2b prob3a prob3b
prob1a:

prob1b:

prob2a:

prob2b:

prob3a:
prob3b:
	@echo "****** Problem 3.a ******"
	g++ -c hw1_helper.cpp
	g++ -c p3_guided.cpp
	g++ -o p3_guided p3_guided.o hw1_helper.o
	@echo "USAGE: ./p3_guided [inputImageName] [outputImageName] 3 [width] [height]"
	./p3_guided peppers_noisy.raw p3_guided.raw 3 512 512



clean:
	rm -f *.o p1_resize p1_bilinear p1_MHC p2_transfer p2_cumulative p2_oilpainting p3_gaussian



