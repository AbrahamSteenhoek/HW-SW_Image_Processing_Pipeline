function [bayer_img] = RGBToBayerMono( rgb_img )
A = imread( rgb_img );

[rows, columns, colorband] = size(A);
redChannel = A(:, :, 1);
greenChannel = A(:, :, 2);
blueChannel = A(:, :, 3);

bayer_mono = zeros( rows, columns );

for i = 1:rows
    for j = 1:columns
        if isequal(mod(i,2),0)
            if isequal(mod(j,2),0)
                bayer_mono(i,j,1) = 0;
                bayer_mono(i,j,2) = 0;
                bayer_mono(i,j,3) = [blueChannel(i,j)];
                % bayer_mono(i,j) = [blueChannel(i,j)];
            else
                bayer_mono(i,j,1) = 0;
                bayer_mono(i,j,2) = [greenChannel(i,j)];
                bayer_mono(i,j,3) = 0;
                % bayer_mono(i,j) = [greenChannel(i,j)];
            end
        else
            if isequal(mod(j,2),0)
                bayer_mono(i,j,1) = 0;
                bayer_mono(i,j,2) = [greenChannel(i,j)];
                bayer_mono(i,j,3) = 0;
                % bayer_mono(i,j) = [greenChannel(i,j)];
            else
                bayer_mono(i,j,1) = [redChannel(i,j)];
                bayer_mono(i,j,2) = 0;
                bayer_mono(i,j,3) = 0;
                % bayer_mono(i,j) = [redChannel(i,j)];
            end
        end
    end
end

%bayer_mono = mat2gray( bayer_mono, [0 255] );
%imshow(bayer_mono);
%title('Bayer Monochrome Image');

% bayer_img = uint8(bayer_mono);
bayer_img = bayer_mono;
end
