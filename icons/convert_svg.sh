#!/bin/sh

convert() {
	W=$1
	H=$2

	inkscape -e /tmp/harbour-aprstx.png -w $W -h $H harbour-aprstx.svg
	pngcrush /tmp/harbour-aprstx.png ${W}x${H}/harbour-aprstx.png
	rm /tmp/harbour-aprstx.png
}

convert 86 86
convert 108 108
convert 128 128
convert 256 256
