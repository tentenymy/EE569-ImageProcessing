% EE569 Homework Assignment #2 
% Date: October. 11th 2015 
% Name: Meiyi Yang
% ID:  6761-0405-85 
% email:  meiyiyan@usc.edu
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Problem 3: Image Segmentation
% (b) Color Palettes Generation
% M-file list:   p3b.m
%                run_cpp.m
% Usage:         run p3b.m
% Function usage: run_cpp(filename, row, col)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
clear all;
clc;

[bound_segment, color_segment] = run_cpp('Man.raw', 481, 321);
save Man.mat bound_segment color_segment;
[bound_segment, color_segment] = run_cpp('Rhinos.raw', 321, 481);
save Rhinos.mat bound_segment color_segment;
