# robust_depth_filter
An intelligent underwater depth estimator exploiting COTS underwater pressure sensors. The filter takes in pressure and temperature from the sensor (and timestamps from an RTC) and uses an alpha-beta filter to calculate the depth of the object, with built-in thermocline, noise and timestamping error compensation.
