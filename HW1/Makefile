CC = gcc
LN = gcc
All : prob1a prob1b prob2a prob2b
prob1a:

prob1b:

prob2a:

prob2b:
	@echo "****** Problem 2.b ******"
	g++ -c hw1_helper.cpp
	g++ -c p2_oilpainting.cpp
	g++ -o p2_oilpainting p2_oilpainting.o hw1_helper.o
	@echo "USAGE: ./p2_oilpainting [inputImageName] [outputImageName] 3 [width] [height]"
	./p2_oilpainting barn.raw p2_oilpainting_barn_512.raw 3 380 275



clean:
	rm -f *.o p1_resize p1_bilinear p1_MHC p2_transfer p2_cumulative



