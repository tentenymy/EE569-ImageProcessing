% Demo for Structured Edge Detector (please see readme.txt first).

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

%% detect edge and visualize results
row = 321;
col = 481;
byte = 3;
fin = fopen('Farm.raw', 'r');
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
figure;
subplot(2,2,1);
imshow(I);

subplot(2,2,2);
fclose(fin);

%figure(1); k = imshow(I);
%I = imread('peppers.png');
tic, E=edgesDetect(I,model); toc
imshow(1 - E);
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

E_sort = sort(E);
threshold = 0.1;
num = floor(row * col * (1 - threshold));
count = 0;
row_new = floor(num / col + 1);
col_new = floor(num - (row_new - 1) * col);

threshold_value = E_sort(row_new, col_new);
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
subplot(2, 2, 3);
imshow(E_threshold);
  
    

%figure(1); im(I); 
%figure(2); im(1-E);
