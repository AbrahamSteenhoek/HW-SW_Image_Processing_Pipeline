function [ rgb_img ] = DemosaicV3( bayer )
bayer = RGBToBayerMono('pokeball-480p.bmp');
figure();
image( bayer );
title( 'Bayer Filter Image' );
[ rows, columns, colorband ] = size( bayer );

rgb_img = zeros( rows, columns, 3 );
red = 0;
blue = 0;
green = 0;
R = 1;
G = 2;
B = 3;

for row = 2:( rows - 1 )
    for col = 2:( columns - 1 )
        % fprintf( "r: %d, c: %d\n", row, col );
        if mod( row, 2 ) == 0
            if mod( col, 2 ) == 0
                % fprintf('B');
                % B: even row, even col
                red = ( bayer( row-1, col-1, R ) + bayer( row-1, col+1, R ) + bayer( row+1, col-1, R ) + bayer( row+1, col+1, R ) ) / 4;
                green = ( bayer( row-1, col, G ) + bayer( row+1, col, G ) + bayer( row, col-1, G ) + bayer( row, col+1, G ) ) / 4;
                blue = bayer( row, col, B );

            else
                % fprintf('G');
                % G: even row, odd col
                red = ( bayer( row-1, col, R ) + bayer( row+1, col, R ) ) / 2;
                green = bayer( row, col, G );
                blue = ( bayer( row, col-1, B ) + bayer( row, col+1, B ) ) / 2;

            end
        else
            if mod( col, 2 ) == 0
                % fprintf('G');
                % G: odd row, even col
                red = ( bayer( row, col-1, R ) + bayer( row, col+1, R ) ) / 2;
                green = bayer( row, col, G );
                blue = ( bayer( row-1, col, B ) + bayer( row+1, col, B ) ) / 2;

            else
                % fprintf('R');
                % R: odd row, odd col
                red = bayer( row, col, R );
                green = ( bayer( row-1, col, G ) + bayer( row+1, col, G ) + bayer( row, col-1, G ) + bayer( row, col+1, G ) ) / 4;
                blue = ( bayer( row-1, col-1, B ) + bayer( row-1, col+1, B ) + bayer( row+1, col-1, B ) + bayer( row+1, col+1, B ) ) / 4;
            end
        end
        rgb_img( row, col, : ) = [ red, green, blue ];
        % pause
    end
end

rgb_img = uint8( rgb_img );
figure();
image( rgb_img );
title( 'Demosaiced Image' );

end