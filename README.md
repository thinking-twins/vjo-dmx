# Visual Jockey Gold DMX Plugins #

This is a collection of [VJo](http://www.visualjockey.com/) custom effects to easily stream your output screen to external DMX devices (e.g. LED Pixel Walls, LED Strings, DMX Fixtures). 

![03_prototyp_dmxplugin.JPG](https://www.thinking-twins.net/public/vjo/03_prototyp_dmxplugin.jpg)


## Available plugins ##

* **DMX Output** (FTDI Devices)
* **Color Grading** (simple gamma correction pass to enhance the color accuracy on ws2801 rgb led pixel modules)
* **Mosaic Effect** (to simulate/preview the rasterized output)
* **Time Blur** (smoothly fade out colors over time)

![vjo_plugins.png](https://www.thinking-twins.net/public/vjo/vjo_plugins.png)

___

## Installation ##

* Download the **plugin files** here: [dmx-tools.zip](https://www.thinking-twins.net/public/vjo/dmx-tools.zip) (contains plugin-fxp, examples and some resources)
* Extract/Merge the archive into the Visual Jockey **root** folder (usually C:\Program Files (x86)\VisualJockey Gold SP1\)
* Connect your DMX Converter (DMX to WS2811 converter)
* Open Visual Jockey Gold SP1
* In the Composition gallery, there's a new Folder called "DMX Examples"
* Load **DMX Example 01.fxc**
* Run the composition!

___

## Example Video ##

See the following [example video](https://youtu.be/1VQNskTLkXc).
___

## DMX Output configuration / settings ##

* Open the DMX Output Effect settings in your composition 
* Choose your DMX linear adress mode: Snake Bottom Left | Snake Top Right | Snake Top Left | 4Dot-Modules
* Define the correct matrix size (pixelX*pixelY)
* Run the test mode (Auto, Step, Snake, Increase Value, Flip, Linear Brightness, Matrix Brightness )


![06b_dmx_plugin.png](https://www.thinking-twins.net/public/vjo/06b_dmx_plugin.png)

## Hardware Setup ##

* USB -> DMX converter compatible with the Open DMX protocol
* DMX -> WS2801 converter (search for **DMX-512-SPI-decoder-to-WS-2801** on ebay)
* WS2801 compatible LED Pixels, Stripes, Boards

## Technical Details ##

* Your USB->DMX device needs to be compatible to the Open DMX protocol (often referred to as ENTEC Open Interface)
* the required FTDI driver (ftd2xx.dll) is distributed within the package
* Make sure the base-channel in your DMX->WS2801 converter is set to **0**

## Supported Platforms ##

 * Windows XP/Vista/7/8/10
 
## Build from Source ##
 
In order to extend the functionality or debug some issues you can build the \*.fxp-file using Visual Studio 2012 or newer. Just open the Solution-File :)

*Tip: For Live-debugging, set the working directory to C:\Program Files (x86)\VisualJockey Gold SP1\bin and make sure your output is copied to the plugin folder.*
