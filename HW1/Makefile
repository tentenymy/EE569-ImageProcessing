CC = gcc
LN = gcc
All : prob1a prob1b prob2a
prob1a:

prob1b:

prob2a:
	@echo "****** Problem 2.a ******"
	g++ -c hw1_helper.cpp
	g++ -c p2_transfer.cpp
	g++ -o p2_transfer p2_transfer.o hw1_helper.o
	@echo "USAGE: ./p2_transfer [inputImageName] [outputImageName] 3 [width] [height]"
	@echo "USAGE: ./p2_cumulative [inputImageName] [outputImageName] 3 [width] [height]"
	./p2_transfer jet.raw p2_transfer.raw 3 512 512
	./p2_cumulative jet.raw p2_cumulative.raw 3 512 512


clean:
	rm -f *.o p1_resize p1_bilinear p1_MHC p2_transfer p2_cumulative



