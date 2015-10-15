% EE569 Homework Assignment #2 
% Date: October. 11th 2015 
% Name: Meiyi Yang
% ID:  6761-0405-85 
% email:  meiyiyan@usc.edu
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Problem 3: Image Segmentation
% (c) Segmentation Result Evaluation
% M-file list:   p3c.m
%                run_evaluation.m
% Usage:         run p3c.m
% Function usage: run_evaluation(segmentation_data, groundtrue_data, row, col, FLAGl)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
clear all;
[curCOV, curPRI,curGCE,curVOI, curBDE] = run_evaluation('Man.mat', 'Man_bound_GT.raw', 481, 321, 1);
[curCOV, curPRI,curGCE,curVOI, curBDE]
[curCOV, curPRI,curGCE,curVOI, curBDE] = run_evaluation('Man.mat', 'Man_color_GT.raw', 481, 321, 2);
[curCOV, curPRI,curGCE,curVOI, curBDE]
[curCOV, curPRI,curGCE,curVOI, curBDE] = run_evaluation('Rhinos.mat', 'Rhinos_bound_GT.raw', 321, 481, 1);
[curCOV, curPRI,curGCE,curVOI, curBDE]
[curCOV, curPRI,curGCE,curVOI, curBDE] = run_evaluation('Rhinos.mat', 'Rhinos_color_GT.raw', 321, 481, 2);
[curCOV, curPRI,curGCE,curVOI, curBDE]
