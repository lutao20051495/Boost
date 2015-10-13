#include <stdio.h>

#include "StrongClassifier.h"
#include "evaluator/Evaluator.h"
int main(int argc,char *argv[])
{
        Evaluator evl;
        if(!evl.LoadParameter(string(argv[argc-1])))
        {
                cout << "load parameter error!" << endl;
        }
        else
        {
                evl.Run(argc-1, argv+1);
        }

        /*
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
                        //TestMrAdaboost(string(argv[2]), string(argv[3]),atof(argv[4]));
                        break;
                }
        case 3:
                {
                        //TestFprAdaboost(string(argv[2]), string(argv[3]),atof(argv[4]));
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
        */
        cout << "Done!" << endl;
        getchar();
	
	return 1;
}
