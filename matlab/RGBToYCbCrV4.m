function [ ycbcr ] = RGBToYCbCr( rgb_uint8 )
rgb_img = double( rgb_uint8 );
R = rgb_img( :, :, 1 );
B = rgb_img( :, :, 2 );
G = rgb_img( :, :, 3 );

conv_mat = [ ...
    65.481 128.553 24.966;...
    -37.797 -74.203 112; ...
    112 -93.786 -18.214] ./ 255; % TODO, use 256 divisor for faster computation speeds
offset = [16;128;128];

ycbcr_444 = zeros( size(rgb_img) );

% first, convert 8-bit RGB to YCbCr 4:4:4
for c = 1 : 3
    ycbcr_444(:,:,c) = ...
    ( conv_mat( c, 1 ) .* rgb_img(:,:,1) ) + ...
    ( conv_mat( c, 2 ) .* rgb_img(:,:,2) ) + ...
    ( conv_mat( c, 3 ) .* rgb_img(:,:,3) ) + ...
    offset(c);
end

ycbcr_422 = ycbcr_444;

[ rows, cols ~ ] = size( ycbcr_422 );

% Second, convert 4:4:4 -> 4:2:2
for row = 1:1:rows
    for col = 2:2:cols
        ycbcr_422( row, col-1, : ) = ycbcr_422( row, col, : );
        % ycbcr_422( :, col-1, : ) = ycbcr_422( :, col, : );
    end
end

ycbcr = uint8( ycbcr_422 );

figure();
imshow( ycbcr );

lb={'Y','Cb','Cr'};

figure();
for channel=1:3
subplot(1,3,channel)
YCBCR_C=ycbcr;
YCBCR_C(:,:,setdiff(1:3,channel))=intmax(class(YCBCR_C))/2; % display the Y Cb Cr components in color instead of grayscale
imshow(ycbcr2rgb(YCBCR_C))
title([lb{channel} ' component'],'FontSize',18);
end


end

