/**
 * @file camera.cpp
 * @author Adrián Yago & Ismael Tienda
 * @brief
 * @version 0.1
 * @date 2022-10-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "camera.hpp"

void progressBar(int row, int height, unsigned start)
{
    if (row % 20 == 0)
    {
        int bar = row;
        int percentage = 0;
        unsigned now = clock();
        double estimated = (double(now - start) / CLOCKS_PER_SEC) / row;

        estimated = (height - row) * estimated;

        cout << "   Progress   [";
        percentage = row * 100 / height;
        for (int i = 0; i < 40; i++)
        {
            if (bar >= height / 40)
            {
                cout << "|";
            }
            else
            {
                cout << " ";
            }
            bar -= height / 40;
        }
        cout << "]\t" << percentage << "\%";

        if (estimated < 9999)
        {
            if (estimated > 60)
            {
                cout << "       (Estimated time: " << int(estimated) / 60 << "min " << int(estimated) % 60
                     << "s) \r";
            }
            else
            {
                cout << "       (Estimated time: " << int(estimated) % 60
                     << "s) \r";
            }
        }
        else
        {
            cout << "\r";
        }

        cout.flush();
    }
}