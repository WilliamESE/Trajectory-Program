from tkinter import *
import tkinter.font
import tkinter.filedialog as tkFileDialog
from tkinter import messagebox
from PIL import Image, ImageTk
import pickle
import array,sys,time,os

class field():
    def __init__(self, canvas, parent=None):
        self.data = {
            "location": "",
            "name":"",
            "width":12,
            "length":12,
            "ratio":1,
            "img":"",
            "img_empty":"",
            "objects":[],
            "start positions":[],
        }

        self.can = canvas
        self.ROOT_DIR = os.path.dirname(os.path.abspath(__file__))

        self.image = Image.open(self.ROOT_DIR + "\\fieldImage_noDisks.png")
        self.img_copy= self.image.copy()
        self.background_image = ImageTk.PhotoImage(self.image)

        self.image_disks = Image.open(self.ROOT_DIR + "\\fieldImage.png")
        self.img_disks_copy= self.image_disks.copy()
        self.background_withDisks = ImageTk.PhotoImage(self.image_disks)

        self.bg = self.can.create_image( 0, 0, image = self.background_image, anchor = "nw")

        self.can.bind("<Configure>",lambda event: self.resize(event))
        self.zeroPos = 2 #meaning in the top left: 1=top right; 2=bottom left; 3=bottom right; 4=middle
        self.openField("D:\\Documents\Gitlab\\vex-u-2223\\AutonProgram\\Fields\\VRC_2223_Spinup\\vrc_2223_spinup.txt")
        print(self.data)
        
        # x,y = self.convertToField(12,12)
        # print(x,y,sep=",")
        # t,g = self.convertFromField(x,y)
        # print(t,g,sep=",")
        # w = self.inchesToPixles(24)
        # h = self.inchesToPixles(24)
        # r = self.fieldItems.create_rectangle(x, y, w, h, fill="red")

    def openField(self, fname):
        #fname = tkFileDialog.askopenfilename(initialdir = self.ROOT_DIR,filetypes = (("field file","*.txt"),("all files","*.*")))
        f = open(fname,"rb")
        self.data = pickle.load(f)
        self.data["ratio"] = self.data["width"] / self.data["length"]

        
    def saveField(self, fname):
        #fname = tkFileDialog.asksaveasfilename(initialdir = self.ROOT_DIR,filetypes=[("field file","*.txt")])
        f = open(fname,"wb")
        pickle.dump(self.data, f)

    def loadField(self):
        #Reload image
        self.image = Image.open(self.ROOT_DIR + "\\Fields\\" + self.data["location"] + "\\images\\" + self.data["img_empty"])
        self.img_copy= self.image.copy()
        self.background_image = ImageTk.PhotoImage(self.image)

        self.bg = self.can.create_image( 0, 0, image = self.background_image, anchor = "nw")
        #update the start positions
        #everything else

    def resize(self, event):
        wid = event.width
        hei = event.width * self.data["ratio"]
        print(wid)
        self.image =Image.open(self.ROOT_DIR + "\\Fields\\" + self.data["location"] + "\\images\\" + self.data["img_empty"]).resize((int(wid),int(hei)), Image.ANTIALIAS)
        self.img_copy = ImageTk.PhotoImage(self.image)
        self.can.itemconfig(self.background_image, image = self.img_copy)

    def inchesToPixles(self,inches:float):
        return (inches / 12) * 77.5

    def pixlesToInches(self,pixles:float):
        return (pixles / 77.5) * 12

    def convertToField(self,x:float, y:float):
        if(self.zeroPos == 0): #Top left
            return (self.inchesToPixles(x)+5),(self.inchesToPixles(y)+5)
        elif(self.zeroPos == 1): #Top Right
            return ((907-self.inchesToPixles(x))),(self.inchesToPixles(y)+5)
        elif(self.zeroPos == 2): #Bottom Left
            return (self.inchesToPixles(x)+5),((907-self.inchesToPixles(y)))
        elif(self.zeroPos == 3): #Bottom Right
            return (907-self.inchesToPixles(x)),(907-self.inchesToPixles(y))
        else: #Middle
            return (453+self.inchesToPixles(x)+4),(453-self.inchesToPixles(y)+4)

    def convertFromField(self,x:float,y:float):
        if(self.zeroPos == 0): #Top left
            return (self.pixlesToInches(x-5)),(self.pixlesToInches(y-5))
        elif(self.zeroPos == 1): #Top Right
            return ((self.pixlesToInches(907-x))),(self.pixlesToInches(y-5))
        elif(self.zeroPos == 2): #Bottom Left
            return (self.pixlesToInches(x-5)),((self.pixlesToInches(907-y)))
        elif(self.zeroPos == 3): #Bottom Right
            return (self.pixlesToInches(907-x)),(self.pixlesToInches(907-y))
        else: #Middle
            return (self.pixlesToInches(x-457)),(self.pixlesToInches(453-y+4))
