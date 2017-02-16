x = -5:0.001:5;
formatSpec = '%8.5f,%8.5f\n';
fileId = fopen('functions/shift3.txt','w');
for i = 1:1:size(x,2)
    y = x(1,i)/2;
    fprintf(fileId,formatSpec,x(1,i),y);
end    
fclose(fileId);