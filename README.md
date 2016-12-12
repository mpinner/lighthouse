Arduino (Teensy) to interface with the HTC Vive
Lighthouse beacons for positional light based fun.

![image](https://c7.staticflickr.com/1/91/30694821654_52a88c412c_n.jpg)

This is a FORK to test a self positioning device based on the work from https://trmm.net/Lighthouse


I used a few libraries for fun and profit: 

   * [fastled](https://www.pjrc.com/teensy/td_libs_FastSPI_LED.html) : get your rainbow color mapping, color pallets switching, and brightness control.
   * [ewma](https://github.com/CBMalloch/Arduino/tree/master/libraries/EWMA) : helps to color between positions while the sensors are often occluded
   * [elapsedMillis](http://playground.arduino.cc/Code/ElapsedMillis) : slick way to fade out when device it not able to find the lighthouses


See more complete write-up here:
[https://blog.crashspace.org/2016/12/position-aware-device/](https://blog.crashspace.org/2016/12/position-aware-device/)