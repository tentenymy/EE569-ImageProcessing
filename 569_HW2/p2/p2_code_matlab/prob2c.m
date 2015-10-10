% EE569 Homework Assignment #2 
% Date: October. 11th 2015 
% Name: Meiyi Yang
% ID:  6761-0405-85 
% email:  meiyiyan@usc.edu
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Problem 2:    Edge Detector
% (c) Structured Edge
% M-file list:   detector_SE.m
%                prob2c.m
% Usage:         run prob2c.m
% Function usage: detector_SE(filename_read, filename, threshold)
% Output image:   Farm_SE_XX.raw, Cougar_SE_XX.raw
% Parameters:   threshold = 0.2:0.1:0.9
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
%% Farm.raw
subplot(3,3,1);
imshow(detector_SE('Farm.raw', 'Farm_SE_02.raw', 0.02));
title('Farm: threshold = 0.02');

subplot(3,3,2);
imshow(detector_SE('Farm.raw', 'Farm_SE_03.raw', 0.03));
title('Farm: threshold = 0.03');

subplot(3,3,3);
imshow(detector_SE('Farm.raw', 'Farm_SE_04.raw', 0.04));
title('Farm: threshold = 0.04');

subplot(3,3,4);
imshow(detector_SE('Farm.raw', 'Farm_SE_05.raw', 0.05));
title('Farm: threshold = 0.05');

subplot(3,3,5);
imshow(detector_SE('Farm.raw', 'Farm_SE_06.raw', 0.06));
title('Farm: threshold = 0.06');

subplot(3,3,6);
imshow(detector_SE('Farm.raw', 'Farm_SE_07.raw', 0.07));
title('Farm: threshold = 0.07');

subplot(3,3,7);
imshow(detector_SE('Farm.raw', 'Farm_SE_08.raw', 0.08));
title('Farm: threshold = 0.08');

subplot(3,3,8);
imshow(detector_SE('Farm.raw', 'Farm_SE_09.raw', 0.09));
title('Farm: threshold = 0.09');

subplot(3,3,9);
imshow(detector_SE('Farm.raw', 'Farm_SE_10.raw', 0.10));
title('Farm: threshold = 0.10');

%% Cougar.raw
figure;
subplot(3,3,1);
imshow(detector_SE('Cougar.raw', 'Cougar_SE_02.raw', 0.02));
title('Cougar: threshold = 0.02');

subplot(3,3,2);
imshow(detector_SE('Cougar.raw', 'Cougar_SE_03.raw', 0.03));
title('Cougar: threshold = 0.03');

subplot(3,3,3);
imshow(detector_SE('Cougar.raw', 'Cougar_SE_04.raw', 0.04));
title('Cougar: threshold = 0.04');

subplot(3,3,4);
imshow(detector_SE('Cougar.raw', 'Cougar_SE_05.raw', 0.05));
title('Cougar: threshold = 0.05');

subplot(3,3,5);
imshow(detector_SE('Cougar.raw', 'Cougar_SE_06.raw', 0.06));
title('Cougar: threshold = 0.06');

subplot(3,3,6);
imshow(detector_SE('Cougar.raw', 'Cougar_SE_07.raw', 0.07));
title('Cougar: threshold = 0.07');

subplot(3,3,7);
imshow(detector_SE('Cougar.raw', 'Cougar_SE_08.raw', 0.08));
title('Cougar: threshold = 0.08');

subplot(3,3,8);
imshow(detector_SE('Cougar.raw', 'Cougar_SE_09.raw', 0.09));
title('Cougar: threshold = 0.09');

subplot(3,3,9);
imshow(detector_SE('Cougar.raw', 'Cougar_SE_10.raw', 0.10));
title('Cougar: threshold = 0.10');