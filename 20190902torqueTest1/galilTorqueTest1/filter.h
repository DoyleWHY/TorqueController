#ifndef FILTER_H
#define FILTER_H


class filter
{
  public:
    filter();
    int buffer_left_move(double *aver,int j);
    double smooth_averpara_filter(double *aver,int j);
    double smooth_aver_filter(double *aver2,int j);

};

#endif // FILTER_H

/************************ (C) COPYRIGHT ROBO Medical ***********END OF FILE****/
