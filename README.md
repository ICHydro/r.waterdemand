# r.waterdemand

This is a simple GRASS GIS Addon that implements a spatial disaggregation algorithm the identify specific locations where surface water abstraction may occur to satisfy human water demand.

# Installation

The Addon is not available in the GRASS repository and therefore it cannot be installed with the GRASS g.extension command, but instead needs to be compiled locally. For complete instructions on how to compile GRASS and/or GRASS extensions we refer to the [GRASS compilation and install manual](https://grasswiki.osgeo.org/wiki/Compile_and_Install), but a short summary is provided below.

## Compilation on linux

Download the GRASS source code and copy the r.waterdemand code into the raster/ folder. Then run:

```
./configure
make libs
cd raster/r.waterdemand
make
```

Then the binary will be available in bin/r.waterdemand, from where you can run it directly or copy it into your GRASS installation.

If compilation fails because of missing dependencies, then you'll have to install those first

## Compilation on Mac

This depends on the method you use to install the dependencies, but in the case that you use the Kyngchaos frameworks then you can use something along the lines of:

```
export PATH=$PATH:/Library/Frameworks/PROJ.framework/Versions/4/Programs

CFLAGS="-ggdb -Wall -Werror-implicit-function-declaration -lm" ./configure --with-proj-includes=/Library/Frameworks/PROJ.framework/Versions/Current/Headers/ --with-proj-libs=/Library/Frameworks/PROJ.framework/Versions/Current/unix/lib/ --with-proj-share=/Library/Frameworks/PROJ.framework/Versions/4/Resources/proj/ --with-jpeg-includes=/Library/Frameworks/UnixImageIO.framework/unix/include/ --with-jpeg-libs=/Library/Frameworks/UnixImageIO.framework/unix/lib/ --with-gdal=/Library/Frameworks/GDAL.framework/Versions/2.1/unix/bin/gdal-config --with-tiff-includes=/Library/Frameworks/UnixImageIO.framework/unix/include/ --with-tiff-libs=/Library/Frameworks/UnixImageIO.framework/unix/lib/ --with-png-includes=/Library/Frameworks/UnixImageIO.framework/unix/include/ --with-png-libs=/Library/Frameworks/UnixImageIO.framework/unix/lib/ --with-opengl-includes=/opt/X11/include/ --with-opengl-libs=/opt/X11/lib/ --with-tcltk-includes="/System/Library/Frameworks/Tcl.framework/Versions/8.5/Headers/ /System/Library/Frameworks/Tk.framework/Versions/8.5/Headers/" --with-freetype-includes=/usr/local/Cellar/freetype/2.8.1/include/freetype2/  --without-fftw

make libs
cd raster/r.waterdemand
make
```

You may need to adjust the paths and versions of the installed frameworks.


# Use

We are still developing more complete documentation but in essence:

```
r.waterdemand elev=mydDEM river=myrivermap pop=mypopulationmap output=outputmap --o
```





