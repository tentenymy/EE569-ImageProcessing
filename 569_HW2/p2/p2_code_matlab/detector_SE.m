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
function G = detector_SE(filename_read, filename_write, threshold)
%% set opts for training (see edgesTrain.m)
opts=edgesTrain();                % default options (good settings)
opts.modelDir='models/';          % model will be in models/forest
opts.modelFnm='modelBsds';        % model name
opts.nPos=5e5; opts.nNeg=5e5;     % decrease to speedup training
opts.useParfor=0;                 % parallelize if sufficient memory

%% train edge detector (~20m/8Gb per tree, proportional to nPos/nNeg)
tic, model=edgesTrain(opts); toc; % will load model if already trained

%% set detection parameters (can set after training)
model.opts.multiscale=1;          % for top accuracy set multiscale=1
model.opts.sharpen=2;             % for top speed set sharpen=0
model.opts.nTreesEval=2;          % for top speed set nTreesEval=1
model.opts.nThreads=4;            % max number threads for evaluation
model.opts.nms=1;                 % set to true to enable nms
%% evaluate edge detector on BSDS500 (see edgesEval.m)
if(0), edgesEval( model, 'show',1, 'name','' ); end
%% read the file
row = 321;
col = 481;
byte = 3;
fin = fopen(filename_read, 'r');
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
I = uint8(I);
fclose(fin);
%% detect edge and threshold the result
tic, E=edgesDetect(I,model); toc
E_test = reshape(E, row * col, 1);
max = -100;
min = 100;
for i = 1:row*col
    if (E_test(i, 1) > max)
        max = E_test(i , 1);
    end
    if (E_test(i, 1) < min)
        min = E_test(i, 1);
    end
end
E_sort = sort(E_test);
num = floor(row * col * (1 - threshold));
%row_new = floor(num / col + 1);
%col_new = floor(num - (row_new - 1) * col);
threshold_value = E_sort(num, 1);
E_threshold = E;
for i = 1: row
    for j = 1 : col
        if (E_threshold(i, j) > threshold_value)
            E_threshold(i, j) = 0;
        else
            E_threshold(i, j) = 1;
        end
    end
end
%% write the file to raw image
G = E_threshold;
fid = fopen(filename_write,'wb');
% Check if file exists
if (fid == -1)
	error('can not open output image filem press CTRL-C to exit \n');
	pause
end
% Transpose matrix to write file properly
G = G'.* 255;
% Write and close file
count = fwrite(fid, G, 'uchar');
fclose(fid);
% Transpose again to restore matrix
G = G';
