#include"Exhaustion.h"

using namespace cv;
using namespace std;


extern unsigned char *qr_binarize(const unsigned char *_img, int _width, int _height, locateRect* BWpointer)
{
	unsigned char *mask_sp = NULL;
	if (_width > 0 && _height > 0){
		unsigned int     *col_sums;
		int            logwindw;
		int            logwindh;
		int            windw;
		int            windh;
		int            y0offs;
		int            y1offs;
		unsigned int      g;
		int            x;
		int            y;
		mask_sp = (unsigned char *)malloc(_width*_height*sizeof(*mask_sp));
		/*We keep the window size fairly large to ensure it doesn't fit completely
		inside the center of a finder pattern of a version 1 QR code at full
		resolution.*/
		for (logwindw = 4; logwindw < 8 && (1 << logwindw) < (_width + 7 >> 3); logwindw++);
		for (logwindh = 4; logwindh < 8 && (1 << logwindh) < (_height + 7 >> 3); logwindh++);
		windw = 1 << logwindw;
		windh = 1 << logwindh;
		col_sums = (unsigned int *)malloc(_width*sizeof(*col_sums));
		/*Initialize sums down each column.*/
		for (x = 0; x < _width; x++){
			g = _img[x];
			col_sums[x] = (g << logwindh - 1) + g;
		}
		for (y = 1; y < (windh >> 1); y++){
			y1offs = QR_MINI(y, _height - 1)*_width;
			for (x = 0; x < _width; x++){
				g = _img[y1offs + x];
				col_sums[x] += g;
			}
		}
		for (y = 0; y < _height; y++){
			unsigned int m;
			int      x0;
			int      x1;
			/*Initialize the sum over the window.*/
			m = (col_sums[0] << logwindw - 1) + col_sums[0];
			for (x = 1; x < (windw >> 1); x++){
				x1 = QR_MINI(x, _width - 1);
				m += col_sums[x1];
			}
			for (x = 0; x < _width; x++){
				/*Perform the test against the threshold T = (m/n)-D,
				where n=windw*windh and D=3.*/
				g = _img[y*_width + x];
				mask_sp[y*_width + x] = -(g + 3 << logwindw + logwindh < m) & 0xFF;
				/*Update the window sum.*/
				if (x + 1 < _width){
					x0 = QR_MAXI(0, x - (windw >> 1));
					x1 = QR_MINI(x + (windw >> 1), _width - 1);
					m += col_sums[x1] - col_sums[x0];
				}
			}
			/*Update the column sums.*/
			if (y + 1 < _height){
				y0offs = QR_MAXI(0, y - (windh >> 1))*_width;
				y1offs = QR_MINI(y + (windh >> 1), _height - 1)*_width;
				for (x = 0; x < _width; x++){
					col_sums[x] -= _img[y0offs + x];
					col_sums[x] += _img[y1offs + x];
				}
			}
		}

		for (x = 1; x <= BWpointer->Xcoordi; x++)
		{
			for (y = 1; y <= _height; y++)
				mask_sp[(y - 1)*_width + x] = 255;
		}
		for (x = BWpointer->Xcoordi + BWpointer->Xlength -1; x <= _width; x++)
		{
			for (y = 1; y <= _height; y++)
				mask_sp[(y - 1)*_width + x] = 255;
		}
		for (y = 1; y <= BWpointer->Ycoordi; y++)
		{
			for (x = 1; x <= _width; x++)
				mask_sp[(y - 1)*_width + x] = 255;
		}
		for (y = BWpointer->Ycoordi + BWpointer->Ylength - 1; y <= _height; y++)
		{
			for (x = 1; x <= _width; x++)
				mask_sp[(y - 1)*_width + x] = 255;
		}
        
        if(col_sums != NULL)
		    free(col_sums);

		return (mask_sp);
	}
}