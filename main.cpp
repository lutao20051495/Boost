#include <stdio.h>

#include "Adaboost.h"
#include "FDDB.h"

void main(int argc,char *argv[])
{

        int type = atoi(argv[1]);
        switch(type)
        {
        case 1:
                {
                        TrainAdaboost(string(argv[2]), string(argv[3]), string(argv[4]));
                        break;
                }
        case 2:
                {
                        //TestMr(string(argv[2]), string(argv[3]),atof(argv[4]));
                        break;
                }
        case 3:
                {
                        //TestFpr(string(argv[2]), string(argv[3]),atof(argv[4]));
                        break;
                }
        case 4:
                {
                        //detect video
                        //DectectVideo(string(argv[2]), string(argv[3]),
                        //             string(argv[4]), atoi(argv[5]));
                        break;
                }
        case 5:
                {
                        //DetectImage(string(argv[2]), string(argv[3]),
                        //             string(argv[4]), atoi(argv[5]));
                        break;
                }
        case 6:
                {
                        genFacePatchFDDB(string(argv[2]), string(argv[3]));
                        break;
                }
        default:
                {
                        cout << "error!" << endl;
                }
        }

        cout << "Done!" << endl;
        getchar();
}
