from tkinter import *
import tkinter.font
import tkinter.filedialog as tkFileDialog
from tkinter import messagebox
import array,sys,time,os
import ntpath
from PIL import Image, ImageTk
import robot as r

class autoPanel():
    def __init__(self, bot:r, parent=None):
        """Loads the panel for a robot"""
