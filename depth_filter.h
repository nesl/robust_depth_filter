#ifndef DEPTH_FILTER_H
#define DEPTH_FILTER_H
 
class depth_filter_class{ 
    public:
        void d_filt(float P, float prev_timestamp, float current_timestamp, float temp, float salinity, float x0, float dx, float g, float h, float output_arr[3]);
};
 
#endif