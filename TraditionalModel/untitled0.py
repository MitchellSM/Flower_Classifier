# -*- coding: utf-8 -*-
"""
Created on Thu Mar 28 18:20:27 2019

@author: Mitchell
"""

import numpy as np
import cv2
from matplotlib import pyplot as plt

img = cv2.imread('1.bmp')
gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
ret, thresh = cv2.threshold(gray,0,255,cv2.THRESH_BINARY_INV+cv2.THRESH_OTSU)