#include "filter.h"

filter::filter()
{

}

int filter::buffer_left_move(short int *aver,int j)
{
    int i ;
    for(i = 1 ; i < j ; i++)
    {
        aver[i-1] = aver[i] ;
    }
    return 0;
}

short int filter::smooth_averpara_filter(short int *aver,int j)
{
    float sum=0;
    for(int i=0;i<j;i++)
    {
        if (i>=0 && i<=j/2-1)
            sum+=aver[i]*(0.8/j);
        else if (i>=j/2 && i<=j-2)
            sum+=aver[i]*(0.8/(j-2));
        else if (i==j-1)
            sum+=aver[i]*0.2f;
    }
    return sum;
}

short int filter::smooth_aver_filter(short int *aver2,int j)
{
    float sum2=0;
    float min = 0;
    float max = 0;
    for(int i=0;i<j;i++)
    {
        sum2+=aver2[i];
        if (min>aver2[i]){
            min = aver2[i];
        }

        if (max<aver2[i]){
            max = aver2[i];}
    }
    return (sum2-min -max)/(j-2);
}

