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

%% parameter
% segmentation_data: be evaluated data(.mat): save in p3b, include
% color_segment, bound segment
% groundtrue_data: ground true raw image
% row: row of ground true image
% col: column of ground true image
% FLAG: 1 is for boundary evaluation, 2 is for color evaluation
%%
%A MATLAB Toolbox
%
%Compare segmentation results with the Berkeley benchmark using four indices
%1. Probabilistic Rand Index
%2. Variation of Information
%3. Global Consistency Error
%4. Boundary Displacement Error
%
%The first three measures are calculated by the function compare_segmentations.m
%The last boundary measure is calculated by compare_image_boundary_error.m
%
%Please notice that this batch file is written to specifically compare the
%lossy coding results with the human segmentations in the Berkeley segbench database.
%
%As a result, the segbench human prior is resized to match the testset results.
%
%Authors: John Wright, and Allen Y. Yang
%Contact: Allen Y. Yang <yang@eecs.berkeley.edu>
%
%(c) Copyright. University of California, Berkeley. 2007.
%
%Notice: The packages should NOT be used for any commercial purposes
%without direct consent of their author(s). The authors are not responsible for any potential property loss or damage caused directly or indirectly by the usage of the software.
 
function [curCOV, curPRI,curGCE,curVOI, curBDE] = run_evaluation(segmentation_data, groundtrue_data, row, col, FLAG)
%% load segmentation data
load(segmentation_data);
%% read ground true
byte = 3;
fin = fopen(groundtrue_data, 'r');
I_read = fread(fin, row * col * byte, 'uchar');
I_read = uint8(I_read);
I = ones(row, col, byte);
count = 0;
for i = 1:row
    for j = 1:col
        for k = 1:byte
            count = count + 1;
            I(i, j, k) = I_read(count, 1);
        end;
    end;
end;
I = double(I);
fclose(fin);
%% read evaluated data and convert GT
benchLabels = ones(row, col);
for i = 1: row
    for j = 1: col
      for k = 1:3
          if (FLAG == 1)
            benchLabels(i, (j - 1) * 3 + k) = 255 * bound_segment(i, j, k);
          else
              benchLabels(i, (j - 1) * 3 + k) = 255 * color_segment(i, j, k);
          end
      end
    end
end
sampleLabels = ones(row, col);
for i = 1: row
    for j = 1: col
       for k = 1:3
          sampleLabels(i, (j - 1) * 3 + k) = I(i, j, k);
      end
    end
end
benchLabels = uint8(benchLabels);
sampleLabels = uint8(sampleLabels);
benchLabels = double(benchLabels);
sampleLabels = double(sampleLabels);
figure, imshow(benchLabels);
figure, imshow(sampleLabels);
%% Begin evaluation
% update the four error measures:        
curBDE = compare_image_boundary_error(benchLabels, sampleLabels);     
[curPRI,curGCE,curVOI] = compare_segmentations(sampleLabels,benchLabels); 
% update cov
groundTruth{1}.Segmentation = sampleLabels;
seg = benchLabels;
[curCOV] = compare_covering(seg,groundTruth);


