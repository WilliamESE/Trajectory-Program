import settings
from tkinter import *
import tkinter.font
import tkinter.filedialog as tkFileDialog
from tkinter import simpledialog
from tkinter import messagebox
import field
import pose as pnt
import math
import bezier as cr
import numpy as np

# Changes for Feburary 2023
# Many modifications need to be made to the generated path to make sure things work properly
#   Delays when shooting
#   Turns to make sure the robot is pointing in the right direction befor shooting
#   Roller at the beginning of a segment
#   Turning when segments headings don't start and end at the same angle, are not aligned properly
#   Running intake in a segment ends when accelerating
# Speed segments are still unrelable.
#   I'm thinking it would be better if I add a max speed property to curve segments + intake and allow that to limit speed for sections and the like.
#   Speed segments save floats instead of ints causing the opening to fail

def gradColour(min:float,max:float,val:float):
    """Returns a colour string based on the val and how close it is to the min and max"""
    #Max is red, min is blue, inbetween in green
    middle = (min + max) / 2
    if(val == middle):
        return "#00FF00"
    
    if(val > middle): #Get a red colour
        percent = math.fabs(val / max)
        rCol = int(255 * percent)
        if(rCol > 255):
            rCol = 255
        return "#{:02x}{:02x}00".format(rCol,255-rCol)

    if(val < middle): #Get a blue colour
        percent = math.fabs(val / min)
        rCol = int(255 * percent)
        if(rCol > 255):
            rCol = 255
        return "#00{:02x}{:02x}".format(255-rCol,rCol)

def gradColourReverse(min:float,max:float,val:float):
    """Returns a colour string based on the val and how close it is to the min and max"""
    #Max is red, min is green, inbetween in blue
    middle = (min + max) / 2
    if(val == middle):
        return "#0000FF"
    
    if(val > middle): #Get a red colour
        percent = math.fabs(val / max)
        rCol = int(255 * percent)
        if(rCol > 255):
            rCol = 255
        return "#{:02x}00{:02x}".format(rCol,255-rCol)

    if(val < middle): #Get a green colour
        percent = math.fabs(val / min)
        rCol = int(255 * percent)
        if(rCol > 255):
            rCol = 255
        return "#00{:02x}{:02x}".format(rCol,255-rCol)
        

# class SpeedSection():
#     def __init__(self,speed:float,curveStartId:int,curveEndId:int,startPercent:float,endPercent:float, colour, turn:bool = False):
#         """Defines an intake section for a path"""
#         self.startId = curveStartId
#         self.endId = curveEndId
#         self.startPercent = startPercent
#         self.endPercent = endPercent
#         self.speed = speed
#         self.colour = colour
#         self.visible = True
#         #Turn information stuff for later
#         self.turn = turn
#         self.radius = 0
#         self.distance = 0
#         self.angle = 0
#         self.intake = False
    
#     def modify(self,speed:float,curveStartId:int,curveEndId:int,startPercent:float,endPercent:float,):
#         self.startId = curveStartId
#         self.endId = curveEndId
#         self.startPercent = startPercent
#         self.endPercent = endPercent
#         self.speed = speed

#     def setVisible(self, value:bool):
#         self.visible = value
        
#     def saveSection(self,f):
#         f.write("{0},{1},{2},{3},{4},{5}\n".format(self.startId,self.endId,self.startPercent,self.endPercent,self.speed,self.intake))
        
#     def readSection(self,f):
#         str = f.readline()
#         items = str.split(",")
#         self.startId = int(items[0])
#         self.endId = int(items[1])
#         self.startPercent = float(items[2])
#         self.endPercent = float(items[3])
#         self.speed = float(items[4])
#         if(items[5] == "True" or items[5] == "1"):
#             self.intake = True

#     def isInSection(self, id:int, percent:float) -> bool:
#         if((id > self.startId)and(id < self.endId)):
#             return True

#         if((self.startId == self.endId)and(id == self.startId)):
#             if((percent >= self.startPercent)and(percent <= self.endPercent)):
#                 return True
#         else:
#             if(id == self.startId):
#                 if(percent >= self.startPercent):
#                     return True
#             elif(id == self.endId):
#                 if(percent <= self.endPercent):
#                     return True
#         return False

