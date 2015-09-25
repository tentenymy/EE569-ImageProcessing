CC = gcc
LN = gcc
All : prob1a prob1b prob2a prob2b prob3a prob3b
prob1a:
	@echo "****** Problem 1.a ******"
	@g++ -c hw1_helper.cpp
	@g++ -c p1_resize.cpp
	@g++ -o p1_resize p1_resize.o hw1_helper.o
	@echo "USAGE: ./p1_resize [inputImageName] [outputImageName] 3 512 512"
	./p1_resize the_starry_night.raw p1_resize.raw 3 512 512

prob1b:
	@echo "****** Problem 1.b ******"
	g++ -c hw1_helper.cpp
	g++ -c p1_bilinear.cpp
	g++ -c p1_MHC.cpp
	g++ -o p1_bilinear p1_bilinear.o hw1_helper.o
	g++ -o p1_MHC p1_MHC.o hw1_helper.o
	@echo "USAGE: ./p1_bilinear [inputImageName] [outputImageName] 1 [width] [height]"
	@echo "USAGE: ./p1_MHC [inputImageName] [outputImageName] 1 [width] [height]"
	./p1_bilinear parrot_CFA.raw p1_bilinear.raw 1 424 636
	./p1_MHC parrot_CFA.raw p1_MHC1.raw 1 424 636

prob2a:
	@echo "****** Problem 1.b ******"
	g++ -c hw1_helper.cpp
	g++ -c p1_bilinear.cpp
	g++ -c p1_MHC.cpp
	g++ -o p1_bilinear p1_bilinear.o hw1_helper.o
	g++ -o p1_MHC p1_MHC.o hw1_helper.o
	@echo "USAGE: ./p1_bilinear [inputImageName] [outputImageName] 1 [width] [height]"
	@echo "USAGE: ./p1_MHC [inputImageName] [outputImageName] 1 [width] [height]"
	./p1_bilinear parrot_CFA.raw p1_bilinear.raw 1 424 636
	./p1_MHC parrot_CFA.raw p1_MHC1.raw 1 424 636

prob2b:
	@echo "****** Problem 2.b ******"
	g++ -c hw1_helper.cpp
	g++ -c p2_oilpainting.cpp
	g++ -o p2_oilpainting p2_oilpainting.o hw1_helper.o
	@echo "USAGE: ./p2_oilpainting [inputImageName] [outputImageName] 3 [width] [height]"
	./p2_oilpainting barn.raw p2_oilpainting_barn_512.raw 3 380 275

prob3a:
	@echo "****** Problem 3.a ******"
	g++ -c hw1_helper.cpp
	g++ -c p3_gaussian.cpp
	g++ -o p3_gaussian p3_gaussian.o hw1_helper.o
	@echo "USAGE: ./p3_gaussian [inputImageName] [outputImageName] 3 [width] [height]"
	./p3_gaussian peppers_noisy.raw p3_gaussian.raw 3 512 512

prob3b:
	@echo "****** Problem 3.b ******"
	g++ -c hw1_helper.cpp
	g++ -c p3_guided.cpp
	g++ -o p3_guided p3_guided.o hw1_helper.o
	@echo "USAGE: ./p3_guided [inputImageName] [outputImageName] 3 [width] [height]"
	./p3_guided peppers_noisy.raw p3_guided.raw 3 512 512



clean:
	rm -f *.o p1_resize p1_bilinear p1_MHC p2_transfer p2_cumulative p2_oilpainting p3_gaussian p3_guided



