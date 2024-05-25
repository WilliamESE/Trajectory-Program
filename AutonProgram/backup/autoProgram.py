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
		self.robots = [
			rb.Robot(root,pnt.Pose(0,0),0,0,15,15,6,4,"Sparky",'red'),
			rb.Robot(root,pnt.Pose(0,0),0,0,15,15,6,4,"Razzle_1",'blue'),
			rb.Robot(root,pnt.Pose(0,0),0,0,15,15,6,4,"Razzel_2",'green')
		]
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

		helpmenu = Menu(self.menubar, tearoff=0)
		helpmenu.add_command(label="Help Index")
		helpmenu.add_command(label="About...")
		self.menubar.add_cascade(label="Help",menu=helpmenu)
		root.config(menu=self.menubar)

		self.paths = [
			
		]

		self.selectedPath = -1

		self.sideFrame = Frame(root, width=286)
		self.sideFrame.pack(side=RIGHT, fill=Y, expand=True)
		#Robot list
		self.frmRobotList = Frame(self.sideFrame, width=286, height=150)
		self.frmRobotList.pack_propagate(0)
		self.frmRobotList.pack(side=TOP, fill=X)

		self.frmRobotTools = Frame(self.frmRobotList, width=286, height=35)
		self.frmRobotTools.pack_propagate(0)
		self.frmRobotTools.pack(side=TOP, fill=X)

		self.lblRobotList = Label(self.frmRobotTools, text="Robot List", font=("Arial", 16))
		self.lblRobotList.pack(side=LEFT, fill=Y)

		self.btnEditRobot = Button(self.frmRobotTools, text="Edit", font=("Arial", 16))
		self.btnEditRobot.pack(side=RIGHT, fill=Y)
		self.btnAddRobot = Button(self.frmRobotTools, text="Add", font=("Arial", 16))
		self.btnAddRobot.pack(side=RIGHT, fill=Y)

		self.lstRobots = Listbox(self.frmRobotList, width=286)
		cnt=0
		for r in self.robots:
			self.lstRobots.insert(cnt,r.name)
			cnt+=1
		self.lstRobots.pack(side=LEFT, fill=BOTH)
		self.lstRobots.bind('<<ListboxSelect>>', self.selectRobot)


		#Paths list
		self.frmPathList = Frame(self.sideFrame, width=286, height=150)
		self.frmPathList.pack_propagate(0)
		self.frmPathList.pack(side=TOP, fill=X)

		self.frmPathTools = Frame(self.frmPathList, width=286, height=35)
		self.frmPathTools.pack_propagate(0)
		self.frmPathTools.pack(side=TOP, fill=X)

		self.lblPathList = Label(self.frmPathTools, text="Paths", font=("Arial", 16))
		self.lblPathList.pack(side=LEFT, fill=Y)

		self.btnEditPath = Button(self.frmPathTools, text="Edit", font=("Arial", 16), command=self.editPath)
		self.btnEditPath.pack(side=RIGHT, fill=Y)
		self.btnAddPath = Button(self.frmPathTools, text="Add", font=("Arial", 16),command=self.addPath)
		self.btnAddPath.pack(side=RIGHT, fill=Y)
		self.btnAddPath = Button(self.frmPathTools, text="Gen", font=("Arial", 16),command=self.generatePath)
		self.btnAddPath.pack(side=RIGHT, fill=Y)

		self.pathFrames = []
		self.pathlbl = []
		self.pathBox = Frame(self.frmPathList, bd=1, relief=FLAT)
		self.genPathListInterface()
		self.pathBox.pack(side=TOP, fill=X)

		# self.lstPaths = Listbox(self.frmPathList, width=286)
		# cnt=0
		# for p in self.paths:
		# 	self.lstPaths.insert(cnt,p.name)
		# 	cnt+=1
		# self.lstPaths.pack(side=LEFT, fill=BOTH)
		# self.lstPaths.bind('<<ListboxSelect>>', self.selectPath)

		#Segments
		self.frmSegsList = Frame(self.sideFrame, width=286, height=300)
		self.frmSegsList.pack_propagate(0)
		self.frmSegsList.pack(side=TOP, fill=X)

		self.frmSegsTools = Frame(self.frmSegsList, width=286, height=35)
		self.frmSegsTools.pack_propagate(0)
		self.frmSegsTools.pack(side=TOP, fill=X)

		self.lblSegsList = Label(self.frmSegsTools, text="Segments", font=("Arial", 16))
		self.lblSegsList.pack(side=LEFT, fill=Y)

		self.btnEditSegs = Button(self.frmSegsTools, text="Edit", font=("Arial", 16), command=self.editSegment)
		self.btnEditSegs.pack(side=RIGHT, fill=Y)
		self.btnAddSegs = Button(self.frmSegsTools, text="Add", font=("Arial", 16), command=self.addSegment)
		self.btnAddSegs.pack(side=RIGHT, fill=Y)
		self.btnRemoveSeg = Button(self.frmSegsTools, text="Remove", font=("Arial", 16), command=self.removeSegment)
		self.btnRemoveSeg.pack(side=RIGHT, fill=Y)

		self.segsFrames = []
		self.segslbl = []
		self.segsBox = Frame(self.frmSegsList, bd=1, relief=FLAT)
		self.genSegmentListInterface()
		self.segsBox.pack(side=TOP, fill=X)

		#Segments
		self.frmSecList = Frame(self.sideFrame, width=286, height=300)
		self.frmSecList.pack_propagate(0)
		self.frmSecList.pack(side=TOP, fill=X)

		self.frmSecTools = Frame(self.frmSecList, width=286, height=35)
		self.frmSecTools.pack_propagate(0)
		self.frmSecTools.pack(side=TOP, fill=X)

		self.lblSecsList = Label(self.frmSecTools, text="Sections", font=("Arial", 16))
		self.lblSecsList.pack(side=LEFT, fill=Y)

		self.btnEditSec = Button(self.frmSecTools, text="Edit", font=("Arial", 16), command=self.editSection)
		self.btnEditSec.pack(side=RIGHT, fill=Y)
		self.btnAddSec = Button(self.frmSecTools, text="Add", font=("Arial", 16), command=self.addSection)
		self.btnAddSec.pack(side=RIGHT, fill=Y)
		self.btnRemoveSec = Button(self.frmSecTools, text="Remove", font=("Arial", 16), command=self.removeSection)
		self.btnRemoveSec.pack(side=RIGHT, fill=Y)

		self.secsFrames = []
		self.secslbl = []
		self.secsBox = Frame(self.frmSecList, bd=1, relief=FLAT)
		self.genSectionsListInterface()
		self.secsBox.pack(side=TOP, fill=X)

		self.fieldCanvas = Canvas(root)
		self.fieldCanvas.pack(side=LEFT, fill=BOTH, expand=True)
		self.fieldCanvas.configure(bg='black')
		self.fieldCanvas.bind('<Button-1>', self.onClick) #onAdd
		self.fieldCanvas.bind('<Button-3>', self.onAdd)
		self.fieldCanvas.bind('<B1-Motion>', self.onDrag)
		root.bind('<Configure>', self.resize)

		self.field = field.field(self.fieldCanvas, root)

	def resize(self, e):
		self.field.resize(e)

	def open(self):
		"""Open a file"""
		filenm = tkFileDialog.askopenfilename(initialdir = self.ROOT_DIR,filetypes = (("Path Data","*.pth"),("all files","*.*")))
		if not filenm: return
		dirpart, filepart = ntpath.split(filenm)
		exts = filepart.split('.')
		ext = exts[len(exts)-1]
		if(ext == "pth"):
			fl = open(filenm,"r")
			str = fl.readline()
			items = str.split(",")
			numPaths = int(items[0])
			for p in range(0,numPaths):
				path = ph.Path(len(self.paths),self.fieldCanvas,self.field,pnt.Pose(0,0),10.504,10.504,0.02,14.5,25.0,20.0,10.0,"")
				path.readPath(fl)
				self.paths.append(path)
			self.genPathListInterface()

	def save(self):
		"""Save a path"""
		filenm = tkFileDialog.asksaveasfilename(initialdir = self.ROOT_DIR,filetypes=(("Path Data","*.pth"),("all files","*.*")))
		if not filenm: return
		fl = open(filenm,"w+")
		fl.write("{0}\n".format(len(self.paths)))
		for p in self.paths:
			p.savePath(fl)

	
	def genPathListInterface(self):
		for x in range(0, len(self.pathFrames)):
			self.pathFrames[x].destroy()
		self.pathFrames = []
		self.pathlbl = []
		cnt=0
		for ph in self.paths:
			pathItem = Frame(self.frmPathList, bd=1, relief=FLAT)

			btxt = Label(pathItem,text=ph.name)
			btxt.pack(side=LEFT, padx=2, pady=2)

			#Colour
			if((cnt % 2) != 0):
				pathItem.config(bg="light blue")
				btxt.config(bg="light blue")

			#Selected
			if(self.selectedPath == cnt):
				pathItem.config(bg="#FFD700")
				btxt.config(bg="#FFD700")
				
			#Pack
			pathItem.pack(side=TOP, fill=X)
			pathItem.bind("<Button-1>", lambda event, arg = cnt: self.selectPath(event, arg))
			self.pathFrames.append(pathItem)
			self.pathlbl.append(btxt)
			cnt += 1		

	def genSegmentListInterface(self):
		if(self.selectedPath == -1):
			return
		for x in range(0, len(self.segsFrames)):
			self.segsFrames[x].destroy()
		self.segsFrames = []
		self.segslbl = []
		cnt=0
		for seg in self.paths[self.selectedPath].segments:
			segItem = Frame(self.frmSegsList, bd=1, relief=FLAT)

			btxt = Label(segItem,text="{0}".format(cnt))
			btxt.pack(side=LEFT, padx=2, pady=2)

			#Colour
			if((cnt % 2) != 0):
				segItem.config(bg="light blue")
				btxt.config(bg="light blue")

			#Selected
			if(self.paths[self.selectedPath].selected == cnt):
				segItem.config(bg="#FFD700")
				btxt.config(bg="#FFD700")
				
			#Pack
			segItem.pack(side=TOP, fill=X)
			segItem.bind("<Button-1>", lambda event, arg = cnt: self.selectSegment(event, arg))
			self.segsFrames.append(segItem)
			self.segslbl.append(btxt)
			cnt += 1

	def genSectionsListInterface(self):
		"""No more of this"""
		# if(self.selectedPath == -1):
		# 	return
		# for x in range(0, len(self.secsFrames)):
		# 	self.secsFrames[x].destroy()
		# self.secsFrames = []
		# self.secslbl = []
		# cnt=0
		# for sec in self.paths[self.selectedPath].sections:
		# 	secItem = Frame(self.frmSecList, bd=1, relief=FLAT)

		# 	btxt = Label(secItem,text="{0}".format(cnt))
		# 	btxt.pack(side=LEFT, padx=2, pady=2)

		# 	#Colour
		# 	if((cnt % 2) != 0):
		# 		secItem.config(bg="light blue")
		# 		btxt.config(bg="light blue")

		# 	#Selected
		# 	if(self.paths[self.selectedPath].selected == cnt):
		# 		secItem.config(bg="#FFD700")
		# 		btxt.config(bg="#FFD700")
				
		# 	#Pack
		# 	secItem.pack(side=TOP, fill=X)
		# 	secItem.bind("<Button-1>", lambda event, arg = cnt: self.selectSection(event, arg))
		# 	self.secsFrames.append(secItem)
		# 	self.secslbl.append(btxt)
		# 	cnt += 1

	def addRobot(self):
		"""Adds a robot to the list"""

	def addPath(self):
		"""Creates a new path for the selected robot"""
		p = ph.Path(len(self.paths),self.fieldCanvas,self.field,pnt.Pose(0,0),10.504,10.504,0.02,14.5,25.0,20.0,10.0,"New")
		self.paths.append(p)
		self.genPathListInterface()

	def editPath(self):
		#Find the path in the array
		if(self.selectedPath != -1):
			self.paths[self.selectedPath].hide()
		self.paths[self.selectedPath].editPath()
		self.genPathListInterface()

	def addSegment(self):
		"""Creates a new segment to the selected path"""
		if(self.selectedPath == -1):
			return
		self.paths[self.selectedPath].addSegment()
		self.paths[self.selectedPath].render()
		self.genSegmentListInterface()
		

	def editSegment(self):
		if(self.selectedPath == -1):
			return
		idx = self.paths[self.selectedPath].selected
		if(idx == -1):
			return
		self.paths[self.selectedPath].editSegment(idx)
		
	def addSection(self):
		"""Creates a new section to the selected path -- used to limit speed through that area"""
		# self.paths[self.selectedPath].addSection()
		# self.genSectionsListInterface()
		
	def editSection(self):
		"""No more of this"""
		# sel = self.paths[self.selectedPath].selectedSection
		# if(sel == -1):
		# 	return
		# self.paths[self.selectedPath].editSection(sel)
	
	def removeSegment(self):
		self.paths[self.selectedPath].removeSegment()
		self.genSegmentListInterface()
		
	def removeSection(self):
		"""No more of this"""
		# idx = self.paths[self.selectedPath].selectedSection
		# self.paths[self.selectedPath].removeSection(idx)
		# self.genSectionsListInterface()

	def selectRobot(self,event):
		"""Update the path and segment box with data from robot"""
	
	def selectPath(self,event,id):
		"""Update the segment box with data from path"""
		#Find the path in the array
		if(self.selectedPath != -1):
			self.paths[self.selectedPath].hide()
			self.pathFrames[self.selectedPath].config(bg="light blue")
			self.pathlbl[self.selectedPath].config(bg="light blue")

		self.selectedPath = id
		self.pathFrames[self.selectedPath].config(bg="#FFD700")
		self.pathlbl[self.selectedPath].config(bg="#FFD700")
		self.paths[self.selectedPath].render()
		self.genSegmentListInterface()
		#self.genSectionsListInterface()

	def selectSegment(self,event,id):
		"""Update the field render"""
		if(self.selectedPath == -1):
			return
		if(self.paths[self.selectedPath].selected != -1):
			self.segsFrames[self.paths[self.selectedPath].selected].config(bg="light blue")
			self.segslbl[self.paths[self.selectedPath].selected].config(bg="light blue")

		self.segsFrames[id].config(bg="#FFD700")
		self.segslbl[id].config(bg="#FFD700")
		self.paths[self.selectedPath].selectSegment(id)

	def selectSection(self,event,id):
		"""Update the render"""
		# if(self.selectedPath == -1):
		# 	return
		# if(self.paths[self.selectedPath].selectedSection != -1):
		# 	self.secsFrames[self.paths[self.selectedPath].selectedSection].config(bg="light blue")
		# 	self.secslbl[self.paths[self.selectedPath].selectedSection].config(bg="light blue")

		# self.secsFrames[id].config(bg="#FFD700")
		# self.secslbl[id].config(bg="#FFD700")
		# self.paths[self.selectedPath].selectSection(id)

	def onClick(self, event):
		"""Canvas with field was clicked, call paths and locate the item the user clicked"""
		item = self.fieldCanvas.find_closest(event.x,event.y)
		tags = self.fieldCanvas.gettags(item)
		if(len(tags) < 2):
			if(self.paths[self.selectedPath].selected != -1):
				self.segsFrames[self.paths[self.selectedPath].selected].config(bg="light blue")
				self.segslbl[self.paths[self.selectedPath].selected].config(bg="light blue")
			for p in self.paths:
				p.pathDeselect()
			return
		if("Path" in tags[0]):
			if(self.paths[self.selectedPath].selected != -1):
				self.segsFrames[self.paths[self.selectedPath].selected].config(bg="light blue")
				self.segslbl[self.paths[self.selectedPath].selected].config(bg="light blue")
			pth = tags[0].split(':')
			
			self.selectedPath = int(pth[1])
			self.paths[int(pth[1])].pathClicked(tags)
			self.genSegmentListInterface()
			#self.genSectionsListInterface()

	def onAdd(self, event):
		"""When right clicked this will add a segment"""
		if(self.selectedPath == -1):
			return
		self.paths[self.selectedPath].addSegment()
		seg = self.paths[self.selectedPath].segments[len(self.paths[self.selectedPath].segments)-1]
		if(len(self.paths[self.selectedPath].segments) != 1):
			seg.curve.p2 = pnt.Pose(event.x , event.y)
			seg.curve.ic2 = pnt.Pose(event.x + 30, event.y + 30)
		else:
			seg.curve.p1 = pnt.Pose(event.x, event.y)
			seg.curve.ic1 = pnt.Pose(event.x + 50, event.y)
			seg.curve.p2 = pnt.Pose(event.x + 120 , event.y)
			seg.curve.ic2 = pnt.Pose(event.x + 150, event.y)
		self.paths[self.selectedPath].render()
		self.genSegmentListInterface()

	def onDrag(self, event):
		"""When an object is dragged"""
		self.paths[self.selectedPath].pathDragged(event)

	def generatePath(self):
		self.paths[self.selectedPath].generate()
	
	
	#def _resize_image(self,event):
		#image = self.fieldItems.resize((self.master.winfo_width(),self.master.winfo_height()))
		#self.image = ImageTk.PhotoImage(image)
		#self.background.config(image = self.image)

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
