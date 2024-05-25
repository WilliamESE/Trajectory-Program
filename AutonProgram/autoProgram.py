####################################################################################################
# Program: Coach Autonomous Program
# Purpose: The coach made program for working with autonomous
# Author: Bill
# Date: 
# Version: 1
####################################################################################################
#Program uses Tkinter for user interface
import settings
from tkinter import *
import tkinter.font
import tkinter.filedialog as tkFileDialog
from tkinter import messagebox
import array,sys,time,os
import ntpath
from PIL import Image, ImageTk
import field
import robot as rb
import path as ph
import bezier as bz
import pose as pnt
import math

#Other folders need to be added to the path: sys.path.append('Location')
#	Then files in that location can be imported

class Form1(Frame):
	global root,canv
	def __init__(self, parent=None, **kw):
		Frame.__init__(self, parent, kw)
		self.op = False
		self.ROOT_DIR = os.path.dirname(os.path.abspath(__file__))
		self.selectedPath = -1
	
	def makeWidgets(self):
		#Construct Menu System
		self.menubar = Menu(root)
		filemenu = Menu(self.menubar, tearoff=0)
		filemenu.add_command(label="Open", command=self.open)
		filemenu.add_command(label="Save", command=self.save)
		filemenu.add_separator()
		filemenu.add_command(label="Exit",command=root.destroy)
		self.menubar.add_cascade(label="File",menu=filemenu)

		robotmenu = Menu(self.menubar, tearoff=0)
		robotmenu.add_command(label="New")
		robotmenu.add_command(label="Open")
		robotmenu.add_separator()
		robotmenu.add_command(label="Save")
		robotmenu.add_command(label="Save As")
		robotmenu.add_separator()
		robotmenu.add_command(label="Settings")
		self.menubar.add_cascade(label="Robot",menu=robotmenu)

		pathmenu = Menu(self.menubar, tearoff=0)
		pathmenu.add_command(label="Export Path")
		pathmenu.add_command(label="Add Segment")
		pathmenu.add_separator()
		pathmenu.add_command(label="New")
		pathmenu.add_command(label="Open")
		pathmenu.add_separator()
		pathmenu.add_command(label="Save")
		pathmenu.add_command(label="Save As")
		self.menubar.add_cascade(label="Path",menu=pathmenu)

		helpmenu = Menu(self.menubar, tearoff=0)
		helpmenu.add_command(label="Help Index")
		helpmenu.add_command(label="About...")
		self.menubar.add_cascade(label="Help",menu=helpmenu)
		root.config(menu=self.menubar)

		self.selectedPath = -1

		self.fieldCanvas = Canvas(root)
		self.fieldCanvas.pack(side=LEFT, fill=BOTH, expand=True)
		self.fieldCanvas.configure(bg='black')
		# self.fieldCanvas.bind('<Button-1>', self.onClick) #onAdd
		# self.fieldCanvas.bind('<Button-3>', self.onAdd)
		# self.fieldCanvas.bind('<B1-Motion>', self.onDrag)
		# root.bind('<Configure>', self.resize)

		self.field = field.field(self.fieldCanvas, root)

	def resize(self, e):
		self.field.resize(e)

	def open(self):
		"""Open a file"""
		

	def save(self):
		"""Save a path"""

	def _update(self):
		update_flag = False # Nothing to update on the screen at this time.
		self._timer = self.after(1000, self._update) # do this all again 1 second from now.
		
def main():
	global root,Frm1
	
	args = sys.argv[1:] #Get main arguments
	#Initialize settings
	#settings.init()
	

	root = Tk()

	root.geometry("%dx%d+0+0" % (1200, 914))
	root.wm_title("839 Coach Auton Program")
	root.configure(background="white")

	root.update() # this is needed to obtain actual screen measurements 
		
	Frm1 = Form1(root)
	Frm1.makeWidgets() # Make sure Setup runs before makeWidgets
	Frm1.pack(side=TOP)

	root.update() # this is needed to obtain actual screen measurements 
	Frm1._update() # Activate the 1 second process.
	
	#The only argument (at least for now) should be a model
	#if(len(args) > 0):
	#	modelName = args[0]
	#   Frm1.loadModel(modelName)

	root.mainloop()
    
if __name__ == '__main__':
    main()
