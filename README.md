# Movie2Ascii
<a href="github.com/BetterRage/Movie2Ascii/raw/master/polishcowrec.mp4">Takes a video and displays it using ASCII characters</a>

1. Download C++ header file with arrays defined for ASCII characters (8x8 pixels)
2. Rank each character based on number of pixels it uses
3. Use ffmpeg to decode video file frame by frame
4. Use lightness value of pixel in HSL format to pick the according ASCII character
5. Render frame with ASCII characters 
6. Output: 
<img src="https://github.com/BetterRage/Movie2Ascii/blob/master/polishcow.png"> <br />
<img src="https://github.com/BetterRage/Movie2Ascii/blob/master/pixar.png"> <br />




Dependencies:
-ffmpeg <br />
-SDL <br />
-spdlog <br />
-https://github.com/cameron314/readerwriterqueue <br />
