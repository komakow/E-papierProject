#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os
import logging
from stuff import epd2in9
import time
from PIL import Image, ImageDraw, ImageFont
import traceback

logging.basicConfig(level=logging.DEBUG)

logging.info("Start")
epd = epd2in9.EPD()
logging.info("init and Clear")
epd.init(epd.lut_full_update)
epd.Clear(0x44)

font24 = ImageFont.truetype('stuff/Font.ttc', 24)
font18 = ImageFont.truetype('stuff/Font.ttc', 18)

Himage = Image.new('1', (epd.height, epd.width), 255)  # 255: clear the frame
draw = ImageDraw.Draw(Himage)
# draw.text((10, 0), 'Jamniczek', font=font24, fill=0)
# draw.text((10, 20), 'zajaczek', font=font24, fill=0)
# draw.text((20, 40), 'lisek', font=font24, fill=0)
# draw.text((20, 60), 'Jez Hippis', font=font24, fill=0)

# rysuj jezyka
logging.info("Wczytaj plik bmp")
bmp = Image.open('stuff/jez.bmp')
Himage.paste(bmp, (0, 0))
draw.text((170, 40), 'DOM', font=font24, fill=0)
epd.display(epd.getbuffer(Himage))