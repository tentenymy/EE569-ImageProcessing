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
close all;
clear all;

%% SIFT
Sift_Feature('School_bus1.jpg', 200, 374);
Sift_Feature('School_bus2.jpg', 200, 356);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
number = 3;
if number == 0
    filename = 'School_bus2.jpg';
end
if number == 1
    filename = 'Bus.jpg';
end
if number == 2
    filename = 'Sedan.jpg';
end
if number == 3
    filename = 'School_bus1.jpg';
end



% Load images
  I1=imread('School_bus1.jpg');
  I2=imread(filename);
  I2 = imresize(I2, [200, 374]);
  
% Get the Key Points
  Options.upright=true;
  Options.tresh=0.0001;
  Ipts1=OpenSurf(I1,Options);
  Ipts2=OpenSurf(I2,Options);
  
  
 
% Put the landmark descriptors in a matrix
  D1 = reshape([Ipts1.descriptor],64,[]); 
  D2 = reshape([Ipts2.descriptor],64,[]); 
  
    figure;
  imshow(D1);
  figure;
  imshow(D2);  
  
% Find the best matches
  err=zeros(1,length(Ipts1));
  cor1=1:length(Ipts1); 
  cor2=zeros(1,length(Ipts1));
  for i=1:length(Ipts1),
      distance=sum((D2-repmat(D1(:,i),[1 length(Ipts2)])).^2,1);
      [err(i),cor2(i)]=min(distance);
  end
  
% Sort matches on vector distance
  [err, ind]=sort(err); 
  cor1=cor1(ind); 
  cor2=cor2(ind);
  
% Show both images
  I = zeros([size(I1,1) size(I1,2)*2 size(I1,3)]);
  I(:,1:size(I1,2),:)=I1; I(:,size(I1,2)+1:size(I1,2)+size(I2,2),:)=I2;
  figure, imshow(I/255); hold on;
  
% Show the best matches
  for i=1:30,
      c=rand(1,3);
      plot([Ipts1(cor1(i)).x Ipts2(cor2(i)).x+size(I1,2)],[Ipts1(cor1(i)).y Ipts2(cor2(i)).y],'-','Color',c)
      plot([Ipts1(cor1(i)).x Ipts2(cor2(i)).x+size(I1,2)],[Ipts1(cor1(i)).y Ipts2(cor2(i)).y],'o','Color',c)
  end