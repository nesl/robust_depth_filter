#include "depth_filter.h"
#include "math.h"
 
//pressure is in bars from pressure sensor, timestamp in seconds, temp in C from pressure sensor, salinity in PPT, dx = velocity (m/s)
//x0 = initial position / predicted position, g and h are filter gains, filter output is output_arr
void depth_filter_class::d_filt(float P, float prev_timestamp, float current_timestamp, float temp, float salinity, float x0, float dx, float g, float h, float output_arr[3]){
    float t= temp;
    float S= salinity;
    float kw= 0.0, Aw= 0.0, Bw= 0.0, k0= 0.0, A= 0.0, B= 0.0, bulk_modulus= 0.0, C= 0.0, dt = 0.0, z = 0.0, x_pred = 0.0, rho_w= 0.0, rho_zero= 0.0, density_seawater = 0.0;
    float x_est = 0.0, residual = 0.0;
    //calculate Secant bulk modulus
    kw= 19652.21+ (148.4206*t) - (2.327105*pow(t,2.0)) + (1.360477*pow(10.0,-2.0)*pow(t,3.0)) - (5.155288*pow(10.0,-5.0)*pow(t,4.0));
    Aw= 3.239908+ (1.43713*pow(10.0,-3.0)*t)+ (1.16092*pow(10.0,-4.0)*pow(t,2.0))- (5.77905*pow(10.0,-7.0)*pow(t,3.0));
    Bw= (8.50935*pow(10.0,-5.0)) - (6.12293*pow(10.0,-6.0)*t) + (5.2787*pow(10.0,-8.0)*pow(t,2.0));
    k0= kw + ((54.6746- (0.603459*t)+ (1.09987*pow(10.0,-2.0)*pow(t,2.0)) - (6.1670*pow(10.0,-5.0)*pow(t,3.0)))*S) + ((7.944*pow(10.0,-2.0) + (1.6483*pow(10.0,-2.0)*t)- (5.3009*pow(10.0,-4.0)*pow(t,2.0)))*(pow(S,1.5)));
    A= Aw+ (((2.2838*pow(10.0,-3.0)) - (1.0981*pow(10.0,-5.0)*t)- (1.6078*pow(10.0,-6.0)*pow(t,2.0)))*S) + (1.91075*pow(10.0,-4.0)*pow(S,1.5));
    B= Bw+ (((-9.9348*pow(10.0,-7.0)) + (2.0816*pow(10.0,-8.0)*t) + (9.1697*pow(10.0,-10.0)*pow(t,2.0)))*S);
    bulk_modulus= k0 + (A*P) + (B*P*P);
    
    //1-atm equation of state
    A= (8.24493*pow(10.0,-1.0)) - (4.0899*pow(10.0,-3.0)*t) + (7.6438*pow(10.0,-5.0)*pow(t,2)) - (8.2467*pow(10.0,-7.0)*pow(t,3.0)) + (5.3875*pow(10.0,-9.0)*pow(t,4.0));
    B= (-5.72466*pow(10.0,-3.0)) + (1.0227*pow(10.0,-4.0)*t) - (1.6546*pow(10.0,-6.0)*pow(t,2.0));
    C= 4.8314*pow(10.0,-4.0);
    rho_w= 999.842594 + (6.793952*pow(10.0,-2.0)*t) - (9.095290*pow(10.0,-3.0)*pow(t,2.0)) + (1.001685*pow(10.0,-3.0)*pow(t,3.0)) - (1.120083*pow(10.0,-6.0)*pow(t,4.0)) + (6.536336*pow(10.0,-9.0)*pow(t,4.0));
    rho_zero = rho_w+ (A*S) + (B*pow(S,1.5)) + (C*pow(S,2.0));
    
    //Measured depth
    density_seawater = rho_zero/(1- (P/bulk_modulus));
    z = P/(density_seawater*9.80665);
    
    //alpha-beta filter (robust to timestamping errors and noise)
    x_est = x0;
    dt = current_timestamp - prev_timestamp;
    output_arr[0] = current_timestamp; //input this as prev_timestamp for subsequent calls
    x_pred = x_est + (dx*dt);
    residual = z-x_pred;
    dx = dx + h * residual / dt;
    output_arr[1] = dx; //input this as new dx for subsequent calls
    x_est = x_pred + g * residual;
    output_arr[2] = x_est; //input this as new x_0 for subsequent calls, this is also the filter output
    
}