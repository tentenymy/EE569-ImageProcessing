clc;
clear all;
close all;

%% SIFT
Sift_Feature('Bus.jpg', 300, 400);
Sift_Feature('Sedan.jpg', 233, 400);

%% SURF
I=imread('Bus.jpg');
Options.verbose=false; % Set this option to true if you want to see more information
Ipts=OpenSurf(I,Options); % Get the Key Points
PaintSURF(I, Ipts);  % Draw points on the image

I2=imread('Sedan.jpg');
Options.verbose=false; % Set this option to true if you want to see more information
Ipts=OpenSurf(I2,Options); % Get the Key Points
PaintSURF(I2, Ipts);  % Draw points on the image