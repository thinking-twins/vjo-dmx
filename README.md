# Visual Jockey Gold DMX Plugins #

This is a collection of [VJo](http://www.visualjockey.com/) custom effects to easily stream your output screen to external DMX devices (e.g. LED Pixel Walls, LED Strings, DMX Fixtures). 

![03_prototyp_dmxplugin.JPG](https://bitbucket.org/repo/x6G97j/images/1282263225-03_prototyp_dmxplugin.JPG)


### Available plugins ###

* **DMX Output** (FTDI Devices)
* **Color Grading** (simple gamma correction pass to enhance the color accuracy on ws2801 rgb led pixel modules)
* **Mosaic Effect** (to simulate/preview the rasterized output)
* **Time Blur** (smoothly fade out colors over time)

![vjo_plugins.png](https://bitbucket.org/repo/x6G97j/images/4099938706-vjo_plugins.png)

### Installation ###

* Download the **plugin files** here:
* Extract the archive into the Visual Jockey **root** folder (usually C:\Program Files (x86)\VisualJockey Gold SP1\)
* Connect your DMX Converter (DMX to WS2811 converter)
* Open Visual Jockey Gold SP1
* In the Composition gallery, there's a new Folder called "DMX Examples"
* Load **DMX_test01.fxc**
* Run the composition!

### Example Videos ###

This plugin creates stunning results when using ws2801 rgb pixels. See the following [example video](https://youtu.be/1VQNskTLkXc).

### DMX Output configuration / settings ###

* Open the DMX Output Effect settings in your composition 
* Choose your DMX linear adress mode: Snake Bottom Left | Snake Top Right | Snake Top Left | 4Dot-Modules
* Define the correct matrix size (pixelX*pixelY)
* Run the test mode (Auto, Step, Snake, Increase Value, Flip, Linear Brightness, Matrix Brightness )

![06b_dmx_plugin.png](https://bitbucket.org/repo/x6G97j/images/1242016383-06b_dmx_plugin.png)