class Segment():
    def __init__(self,parent,pathId:int,id:int,curve:cr.Bezier, points:int, colour, selColour, selIntColour, wb):
        self.id = id
        self.curve = curve
        self.parent = parent
        self.samples = 1 / points
        self.rendered = False
        self.intRendered = False
        self.points = []
        self.lines = []
        self.pathId = pathId
        self.interfaceSize = 10
        self.pointSize = 2
        self.dotColour = colour
        self.intColour = selIntColour
        self.dotColourSel = selColour
        self.lockDirect = False
        self.lockMag = False
        self.lockEDirect = False
        self.lockEMag = False
        self.extend = False
        self.shoot = False
        self.length = 0
        self.wheelbase = wb
        self.backward = False
        self.roller = False
        self.rollerStart = False
        self.intake = False
        self.speed = 14.5
        #self.segments[0].shoot,self.segments[0].extend,self.segments[0].roller,self.segments[0].intake
        
    def saveSegment(self,f):
        f.write("{0},{1},{2},{3},{4},{5},{6},{7},{8},{9},{10},{11},{12},{13},{14},{15},{16}\n".format(
            self.curve.p1.x,self.curve.p1.y,
            self.curve.p2.x,self.curve.p2.y,
            self.curve.ic1.x,self.curve.ic1.y,
            self.curve.ic2.x,self.curve.ic2.y,
            self.lockDirect,self.lockMag,self.lockEDirect,self.lockEMag,
            self.extend,self.shoot,self.wheelbase,self.backward,self.roller
        ))
        
    def readSegment(self,f,pid,id):
        str = f.readline()
        items = str.split(",")
        self.curve.p1.x = float(items[0])
        self.curve.p1.y = float(items[1])
        self.curve.p2.x = float(items[2])
        self.curve.p2.y = float(items[3])
        self.curve.ic1.x = float(items[4])
        self.curve.ic1.y = float(items[5])
        self.curve.ic2.x = float(items[6])
        self.curve.ic2.y = float(items[7])
        if(items[8] == "True" or items[8] == "1"):
            self.lockDirect = True
        if(items[9] == "True" or items[9] == "1"):
            self.lockMag = True
        if(items[10] == "True" or items[10] == "1"):
            self.lockEDirect = True
        if(items[11] == "True" or items[11] == "1"):
            self.lockEMag = True
        if(items[12] == "True" or items[12] == "1"):
            self.extend = True
        if(items[13] == "True" or items[13] == "1"):
            self.shoot = True
        self.wheelbase = float(items[14])
        if(items[15] == "True" or items[15] == "1"):
            self.backward = True
        if(items[16] == "True" or items[16] == "1"):
            self.roller = True
        self.pathId = pid
        self.id = id  

    def setLockDirect(self, value:bool):
        self.lockDirect = value

    def setLockMag(self, value:bool):
        self.lockMag = value

    def setShoot(self, value:bool):
        self.shoot = value

    def setExtend(self, value:bool):
        self.extend = value

    def hide(self):
        for i in self.points:
            self.parent.itemconfig(i,fill='#AAAAAA')
        for i in self.lines:
            self.parent.itemconfig(i,state='hidden')
        if(self.intRendered):
            self.parent.itemconfig(self.start,state='hidden')
            self.parent.itemconfig(self.end,state='hidden')
            self.parent.itemconfig(self.ceo1Line,state='hidden')
            self.parent.itemconfig(self.ceo1Point,state='hidden')
            self.parent.itemconfig(self.ceo2Line,state='hidden')
            self.parent.itemconfig(self.ceo2Point,state='hidden')

    def render(self, sections, pcx, pcy, pch, selected:bool = False):
        """Draws curve"""
        self.length = self.curve.getLength(0.01)
        numItems = self.length / 10 #a dot every 30 pixles
        cnt = 0
        prevx = pcx
        prevy = pcy

        #Empty canvas of this segment's components
        for c in self.points:
            self.parent.delete(c)

        for l in self.lines:
            self.parent.delete(l)

        self.lines = []
        self.points = []
        # print("here")

        for i in np.arange(0,numItems+1,1):
            p = i / numItems
            if(p > 1):
                p = 1
            x = self.curve.mx(p)
            y = self.curve.my(p)
            h = self.curve.getDerivativeAt(p)#math.degrees(math.atan2(prevy - y, prevx - x))
            if(self.backward):
                h += 180
            prev = math.fmod(pch, 360)
            if(prev < 0):
                prev += 360
            #Get angle difference
            dif = math.fmod(prev-h+180, 360)
            if(dif < 0):
                dif += 360
            dif = math.fmod(dif-180,360)
            pch -= dif

            pnt1 = pnt.Pose(x-self.pointSize,y-self.pointSize)
            pnt2 = pnt.Pose(x+self.pointSize,y+self.pointSize)
            
            #Get colour
            col = gradColour(-10,10,dif)
            out = ""
            if(self.backward):
                col = gradColourReverse(-10,10,dif)
            # for s in sections:
            #     if(s.visible):
            #         if(s.isInSection(self.id, i / numItems)):
            #             out = "black"
            #             break
            
            if(prevx != 0):
                ln = self.parent.create_line(x,y,prevx,prevy,fill=col)
                self.lines.append(ln)
            
            obj = self.parent.create_oval(pnt1.x,pnt1.y,pnt2.x,pnt2.y,fill=col,outline=out,tags=("Path:{0}".format(self.pathId),"Segment:{0}".format(self.id),"dot:{0}".format(cnt)))
            self.points.append(obj)
            cnt+=1
            prevx = x
            prevy = y

        #Draw interface items
        if(selected):
            sx = self.curve.p1.x
            sy = self.curve.p1.y
            ex = self.curve.p2.x
            ey = self.curve.p2.y
            
            c1x = self.curve.ic1.x
            c1y = self.curve.ic1.y
            c2x = self.curve.ic2.x
            c2y = self.curve.ic2.y
            col = self.intColour
            if(not(self.intRendered)):
                #Start and end
                self.start = self.parent.create_oval(sx+self.interfaceSize,sy+self.interfaceSize,sx-self.interfaceSize,sy-self.interfaceSize,outline="",fill=col,tags=("Path:{0}".format(self.pathId),"Segment:{0}".format(self.id),"int:start"))
                self.end = self.parent.create_oval(ex+self.interfaceSize,ey+self.interfaceSize,ex-self.interfaceSize,ey-self.interfaceSize,outline="",fill=col,tags=("Path:{0}".format(self.pathId),"Segment:{0}".format(self.id),"int:end"))

                #Start direction
                self.ceo1Line = self.parent.create_line(c1x,c1y,sx,sy,fill=col)
                self.ceo1Point = self.parent.create_oval(c1x+self.interfaceSize,c1y+self.interfaceSize,c1x-self.interfaceSize,c1y-self.interfaceSize,outline="",fill=col,tags=("Path:{0}".format(self.pathId),"Segment:{0}".format(self.id),"int:stceo"))
                
                #End direction
                self.ceo2Line = self.parent.create_line(c2x,c2y,ex,ey,fill=col)
                self.ceo2Point = self.parent.create_oval(c2x+self.interfaceSize,c2y+self.interfaceSize,c2x-self.interfaceSize,c2y-self.interfaceSize,outline="",fill=col,tags=("Path:{0}".format(self.pathId),"Segment:{0}".format(self.id),"int:enceo"))
                self.intRendered = True
            else:
                #Start and end
                self.parent.coords(self.start,sx+self.interfaceSize,sy+self.interfaceSize,sx-self.interfaceSize,sy-self.interfaceSize)
                self.parent.itemconfig(self.start,fill=col,state='normal')
                self.parent.coords(self.end,ex+self.interfaceSize,ey+self.interfaceSize,ex-self.interfaceSize,ey-self.interfaceSize)
                self.parent.itemconfig(self.end,fill=col,state='normal')

                #Start direction
                self.parent.coords(self.ceo1Line,c1x,c1y,sx,sy)
                self.parent.itemconfig(self.ceo1Line,fill=col,state='normal')
                self.parent.coords(self.ceo1Point,c1x+self.interfaceSize,c1y+self.interfaceSize,c1x-self.interfaceSize,c1y-self.interfaceSize)
                self.parent.itemconfig(self.ceo1Point,fill=col,state='normal')

                #End direction
                self.parent.coords(self.ceo2Line,c2x,c2y,ex,ey)
                self.parent.itemconfig(self.ceo2Line,fill=col,state='normal')
                self.parent.coords(self.ceo2Point,c2x+self.interfaceSize,c2y+self.interfaceSize,c2x-self.interfaceSize,c2y-self.interfaceSize)
                self.parent.itemconfig(self.ceo2Point,fill=col,state='normal')
        else:
            if(self.intRendered):
                self.parent.itemconfig(self.start,state='hidden')
                self.parent.itemconfig(self.end,state='hidden')
                self.parent.itemconfig(self.ceo1Line,state='hidden')
                self.parent.itemconfig(self.ceo1Point,state='hidden')
                self.parent.itemconfig(self.ceo2Line,state='hidden')
                self.parent.itemconfig(self.ceo2Point,state='hidden')

        self.rendered = True

