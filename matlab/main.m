clear, clc

DATASET_N = 10;
DATASET_D = 1;

% Initialize Dataset
DATASET = zeros(DATASET_N, DATASET_D);
for i = 0:DATASET_N - 1
    
    for j = 0:DATASET_D - 1

%         DATASET(i + 1, j + 1) = i / (j + 1) + j / (i + 1) + ...
%             ( i * (j + 1) ) / (i + j + 1) + ( i + j + 1 ) / ( i * j + 1);

        DATASET(i + 1, j + 1) = i;

    end
    
end

% Build Vantage Point Tree
vp_tree = vpTree( DATASET );