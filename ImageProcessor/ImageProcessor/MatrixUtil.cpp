#include "MatrixUtil.h"

namespace util
{
	void MatrixUtil::rotate3X3In90Degrees(float src[3][3])
	{
        float dst[3][3];

        for (int row = 0; row < 3; row++)
        {
            for (int col = 0; col < 3; col++)
            {
                const int value = src[row][col];
                dst[col][2 - row] = value;
            }
        }

        memcpy(dst, src, 3 * 3 * sizeof(float));
	}

    void MatrixUtil::multiply3ItemByItem(float entry1[][3], float entry2[][3], float result[][3])
    {
        for (int x = 0; x < 3; x++)
        {
            for (int y = 0; y < 3; y++) 
            {
                result[x][y] = entry1[x][y] * entry2[x][y];
            }
        }
    }
}