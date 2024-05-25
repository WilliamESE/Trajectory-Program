import settings
from tkinter import *
import tkinter.font
import tkinter.filedialog as tkFileDialog
from tkinter import messagebox
import pose as pnt
import math as m
import pickle

class Robot():
    def __init__(self,parent,file:StringVar):
        self.data = {
            "name": "",
            "type": "INLINE",
            "position": {
                "heading": 0,
                "x": 0,
                "y": 0,
            },
            "length": 0,
            "width": 0,
            "img": "",
            "wheels": {
                "count": 0,
                "diameter": 0,
                "len": 0,
                "wid": 0,
            },
            "colour": 0,
            "paths": [],
            "location": "",
        }
        self.root = parent
        self.open(file)
    
    def open(self, fname):
        f = open(fname,"rb")
        self.data = pickle.load(f)

    def save(self, fname):
        f = open(fname,"wb")
        pickle.dump(self.data, f)
    
    def setPosition(self,pos:pnt.Pose,heading:float):
        self.data["position"]["heading"] = heading
        self.data["position"]["x"] = pos.x
        self.data["position"]["y"] = pos.y

    def render(self):
        #Draw the robot on the canvas
        if(self.data["type"] == "INLINE"): #INLINE drive
            rad = m.radians(self.data["position"]["heading"])
            #Front indicator
            p = pnt.Pose(self.data["position"]["x"] + (self.data["length"]/2) * m.cos(rad),self.data["position"]["y"] + (self.data["length"]/2) * m.sin(rad))
            self.f_indicator = self.root.create_line(self.data["position"]["x"],self.data["position"]["y"],p.x,p.y,arrow=FIRST,fill=self.data["colour"])

            #Center line
            cls = pnt.Pose(self.data["position"]["x"] + (self.data["width"]/2) * m.cos(rad + (m.pi/2)),self.data["position"]["y"] + (self.data["width"]/2) * m.sin(rad + (m.pi/2)))
            cle = pnt.Pose(self.data["position"]["x"] + (self.data["width"]/2) * m.cos(rad - (m.pi/2)),self.data["position"]["y"] + (self.data["width"]/2) * m.sin(rad - (m.pi/2)))
            self.center_line = self.root.create_line(cls.x,cls.y,cle.x,cle.y,fill=self.data["colour"])

            #Wheels
            #Estiblish the lines along each side, then use the count as the step along that line for placing wheels.
            rls = pnt.Pose(cls.x + (self.data["length"]/2) * m.cos(rad + (m.pi)),cls.y + (self.data["length"]/2) * m.sin(rad + (m.pi)))
            rle = pnt.Pose(cls.x + (self.data["length"]/2) * m.cos(rad),cls.y + (self.data["length"]/2) * m.sin(rad))
            rld = rls - rle

            lls = pnt.Pose(cle.x + (self.data["length"]/2) * m.cos(rad + (m.pi)),cle.y + (self.data["length"]/2) * m.sin(rad + (m.pi)))
            lle = pnt.Pose(cle.x + (self.data["length"]/2) * m.cos(rad),cle.y + (self.data["length"]/2) * m.sin(rad))
            lld = lls - lle

            



    def update(self):
        """redraw the robot"""