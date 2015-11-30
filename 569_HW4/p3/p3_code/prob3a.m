% EE569 Homework Assignment #4 
% Date: Noverber 29th 2015 
% Name: Meiyi Yang
% ID:  6761-0405-85 
% email:  meiyiyan@usc.edu
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Problem 3:    Salient Point Descriptors and Image Matching
% p3_code/prob3a.m      %  main script for problem3a
% p3_code/prob3b.m      %  main script for problem3b
% p3_code/prob3c.m      %  main script for problem3c
% p3_code/SIFT/         %  SIFT Source code
% p3_code/SURF/         % SURF Source code
% p3_code/BOW/          %  BOW Source code
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 

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