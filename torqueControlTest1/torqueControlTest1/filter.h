#ifndef FILTER_H
#define FILTER_H


class filter
{
  public:
    filter();
    int buffer_left_move(short int *aver,int j);
    short int smooth_averpara_filter(short int *aver,int j);
    short int smooth_aver_filter(short int *aver2,int j);

};

#endif // FILTER_H

/************************ (C) COPYRIGHT ROBO Medical ***********END OF FILE****/