class Path():
    def __init__(self, id:int, parent, field, start:pnt.Pose, rw:float, fl:float, interval:float, maxVel:float, absMaxVel:float, maxAccel:float, maxJerk:float, name):
        self.root = parent
        self.id = id
        self.field = field
        self.start = start
        self.end = start
        self.rw = rw
        self.fl = fl
        self.interval = interval
        self.maxVel = maxVel
        self.absMaxVel = absMaxVel
        self.maxAccel = maxAccel
        self.maxJerk = maxJerk
        self.selected = -1
        self.selectedSection = -1
        self.name = name
        self.selInt = -1
        #Define the sgement array
        self.segments = []
        #speed limiter sections -- intake sections included
        self.sections = []
        
        self.profile = []
        self.leftProfile = []
        self.rightProfile = []
        self.trajdots = []
        self.editing = True
        
    def savePath(self, f):
        f.write("Path,{0},{1},{2},{3},{4},{5},{6},{7},{8},{9}\n".format(
            self.name,self.rw,self.fl,self.interval,self.absMaxVel,self.maxVel,self.maxAccel,self.maxJerk,
            len(self.segments),len(self.sections)
        ))
        for s in self.segments:
            s.saveSegment(f)
        # for s in self.sections:
        #     s.saveSection(f)
            
    def readPath(self, f):
        str = f.readline()
        items = str.split(",")
        self.name = items[1]
        self.rw = float(items[2])
        self.fl = float(items[3])
        self.interval = float(items[4])
        self.absMaxVel = float(items[5])
        self.maxVel = float(items[6])
        self.maxAccel = float(items[7])
        self.maxJerk = float(items[8])
        numSegs = int(items[9])
        numSec = int(items[10])

        print(self.id)

        for s in range(0,numSegs):
            seg = Segment(self.field.fieldItems,self.id,s,cr.Bezier(pnt.Pose(100,100),pnt.Pose(100,200),pnt.Pose(300,300),pnt.Pose(300,400)),50,'#ffd700','#af8700','#AA0000',self.field.inchesToPixles(self.rw))
            seg.readSegment(f,0,s)
            self.segments.append(seg)
            
        # for s in range(0,numSec):
        #     sec = SpeedSection(0,0,0,0,0,0)
        #     sec.readSection(f)
        #     self.sections.append(sec)

    def addSegment(self):
        """Adds a curve to the end of the path"""
        #Segments needs to be generated here to lock onto the end of the previous
        if(len(self.segments) == 0): #Make first one
            curve = cr.Bezier(pnt.Pose(100,100),pnt.Pose(100,200),pnt.Pose(300,300),pnt.Pose(300,400))
        else:
            last = self.segments[len(self.segments) - 1].curve
            curve = cr.Bezier(last.p2,pnt.Pose(last.p2.x,last.p2.y+100),pnt.Pose(last.p2.x+200,last.p2.y+200),pnt.Pose(last.p2.x+200,last.p2.y+300))
        self.segments.append(Segment(self.field.fieldItems,self.id,len(self.segments),curve,50,'#ffd700','#af8700','#AA0000',self.field.inchesToPixles(self.rw)))

    def removeSegment(self):
        """Removes the last segment from the path"""
        self.segments[len(self.segments)-1].hide()
        del self.segments[len(self.segments)-1]
        self.selected = -1

    def addSection(self):
        """Adds a curve to the end of the path"""
        # section = SpeedSection(10,0,1,0.0,1.0,'red')
        # self.sections.append(section)
        # self.render()

    def removeSection(self, idx:int):
        """Adds a curve to the end of the path"""
        # self.sections.pop(idx)
        # self.render()
        # self.selectedSection = -1

    def pathClicked(self, tags):
        """React to user clicking on an item"""
        seg = tags[1].split(':')
        self.selectSegment(int(seg[1]))
        item = tags[2].split(':')
        if(item[0] == "int"):
            if(item[1] == "start"):
                self.selInt = 0
            elif(item[1] == "end"):
                self.selInt = 1
            elif (item[1] == "stceo"):
                self.selInt = 2
            elif (item[1] == "enceo"):
                self.selInt = 3
        else:
            self.selInt = -1

    def selectSegment(self, idx:int):
        self.selected = idx
        self.render()

    # def selectSection(self, idx:int):
    #     for s in self.sections:
    #         s.setVisible(False)
    #     self.sections[idx].setVisible(True)
    #     self.selectedSection = idx
    #     self.render()

    # def deselectSection(self):
    #     for s in self.sections:
    #         s.setVisible(False)
    #     self.render()

    def pathDeselect(self):
        self.selected = -1
        self.render()

    def setSegmentLockDirect(self, idx:int, value:bool):
        self.segments[idx].lockDirect = value

    def setSegmentLockMag(self, idx:int, value:bool):
        self.segments[idx].lockMag = value

    def editPath(self):
        self.pathDialog(self.root, self)

    def editSegment(self, idx:int):
        self.SegmentDialog(self.root, self.segments[idx], self.field)
        self.render()

    # def editSection(self, idx:int):
    #     self.SectionDialog(self.root, self.sections[idx])
    #     self.render()

    def pathDragged(self, event):
        """React to user clicking on an item"""
        if(self.selInt == 0):   
            self.segments[self.selected].curve.setStart(pnt.Pose(event.x,event.y))
            #Update previous if there is one
            if(self.selected != 0):
                self.segments[self.selected-1].curve.setEnd(pnt.Pose(event.x,event.y))
        elif(self.selInt == 1):
            self.segments[self.selected].curve.setEnd(pnt.Pose(event.x,event.y))
            #Update next if there is one
            if(self.selected != (len(self.segments) - 1)):
                self.segments[self.selected+1].curve.setStart(pnt.Pose(event.x,event.y))
        elif(self.selInt == 2):
            self.segments[self.selected].curve.setStartDirection(pnt.Pose(event.x,event.y))
            #Update previous if there is one and locks are active
            if(self.selected != 0):
                if(self.segments[self.selected].lockDirect):
                    self.segments[self.selected-1].curve.ic2 = self.calLockDirection(self.selected,self.selected-1,0)
        elif(self.selInt == 3):
            self.segments[self.selected].curve.setEndDirection(pnt.Pose(event.x,event.y))
            #Update previous if there is one and locks are active
            if(self.selected != (len(self.segments) - 1)):
                if(self.segments[self.selected].lockEDirect):
                    self.segments[self.selected+1].curve.ic1 = self.calLockDirection(self.selected,self.selected+1,1,True)
        self.render()

    def calLockDirection(self, c1:int, c2:int, direct:int, end:bool = False):
        curve1 = self.segments[c1].curve
        curve2 = self.segments[c2].curve
        if(direct == 0):
            dx = curve1.ic1.x - curve1.p1.x
            dy = curve1.ic1.y - curve1.p1.y
        else:
            dx = curve1.ic2.x - curve1.p2.x
            dy = curve1.ic2.y - curve1.p2.y
        theta = math.atan2(dy,dx)
        beta = theta + math.pi
        if(direct == 0):
            if(self.segments[c1].backward or self.segments[c2].backward):
                beta = theta
        else:
            if(self.segments[c1].backward or self.segments[c2].backward):
                beta = theta

        if(direct == 0):
            dx2 = curve2.ic2.x - curve2.p2.x
            dy2 = curve2.ic2.y - curve2.p2.y
        else:
            dx2 = curve2.ic1.x - curve2.p1.x
            dy2 = curve2.ic1.y - curve2.p1.y
                
        if(end):
            if(self.segments[c1].lockEMag):
                mag = math.sqrt(dx * dx + dy * dy)
            else:
                mag = math.sqrt(dx2 * dx2 + dy2 * dy2)
        else:
            if(self.segments[c1].lockMag):
                mag = math.sqrt(dx * dx + dy * dy)
            else:
                mag = math.sqrt(dx2 * dx2 + dy2 * dy2)

        if(direct == 0):
            px = curve2.p2.x + mag * math.cos(beta)
            py = curve2.p2.y + mag * math.sin(beta)
        else:
            px = curve2.p1.x + mag * math.cos(beta)
            py = curve2.p1.y + mag * math.sin(beta)

        return pnt.Pose(px,py)

    def render(self):
        """Draws path onto the field"""
        x = 0
        y = 0
        h = 0
        if(not self.editing):
            self.editmode()
        if(len(self.segments) != 0):
            h = self.segments[0].curve.getDerivativeAt(0)
        for i in range(0,len(self.segments)):
            selected = False
            if(self.selected == i):
                selected = True
            self.segments[i].render(self.sections,x,y,h,selected)
            x = self.segments[i].curve.mx(1)
            y = self.segments[i].curve.my(1)

    def hide(self):
        for i in range(0,len(self.segments)):
            self.segments[i].hide()

    class MotionItem():
        def __init__(self):
            self.x = 0
            self.y = 0
            self.percent = 0
            self.vel = 0
            self.accel = 0
            self.time = 0
            self.heading = 0
            self.curve = 0
            self.pos = 0
            self.jerk = 0
            self.shoot = 0
            self.extend = 0
            self.roller = 0
            self.turn = 0
            self.intake = 0
            self.delay = 0
        
        def copy(self,item):
            self.x = item.x
            self.y = item.y
            self.percent = item.percent
            self.vel = item.vel
            self.accel = item.accel
            self.time = item.time
            self.heading = item.heading
            self.curve = item.curve
            self.pos = item.pos
            self.jerk = item.jerk
            self.shoot = item.shoot
            self.extend = item.extend
            self.roller = item.roller
            self.turn = item.turn
            self.intake = item.intake
            self.delay = item.delay
            
    class WayPoint():
        def __init__(self,distance:float,startVel:float,endVel:float,maxVel:float,shoot:bool = False,extend:bool = False, roller:bool = False, intake:bool = False):
            self.distance = distance
            self.startVel = startVel
            self.endVel = endVel
            self.maxVel = maxVel
            self.shoot = shoot
            self.ext = extend
            self.limiter = False
            self.roller = roller
            self.intake = intake
            
        def modify(self,distance:float,startVel:float,endVel:float,maxVel:float,shoot:bool = False,extend:bool = False, roller:bool = False, intake:bool = False):
            self.distance = distance
            self.startVel = startVel
            self.endVel = endVel
            self.maxVel = maxVel
            self.shoot = shoot
            self.ext = extend
            self.roller = roller
            self.intake = intake
            
    def getPercentFromDistance(self,pos:float):
        dist = pos
        cnt = 0
        while((dist >= 0) and (cnt < len(self.segments))):
            dist -= self.segments[cnt].length
            if(dist > 0):
                cnt+=1
        if(cnt >= len(self.segments)):
            cnt = len(self.segments) - 1
        dist += self.segments[cnt].length
        percent = math.fabs(dist / self.segments[cnt].length)
        return cnt,percent

    def editmode(self):
        for c in self.trajdots:
            self.root.delete(c)
        self.trajdots = []
        self.editing = True

    def generate(self):
        """Generates the robot's velocities for the path"""        
        #First thing is to find the waypoints
        #   they exist at the beginning and end of each change in velocity
        #   for example waypoint[0] is the beginning, the next might be at the beginning of a special speed section
        #   waypoint[2] would then be at the end of that special speed section
        #       ---------                2 ----------
        #     /           \            /              \
        #    /             1 ---------                 \
        #   0                                           3______
        
        #Each way point has a few variables
        #   Distance, Start Speed, End Speed, and Max Speed
        #   Acceleration remains the same across all waypoints
        #   For waypoint 0 for example:
        #       Start speed = 0
        #       End speed = 10
        #       Max speed  = 30
        
        #Way points need to change a bit
        #   Distance should be to where the waypoint ends
        #       This can be set when the next waypoint is generated or the end of the path is reached
        #   Speeds will be for that waypoint
        #       Some consideration must be taken when setting the end velocity of the previous
        #           Perhaps only starts of speed sections can modify the end speed of the previous (assuming it isn't shooting or something else)
        #       If the speed of the previous can be modified, and is greater then it can be lowered to the max of the next
        #           Stops it from overriding the shooting period / when switching from forwards to backwards
        #   For backward sections a negative distance will be used
        #   Duplication of positions must be avioded.
        
        #   Look for speed sections
        #       If a start of one is 
        
        #Collect the waypoints -- start at the beginning
        self.waypoints = []
        prev = 0#self.waypoints[0]
        current = self.WayPoint(0,0,0,self.maxVel)
        prevState = -1
        for s in self.segments:      
            #When using segments to control when to change speeds. We can essentialy have a waypoint at the end of each section
            #   We set the start based on the previous end speed, the max, and the end is = to the max unless there is something like shooting or rollers at the end in which case it'll be 0
            dist = self.getDistanceTo(s.id,1) #Gets distance to the end of the current segment
            if(s.backward):
                dist *= -1
            if(s.id != 0):
                startVel = prev.endVel
            else:
                startVel = 0
            maxVel = s.speed
            endVel = maxVel
            #Check for anything at the end of the segment: Shooting, extension, the following is backwards or forwards
            direct = False
            end = False
            next = s.id + 1
            if(next >= len(self.segments)):
                end = True
            else:
                if(s.backward != self.segments[next].backward):
                    direct = True
            roll = False
            if(s.roller):
                #If rollerStart is true than the roller is at the beginning of the segment
                if(not s.rollerStart):
                    roll = True
                else:
                    #Update current and previous to make sure the robot stops
                    if(s.id != 0):
                        prev.endVel = 0
                        startVel = 0
                    else:
                        startVel = 0
            #If we are shooting, extending, changing direction next time or ending the robot must come to a stop
            if(s.shoot or s.extend or direct or end or roll):
                #Robot must come to a stop here
                endVel = 0
                
            #Speeds calculated, now create the new waypoint
            current.modify(dist,startVel,endVel,maxVel,s.shoot,s.extend,s.roller,s.intake)
            self.waypoints.append(current)
            current = self.WayPoint(0,0,0,self.maxVel)
            prev = self.waypoints[len(self.waypoints) - 1]
                
        for w in self.waypoints:
            print(w.distance,w.startVel,w.maxVel,w.endVel,w.shoot,w.ext,w.roller,w.limiter,sep=",")

        #Waypoints have been collected! that was fun
        #Now the trapizodial profile for the robot must be generated.
        #   This will go through each waypoint generating the acceleration and deacceleration for each period
        #       with the max velcoities

        profile = []
        self.rotation = 0
        self.sTime = 0
        self.cidx = 0
        self.tdistance = 0
        self.prevData = self.MotionItem()
        for w in range(0,len(self.waypoints)):
            profile.extend(self.trapizodial(self.waypoints[w]))
            # if(len(profile) != 0):
            #     profile[len(profile)-1].shoot = self.waypoints[w].shoot
            #     profile[len(profile)-1].extend = self.waypoints[w].ext
            #     profile[len(profile)-1].roller = self.waypoints[w].roller

        #The robot's path is generated!
        #Now I need to generate the paths for the left and right side
        #   headings will need to be fixed to stop it being bounded between 180 and -180
        #For the sides of the drivetrain
        #   Position is calculated based on the profile.x & y + heading
        #   The distance between position is used to calculate velocity
        lTraj = []
        rTraj = []
        idx = 0
        lprev = self.MotionItem()
        rprev = self.MotionItem()
        prev = self.MotionItem()
        for p in profile:
            #Copy over the data
            ldata = self.MotionItem()
            ldata.copy(p)
            rdata = self.MotionItem()
            rdata.copy(p)

            #Position information
            cosAngle = math.cos(math.radians(p.heading))
            sinAngle = math.sin(math.radians(p.heading))

            ldata.x = p.x + self.rw / 2 * sinAngle
            ldata.y = p.y + self.rw / 2 * cosAngle

            rdata.x = p.x - self.rw / 2 * sinAngle
            rdata.y = p.y - self.rw / 2 * cosAngle
            
            #Speed information
            if(idx != 0):
                dt = p.heading - prev.heading
                if(dt < 10):
                    r = 0
                    if(dt != 0):
                        r = (self.field.pixlesToInches(p.pos - prev.pos)) / math.radians(dt)
                    
                    w = 0
                    if(r != 0):
                        w = p.vel / r
                    
                    ldist = math.sqrt(((lprev.x - ldata.x)*(lprev.x - ldata.x))+((lprev.y - ldata.y)*(lprev.y - ldata.y)))
                    rdist = math.sqrt(((rprev.x - rdata.x)*(rprev.x - rdata.x))+((rprev.y - rdata.y)*(rprev.y - rdata.y)))
                    ldata.pos = lprev.pos + ldist
                    rdata.pos = rprev.pos + rdist
                    
                    if(w != 0):
                        if(p.vel > 0):
                            ldata.vel = w * (r - (self.rw / 2))
                            rdata.vel = w * (r + (self.rw / 2))
                        else:
                            rdata.vel = w * (r - (self.rw / 2))
                            ldata.vel = w * (r + (self.rw / 2))
                        if((math.fabs(ldata.vel) > 29)or(math.fabs(rdata.vel) > 29)):
                            print("Warning speed higher than maximum generated!")
                    else:
                        ldata.vel = p.vel
                        rdata.vel = p.vel
                    
                    ldata.accel = w
                    rdata.accel = r
                else:
                    #The turn is too drastic, the left and right speeds would have to go to unreachable speeds to make the turn
                    #The robot must turn on the spot -- simply set the turn boolean of the profile at this point
                    p.turn = True
                
                # ldata.accel = (ldata.vel - lprev.vel) / self.interval
                ldata.jerk = (ldata.accel - lprev.accel) / self.interval
                
                # rdata.accel = (rdata.vel - rprev.vel) / self.interval
                rdata.jerk = (rdata.accel - rprev.accel) / self.interval

            lTraj.append(ldata)
            rTraj.append(rdata)
            lprev = lTraj[len(lTraj)-1]
            rprev = rTraj[len(rTraj)-1]
            prev.copy(p)
            idx += 1
            

        #We have to two paths, lets render everything onto the screen
        self.hide() #Hides the current segments -- so they don't get in the way of what is to come
        self.editing = False
        idx = 0

        f = open("./src/paths/{0}.cpp".format(self.name),"w")
        f.write("#include \"path.hpp\"\n\n\nstd::vector<t_motiondata> g_{0} = {{\n".format(self.name))
        predev = profile[0].heading
        for idx in range(0,len(profile)):
            p = profile[idx]
            l = lTraj[idx]
            r = rTraj[idx]

            fpntx, fpnty = self.field.convertToField(p.x,p.y)
            flpntx, flpnty = self.field.convertToField(l.x,l.y)
            frpntx, frpnty = self.field.convertToField(r.x,r.y)

            #Draw the main profile point -- colour is speed
            pnt1 = pnt.Pose(fpntx-2,fpnty-2)
            pnt2 = pnt.Pose(fpntx+2,fpnty+2)
            
            #Get colour
            col = gradColour(-self.maxVel,self.maxVel,p.vel)

            diff = p.heading - predev
            # if(math.fabs(diff) > 0.6):
            #     col = '#A020F0'
            predev = p.heading
            obj = self.root.create_oval(pnt1.x,pnt1.y,pnt2.x,pnt2.y,fill=col,outline="")
            self.trajdots.append(obj)

            col = gradColour(-self.absMaxVel,self.absMaxVel,r.vel)
            #Draw right side and left side
            pnt1 = pnt.Pose(flpntx-2,flpnty-2)
            pnt2 = pnt.Pose(flpntx+2,flpnty+2)
            obj = self.root.create_oval(pnt1.x,pnt1.y,pnt2.x,pnt2.y,fill=col,outline="")
            self.trajdots.append(obj)

            col = gradColour(-self.absMaxVel,self.absMaxVel,l.vel)
            pnt1 = pnt.Pose(frpntx-2,frpnty-2)
            pnt2 = pnt.Pose(frpntx+2,frpnty+2)
            obj = self.root.create_oval(pnt1.x,pnt1.y,pnt2.x,pnt2.y,fill=col,outline="")
            self.trajdots.append(obj)

            #finally output the path
            #time,x,y,h,pos,lx,ly,rx,ry,rvel,lvel
            back = False
            if(p.vel < 0):
                back = True
            flags = IntVar()
            flags = p.shoot | (p.extend << 1) | (back << 2) | (p.roller << 3) | (p.turn << 4) | (p.intake << 5) | (p.delay << 6)
            # print(p.shoot,p.extend,sep=",")
            # print(flags)
            f.write("{{{0},{1},{2},{3},{4},{5},{6}}}".format(round(p.time*100),round(p.x*100),round(p.y*100),round((p.heading+90)*100),round(l.vel*100),round(r.vel*100),flags))
            if(idx != (len(profile)-1)):
                f.write(",")
            f.write("\n")

        f.write("};\n")
        f.close()
                
         
    def getDistanceTo(self,segId:int,percent:float) -> float:
        sum = 0
        for s in range(0,segId,1):
            sum += self.segments[s].length
        sum += self.segments[segId].curve.getLengthTo(percent,0.01)
        return sum
    
    def getAngle(self,seg:int,percent:float,prevAngle:float = 0,direction:bool = False):
        theta = self.segments[seg].curve.getDerivativeAt(percent)
        if(direction):
            theta += 180
        #Constrain the previous to [0,-360]
        prev = math.fmod(prevAngle, 360)
        if(prev < 0):
            prev += 360
        #Get angle difference
        dif = math.fmod(prev-theta+180, 360)
        if(dif < 0):
            dif += 360
        dif = math.fmod(dif-180,360)
        return prevAngle - dif
            
        
    def trapizodial(self,way1:WayPoint):
        """Generates profile between two waypoints"""
        profile = []
        
        #Calculate the cutoff periods
        cutoffStartTime = math.fabs(way1.startVel / self.maxAccel)
        cutoffStartDistance = 0.5 * self.maxAccel * cutoffStartTime * cutoffStartTime

        cutoffEndTime = math.fabs(way1.endVel / self.maxAccel)
        cutoffEndDistance = 0.5 * self.maxAccel * cutoffEndTime * cutoffEndTime
        
        distance = math.fabs(math.fabs(way1.distance) - math.fabs(self.tdistance))

        # Acceleration t = v/d
        accelerationStartTime = (math.fabs(way1.maxVel-way1.startVel)) / self.maxAccel
        accelerationEndTime = (math.fabs(way1.maxVel-way1.endVel))  / self.maxAccel
        # Acceleration distance = 1/2at^2
        accelerationStartDist = 0.5 * self.maxAccel * accelerationStartTime * accelerationStartTime
        accelerationEndDist = way1.endVel * accelerationEndTime + 0.5 * self.maxAccel * accelerationEndTime * accelerationEndTime
        trapezoidDistance = self.field.pixlesToInches(distance)
        if(trapezoidDistance == 0):
            print("Error: Zero distance for way point")
            return profile
        maxDistance = trapezoidDistance - (accelerationStartDist + accelerationEndDist)

        print(accelerationStartTime,accelerationEndTime,accelerationStartDist,accelerationEndDist,trapezoidDistance,maxDistance,sep=",")

        vm = way1.maxVel
        if (maxDistance < 0):
            if(accelerationStartTime > 0):
                vm = math.sqrt((way1.endVel*(2-way1.endVel) + 2 * trapezoidDistance * self.maxAccel)/(2))
                accelerationStartTime = vm / self.maxAccel
                accelerationEndTime = (vm-way1.endVel) / self.maxAccel
            maxDistance = 0
            print(accelerationStartTime,accelerationEndTime,vm,sep=",")


        endAcceleration = accelerationStartTime
        # maxDistance time = (distance / maxVel) - endAccel
        endMax = endAcceleration + math.fabs(maxDistance / way1.maxVel)
        # endDecel = maxDistance + endAccel
        endDeceleration = endMax + accelerationEndTime + self.interval

        startAccelDirection = math.copysign(1, way1.maxVel-way1.startVel)
        endAccelDirection = math.copysign(1, way1.endVel-way1.maxVel)

        direction = math.copysign(1, way1.distance)
        vm *= direction
        startVel = way1.startVel * direction
        endVel = way1.endVel * direction
        endDirection = math.copysign(1, (vm - way1.endVel)) * direction
        maxStartAccel = (self.maxAccel * startAccelDirection) * direction
        maxEndAccel = (self.maxAccel * endAccelDirection) * direction

        vel = 0
        accel = 0
        time = 0
        etime = self.interval
        while(time <= endDeceleration):
            data = self.MotionItem()
            if (time <= endAcceleration):
                vel = startVel + maxStartAccel * time
                if(math.fabs(vel) > math.fabs(vm)):
                    vel = vm
                accel = maxStartAccel
                self.tdistance += self.field.inchesToPixles(math.fabs(vel)) * self.interval
            elif(time < endMax):
                vel = vm
                accel = 0
                self.tdistance += self.field.inchesToPixles(math.fabs(vel)) * self.interval
            elif(time <= endDeceleration):
                vel = vm + maxEndAccel * etime
                if(((vel - endVel)*endDirection) < 0):
                    vel = endVel
                accel = maxEndAccel
                self.tdistance += self.field.inchesToPixles(math.fabs(vel)) * self.interval
                etime += self.interval
            else:
                vel = 0

            #Collect information
            if(vel == 0 and self.sTime != 0):
                self.tDistance = self.getDistanceTo(data.curve,1) + 0.01
            data.curve, data.percent = self.getPercentFromDistance(self.tdistance)
            data.accel = accel
            data.vel = vel
            data.x, data.y = self.field.convertFromField(self.segments[data.curve].curve.mx(data.percent), self.segments[data.curve].curve.my(data.percent))
            data.time = self.sTime
            data.pos = self.tdistance
            data.heading = self.getAngle(data.curve,data.percent,self.prevData.heading,self.segments[data.curve].backward)
            data.intake = way1.intake
            #Roller at the start
            if((time == 0) and (self.segments[data.curve].roller) and (self.segments[data.curve].rollerStart)):
                data.roller = True
            #Add to array
            profile.append(data)
            self.prevData = profile[len(profile)-1]
            
            time += self.interval
            self.sTime += self.interval
        
        #Deal with actionable items like shooting / extentsion / roller
        if((self.segments[data.curve].roller) and (not self.segments[data.curve].rollerStart)):
            self.prevData.roller = True
            self.prevData.time = 20

        if(self.segments[data.curve].shoot):
            #Add turn item prior to the shooting to make sure it is lined up properly
            idx = len(profile)-1
            p = self.MotionItem()
            p.copy(self.prevData)
            p.turn = 1
            p.vel = 0
            profile.insert(idx, p)
            #Add delay after shooting
            p = self.MotionItem()
            p.copy(self.prevData)
            p.time = 10
            p.vel = 0
            p.delay = 1
            p.intake = 0
            profile.append(p)
            self.prevData.shoot = 1

        if(self.segments[data.curve].extend):
            #Add turn item prior to the shooting to make sure it is lined up properly
            idx = len(profile)-2
            p = self.MotionItem()
            p.copy(self.prevData)
            p.turn = 1
            p.vel = 10
            profile.insert(idx, p)
            #Add delay after shooting
            p = self.MotionItem()
            p.copy(self.prevData)
            p.time = 10
            p.vel = 0
            p.delay = 1
            p.intake = 0
            profile.append(p)
            self.prevData.extend = 1

        return profile


    def export(self,filename):
        """Writes the path out to the file"""

    class pathDialog(simpledialog.Dialog):
        def __init__(self, parent, path):
            self.path = path
            self.state = "Cancel"
            super().__init__(parent, "Path Editor")

        def body(self, frame):
            #Name editor
            nameFrame = Frame(frame, bd=1, relief=FLAT)
            
            lbl_name = Label(nameFrame,text="Name:")
            lbl_name.pack(side=LEFT, padx=2, pady=2)
            self.pathName = Entry(nameFrame, width = 28)
            self.pathName.insert(0, self.path.name)
            self.pathName.pack(side=LEFT, padx=2, pady=2)
            
            nameFrame.pack(side=TOP, fill=X)

            #Wheelbase, Length, maxvelocity, max acceration, max jerk, 
            wheelFrame = Frame(frame, bd=1, relief=FLAT)
            
            lbl_wheel = Label(wheelFrame,text="Wheelbase")
            lbl_wheel.pack(side=LEFT, padx=2, pady=2)
            self.pathWheelbase = Entry(wheelFrame, width = 28)
            self.pathWheelbase.insert(0, round(self.path.rw,3))
            self.pathWheelbase.pack(side=LEFT, padx=2, pady=2)
            
            wheelFrame.pack(side=TOP, fill=X)

            lengthFrame = Frame(frame, bd=1, relief=FLAT)
            
            lbl_Length = Label(lengthFrame,text="Length")
            lbl_Length.pack(side=LEFT, padx=2, pady=2)
            self.pathLength = Entry(lengthFrame, width = 28)
            self.pathLength.insert(0, round(self.path.fl,3))
            self.pathLength.pack(side=LEFT, padx=2, pady=2)
            
            lengthFrame.pack(side=TOP, fill=X)

            velocityFrame = Frame(frame, bd=1, relief=FLAT)
            
            lbl_velocity = Label(velocityFrame,text="Max Velocity")
            lbl_velocity.pack(side=LEFT, padx=2, pady=2)
            self.pathvelocity = Entry(velocityFrame, width = 28)
            self.pathvelocity.insert(0, round(self.path.maxVel,3))
            self.pathvelocity.pack(side=LEFT, padx=2, pady=2)
            
            velocityFrame.pack(side=TOP, fill=X)

            AccelFrame = Frame(frame, bd=1, relief=FLAT)
            
            lbl_Accel = Label(AccelFrame,text="Max Accel")
            lbl_Accel.pack(side=LEFT, padx=2, pady=2)
            self.pathAccel = Entry(AccelFrame, width = 28)
            self.pathAccel.insert(0, round(self.path.maxAccel,3))
            self.pathAccel.pack(side=LEFT, padx=2, pady=2)
            
            AccelFrame.pack(side=TOP, fill=X)

            JerkFrame = Frame(frame, bd=1, relief=FLAT)
            
            lbl_Jerk = Label(JerkFrame,text="Max Jerk")
            lbl_Jerk.pack(side=LEFT, padx=2, pady=2)
            self.pathJerk = Entry(JerkFrame, width = 28)
            self.pathJerk.insert(0, round(self.path.maxJerk,3))
            self.pathJerk.pack(side=LEFT, padx=2, pady=2)
            
            JerkFrame.pack(side=TOP, fill=X)

            return frame
        
        def ok_pressed(self):
            self.path.name = self.pathName.get()
            self.path.rw = float(self.pathWheelbase.get())
            self.path.fl = float(self.pathLength.get())
            self.path.maxVel = float(self.pathvelocity.get())
            self.path.maxAccel = float(self.pathAccel.get())
            self.path.maxJerk = float(self.pathJerk.get())
            self.destroy()

        def cancel_pressed(self):
            self.state = "Cancel"
            self.destroy()

        def buttonbox(self):
            self.ok_button = Button(self, text='OK', width=7, command=self.ok_pressed)
            self.ok_button.pack(side="right")
            cancel_button = Button(self, text='Cancel', width=7, command=self.cancel_pressed)
            cancel_button.pack(side="right")
            self.bind("<Return>", lambda event: self.ok_pressed())
            self.bind("<Escape>", lambda event: self.cancel_pressed())

    class SegmentDialog(simpledialog.Dialog):
        def __init__(self, parent, segment, field):
            self.seg = segment
            self.field = field
            self.state = "Cancel"
            super().__init__(parent, "Segment Editor")

        def body(self, frame):
            #Start point, end point, direction start, direction end, shoot at end, locks
            startFrame = Frame(frame, bd=1, relief=FLAT)
            
            px,py = self.field.convertFromField(self.seg.curve.p1.x,self.seg.curve.p1.y)

            lbl_start = Label(startFrame,text="Start: X")
            lbl_start.pack(side=LEFT, padx=2, pady=2)
            self.segStartX = Entry(startFrame, width = 15)
            self.segStartX.insert(0, round(px,3))
            self.segStartX.pack(side=LEFT, padx=2, pady=2)
            lbl_startY = Label(startFrame,text="Y")
            lbl_startY.pack(side=LEFT, padx=2, pady=2)
            self.segStartY = Entry(startFrame, width = 15)
            self.segStartY.insert(0, round(py,3))
            self.segStartY.pack(side=LEFT, padx=2, pady=2)
            
            startFrame.pack(side=TOP, fill=X)

            endFrame = Frame(frame, bd=1, relief=FLAT)

            px,py = self.field.convertFromField(self.seg.curve.p2.x,self.seg.curve.p2.y)
            
            lbl_End = Label(endFrame,text="End: X")
            lbl_End.pack(side=LEFT, padx=2, pady=2)
            self.segEndX = Entry(endFrame, width = 15)
            self.segEndX.insert(0, round(px,3))
            self.segEndX.pack(side=LEFT, padx=2, pady=2)
            lbl_EndY = Label(endFrame,text="Y")
            lbl_EndY.pack(side=LEFT, padx=2, pady=2)
            self.segEndY = Entry(endFrame, width = 15)
            self.segEndY.insert(0, round(py,3))
            self.segEndY.pack(side=LEFT, padx=2, pady=2)
            
            endFrame.pack(side=TOP, fill=X)

            sdFrame = Frame(frame, bd=1, relief=FLAT)

            px,py = self.field.convertFromField(self.seg.curve.ic1.x,self.seg.curve.ic1.y)
            
            lbl_sd = Label(sdFrame,text="Sd: X")
            lbl_sd.pack(side=LEFT, padx=2, pady=2)
            self.segsdX = Entry(sdFrame, width = 15)
            self.segsdX.insert(0, round(px,3))
            self.segsdX.pack(side=LEFT, padx=2, pady=2)
            lbl_sdY = Label(sdFrame,text="Y")
            lbl_sdY.pack(side=LEFT, padx=2, pady=2)
            self.segsdY = Entry(sdFrame, width = 15)
            self.segsdY.insert(0, round(py,3))
            self.segsdY.pack(side=LEFT, padx=2, pady=2)
            
            sdFrame.pack(side=TOP, fill=X)

            edFrame = Frame(frame, bd=1, relief=FLAT)

            px,py = self.field.convertFromField(self.seg.curve.ic2.x,self.seg.curve.ic2.y)
            
            lbl_ed = Label(edFrame,text="Ed: X")
            lbl_ed.pack(side=LEFT, padx=2, pady=2)
            self.segedX = Entry(edFrame, width = 15)
            self.segedX.insert(0, round(px,3))
            self.segedX.pack(side=LEFT, padx=2, pady=2)
            lbl_edY = Label(edFrame,text="Y")
            lbl_edY.pack(side=LEFT, padx=2, pady=2)
            self.eegsdY = Entry(edFrame, width = 15)
            self.eegsdY.insert(0, round(py,3))
            self.eegsdY.pack(side=LEFT, padx=2, pady=2)
            
            edFrame.pack(side=TOP, fill=X)

            speedFrame = Frame(frame, bd=1, relief=FLAT)
            
            lbl_Speed = Label(speedFrame,text="Speed: ")
            lbl_Speed.pack(side=LEFT, padx=2, pady=2)
            self.segSpeed = Entry(speedFrame, width = 20)
            self.segSpeed.insert(0, round(self.seg.speed,3))
            self.segSpeed.pack(side=LEFT, padx=2, pady=2)
            
            speedFrame.pack(side=TOP, fill=X)

            shootFrame = Frame(frame, bd=1, relief=FLAT)
            
            self.shoot = IntVar(value=(self.seg.shoot))
            self.segShoot = Checkbutton(shootFrame, text="Shoot", variable=self.shoot)
            self.segShoot.pack(side=LEFT, padx=2, pady=2)
            
            shootFrame.pack(side=TOP, fill=X)

            intakeFrame = Frame(frame, bd=1, relief=FLAT)
            
            self.intake = IntVar(value=(self.seg.intake))
            self.segIntake = Checkbutton(intakeFrame, text="Intake", variable=self.intake)
            self.segIntake.pack(side=LEFT, padx=2, pady=2)
            
            intakeFrame.pack(side=TOP, fill=X)

            extFrame = Frame(frame, bd=1, relief=FLAT)
            
            self.extValue = IntVar(value=(self.seg.lockEMag))
            self.extCheck = Checkbutton(extFrame, text="Extend", variable=self.extValue)
            self.extCheck.pack(side=LEFT, padx=2, pady=2)
            
            extFrame.pack(side=TOP, fill=X)

            rollFrame = Frame(frame, bd=1, relief=FLAT)
            
            self.lock_roll = IntVar(value=(self.seg.roller))
            self.segroll = Checkbutton(rollFrame, text="Roller", variable=self.lock_roll)
            self.segroll.pack(side=LEFT, padx=2, pady=2)
            
            rollFrame.pack(side=TOP, fill=X)

            rStartFrame = Frame(frame, bd=1, relief=FLAT)
            
            self.rStart = IntVar(value=(self.seg.rollerStart))
            self.segRStart = Checkbutton(rStartFrame, text="Roller at start", variable=self.rStart)
            self.segRStart.pack(side=LEFT, padx=2, pady=2)
            
            rStartFrame.pack(side=TOP, fill=X)

            directFrame = Frame(frame, bd=1, relief=FLAT)
            
            self.lock_d = IntVar(value=(self.seg.lockDirect))
            self.segdirect = Checkbutton(directFrame, text="Direct Lock Start", variable=self.lock_d)
            self.segdirect.pack(side=LEFT, padx=2, pady=2)
            
            directFrame.pack(side=TOP, fill=X)

            MagFrame = Frame(frame, bd=1, relief=FLAT)
            
            self.lock_m = IntVar(value=(self.seg.lockMag))
            self.segMag = Checkbutton(MagFrame, text="Mag Lock Start", variable=self.lock_m)
            self.segMag.pack(side=LEFT, padx=2, pady=2)
            
            MagFrame.pack(side=TOP, fill=X)

            directEFrame = Frame(frame, bd=1, relief=FLAT)
            
            self.lock_de = IntVar(value=(self.seg.lockEDirect))
            self.segdirecte = Checkbutton(directEFrame, text="Direct Lock End", variable=self.lock_de)
            self.segdirecte.pack(side=LEFT, padx=2, pady=2)
            
            directEFrame.pack(side=TOP, fill=X)

            MagEFrame = Frame(frame, bd=1, relief=FLAT)
            
            self.lock_me = IntVar(value=(self.seg.lockEMag))
            self.segMage = Checkbutton(MagEFrame, text="Mag Lock End", variable=self.lock_me)
            self.segMage.pack(side=LEFT, padx=2, pady=2)
            
            MagEFrame.pack(side=TOP, fill=X)

            backFrame = Frame(frame, bd=1, relief=FLAT)
            
            self.lock_back = IntVar(value=(self.seg.backward))
            self.segback = Checkbutton(backFrame, text="Backwards", variable=self.lock_back)
            self.segback.pack(side=LEFT, padx=2, pady=2)
            
            backFrame.pack(side=TOP, fill=X)

            return frame
        
        def ok_pressed(self):
            px,py = self.field.convertToField(float(self.segStartX.get()),float(self.segStartY.get()))
            self.seg.curve.p1.x = px
            self.seg.curve.p1.y = py

            px,py = self.field.convertToField(float(self.segEndX.get()),float(self.segEndY.get()))
            self.seg.curve.p2.x = px
            self.seg.curve.p2.y = py

            px,py = self.field.convertToField(float(self.segsdX.get()),float(self.segsdY.get()))
            self.seg.curve.ic1.x = px
            self.seg.curve.ic1.y = py

            px,py = self.field.convertToField(float(self.segedX.get()),float(self.eegsdY.get()))
            self.seg.speed = float(self.segSpeed.get())
            self.seg.curve.ic2.x = px
            self.seg.curve.ic2.y = py
            self.seg.shoot = self.shoot.get()
            self.seg.lockDirect = self.lock_d.get()
            self.seg.lockMag = self.lock_m.get()
            self.seg.lockEDirect = self.lock_de.get()
            self.seg.lockEMag = self.lock_me.get()
            self.seg.extend = self.extValue.get()
            self.seg.backward = self.lock_back.get()
            self.seg.roller = self.lock_roll.get()
            self.seg.intake = self.intake.get()
            self.seg.rollerStart = self.rStart.get()
            self.destroy()

        def cancel_pressed(self):
            self.state = "Cancel"
            self.destroy()

        def buttonbox(self):
            self.ok_button = Button(self, text='OK', width=7, command=self.ok_pressed)
            self.ok_button.pack(side="right")
            cancel_button = Button(self, text='Cancel', width=7, command=self.cancel_pressed)
            cancel_button.pack(side="right")
            self.bind("<Return>", lambda event: self.ok_pressed())
            self.bind("<Escape>", lambda event: self.cancel_pressed())

    # class SectionDialog(simpledialog.Dialog):
    #     def __init__(self, parent, section):
    #         self.sec = section
    #         self.state = "Cancel"
    #         super().__init__(parent, "Path Editor")

    #     def body(self, frame):
    #         #speed:float,curveStartId:int,curveEndId:int,startPercent:float,endPercent:float, colour
    #         # self.startId = curveStartId
    #         # self.endId = curveEndId
    #         # self.startPercent = startPercent
    #         # self.endPercent = endPercent
    #         # self.speed = speed
    #         # self.colour = colour
    #         # self.visible = True
    #         speedFrame = Frame(frame, bd=1, relief=FLAT)
            
    #         lbl_speed = Label(speedFrame,text="Speed:")
    #         lbl_speed.pack(side=LEFT, padx=2, pady=2)
    #         self.secspeed = Entry(speedFrame, width = 28)
    #         self.secspeed.insert(0, self.sec.speed)
    #         self.secspeed.pack(side=LEFT, padx=2, pady=2)
            
    #         speedFrame.pack(side=TOP, fill=X)

    #         startFrame = Frame(frame, bd=1, relief=FLAT)
            
    #         lbl_start = Label(startFrame,text="Start Segment")
    #         lbl_start.pack(side=LEFT, padx=2, pady=2)
    #         self.secstart = Entry(startFrame, width = 28)
    #         self.secstart.insert(0, self.sec.startId)
    #         self.secstart.pack(side=LEFT, padx=2, pady=2)
            
    #         startFrame.pack(side=TOP, fill=X)

    #         endFrame = Frame(frame, bd=1, relief=FLAT)
            
    #         lbl_end = Label(endFrame,text="End Segment")
    #         lbl_end.pack(side=LEFT, padx=2, pady=2)
    #         self.secend = Entry(endFrame, width = 28)
    #         self.secend.insert(0, self.sec.endId)
    #         self.secend.pack(side=LEFT, padx=2, pady=2)
            
    #         endFrame.pack(side=TOP, fill=X)

    #         stPerFrame = Frame(frame, bd=1, relief=FLAT)
            
    #         lbl_stPer = Label(stPerFrame,text="Start Percent")
    #         lbl_stPer.pack(side=LEFT, padx=2, pady=2)
    #         self.secstPer = Entry(stPerFrame, width = 28)
    #         self.secstPer.insert(0, round(self.sec.startPercent,3))
    #         self.secstPer.pack(side=LEFT, padx=2, pady=2)
            
    #         stPerFrame.pack(side=TOP, fill=X)

    #         enPerFrame = Frame(frame, bd=1, relief=FLAT)
            
    #         lbl_enPer = Label(enPerFrame,text="End Percent")
    #         lbl_enPer.pack(side=LEFT, padx=2, pady=2)
    #         self.secenPer = Entry(enPerFrame, width = 28)
    #         self.secenPer.insert(0, round(self.sec.endPercent,3))
    #         self.secenPer.pack(side=LEFT, padx=2, pady=2)
            
    #         enPerFrame.pack(side=TOP, fill=X)
            
    #         intEFrame = Frame(frame, bd=1, relief=FLAT)
            
    #         self.chk_int = IntVar(value=(self.sec.intake))
    #         self.secInt = Checkbutton(intEFrame, text="Intake", variable=self.chk_int)
    #         self.secInt.pack(side=LEFT, padx=2, pady=2)
            
    #         intEFrame.pack(side=TOP, fill=X)

    #         return frame
        
    #     def ok_pressed(self):
    #         self.sec.speed = float(self.secspeed.get())
    #         self.sec.startId = float(self.secstart.get())
    #         self.sec.endId = float(self.secend.get())
    #         self.sec.startPercent = float(self.secstPer.get())
    #         self.sec.endPercent = float(self.secenPer.get())
            
    #         self.sec.intake = self.chk_int.get()
    #         self.destroy()

    #     def cancel_pressed(self):
    #         self.state = "Cancel"
    #         self.destroy()

    #     def buttonbox(self):
    #         self.ok_button = Button(self, text='OK', width=7, command=self.ok_pressed)
    #         self.ok_button.pack(side="right")
    #         cancel_button = Button(self, text='Cancel', width=7, command=self.cancel_pressed)
    #         cancel_button.pack(side="right")
    #         self.bind("<Return>", lambda event: self.ok_pressed())
    #         self.bind("<Escape>", lambda event: self.cancel_pressed())