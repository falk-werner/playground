# Virtual WebCAM

## Install virtual video device

    sudo apt update
    sudo apt install v4l2loopback-utils
    sudo modprobe v4l2loopback

## Capture screen using ffmpeg

    ffmpeg -f x11grab -r 15 -s 1280x720 -i :0.0+0,0 -vcodec rawvideo -pix_fmt yuv420p -threads 0 -f v4l2 /dev/video0

## Build

    mkdir build
    cd build
    cmake ..
    make

## Print Video Information

    ./video-info /dev/video0
    width : 640
    height: 480
    pixelformat: YU12
    bytesperline: 640
    sizeimage: 460800
    colorspace: 8
    priv: 0xfeedcafe
    flags: 0
    quantization: 0
    xfer_func: 0

## Send Single Image

    ./put-image -f ../img/angry.png

## References

*    [v4l2loopback](https://github.com/umlaeute/v4l2loopback)