import numpy as np
import pose as p
import math

class Bezier():
    def __init__(self, start:p.Pose, sdirect:p.Pose, end:p.Pose, edirect:p.Pose):
        self.p1 = start
        self.p2 = end
        self.ic1 = sdirect
        self.ic2 = edirect

    def setStart(self,pnt:p.Pose):
        self.p1 = pnt

    def setStartDirection(self,pnt:p.Pose):
        self.ic1 = pnt

    def setEnd(self,pnt:p.Pose):
        self.p2 = pnt

    def setEndDirection(self,pnt:p.Pose):
        self.ic2 = pnt
        
    def modify(self, start:p.Pose, sdirect:p.Pose, end:p.Pose, edirect:p.Pose):
        self.p1 = start
        self.p2 = end
        self.ic1 = sdirect
        self.ic2 = edirect

    def getX(self,t:float) -> float:
        t2 = t * t
        t3 = t * t * t
        it2 = (1-t) * (1-t)
        it3 = (1-t) * (1-t) * (1-t)
        return (self.p1.x * it3) + (3 * t * it2 * self.ic1.x) + (3 * t2 * (1-t) * self.ic2.x) + (t3 * self.p2.x)

    def getY(self,t:float) -> float:
        t2 = t * t
        t3 = t * t * t
        it2 = (1-t) * (1-t)
        it3 = (1-t) * (1-t) * (1-t)
        return (self.p1.y * it3) + (3 * t * it2 * self.ic1.y) + (3 * t2 * (1-t) * self.ic2.y) + (t3 * self.p2.y)
    
    def getDerivativeAt(self,t:float) -> float:
        if(not hasattr(self, 'arcLengths')):
            self.getLength(0.01)
        p = self.map(t)
        t2 = p * p
        it2 = (1-p)*(1-p)
        x = (-3*it2*self.p1.x) - (6*(1-p)*p*self.ic1.x) + (3*it2*self.ic1.x) - (3*t2 * self.ic2.x) + (6*(1-p)*p*self.ic2.x) + (3*t2*self.p2.x)
        y = (-3*it2*self.p1.y) - (6*(1-p)*p*self.ic1.y) + (3*it2*self.ic1.y) - (3*t2 * self.ic2.y) + (6*(1-p)*p*self.ic2.y) + (3*t2*self.p2.y)
        return (math.degrees(math.atan2(y,x)) + 360) % 360
    
    def getDerivativeAt2(self,t:float) -> float:
        t2 = t * t
        it2 = (1-t)*(1-t)
        x = (-3*it2*self.p1.x) - (6*(1-t)*t*self.ic1.x) + (3*it2*self.ic1.x) - (3*t2 * self.ic2.x) + (6*(1-t)*t*self.ic2.x) + (3*t2*self.p2.x)
        y = (-3*it2*self.p1.y) - (6*(1-t)*t*self.ic1.y) + (3*it2*self.ic1.y) - (3*t2 * self.ic2.y) + (6*(1-t)*t*self.ic2.y) + (3*t2*self.p2.y)
        return math.degrees(math.atan2(y,x))
    
    def get2ndDerivativeAt(self,t:float) -> float:
        it = 1-t
        x = (6*it*self.p1.x) + (6*t*self.ic1.x) - 6*it*self.ic1.x - 3*self.ic1.x - 6*t*self.ic2.x - 6*t*self.ic2.x + 6*it*self.ic2.x + 6*t*self.p2.x
        y = (6*it*self.p1.y) + (6*t*self.ic1.y) - 6*it*self.ic1.y - 3*self.ic1.y - 6*t*self.ic2.y - 6*t*self.ic2.y + 6*it*self.ic2.y + 6*t*self.p2.y
        return (math.degrees(math.atan2(y,x)))

    def getLength(self, sample:float) -> float:
        """Get total length and lengths of individual sections."""
        self.arcLengths = []
        length = 0
        ox = self.p1.x
        oy = self.p1.y
        j = 0
        for i in np.arange(0,1+sample,sample):
            x = self.getX(i)
            y = self.getY(i)
            dx = ox - x
            dy = oy - y
            length += math.sqrt(dx * dx + dy * dy)
            self.arcLengths.append(length)
            ox = x
            oy = y
            j+=1
        return length
    
    def getLengthTo(self, percent:float, sample:float) -> float:
        """Gets the length to a percentage using samples as resolution"""
        length = 0
        ox = self.p1.x
        oy = self.p1.y
        j = 0
        for i in np.arange(0,percent+sample,sample):
            x = self.getX(i)
            y = self.getY(i)
            dx = ox - x
            dy = oy - y
            length += math.sqrt(dx * dx + dy * dy)
            ox = x
            oy = y
            j+=1
        return length

    def map(self, u:float) -> float:
        target = u * self.arcLengths[len(self.arcLengths)-1]; #Get desired distance based on the total length - the arcLengths is the summed distance from the beginning, so [100] == total length.

        #Binary search algorithm
        low = 0
        high = 100
        index = 0
        while (int(low) < int(high)):
            index = int(low  + ((high - low ) / 2))
            idx = int(index)
            if(self.arcLengths[idx] < target):
                low = int(index + 1)
            else:
                high = int(index)
        
        if(self.arcLengths[int(index)] > target):
            index-=1
        
        #Now that we have our proper index we need to identify the proper percentage to get as close to the desired target.
        lenBefore = self.arcLengths[int(index)]
        if(lenBefore == target):
            return index / 100; #If the target happens to land exactly at the start of this index, the math is very easy.
        else:
            return (index + (target - lenBefore) / (self.arcLengths[int(index) + 1] - lenBefore)) / 100

    def mx(self,u:float) -> float:
        return self.getX(self.map(u))

    def my(self,u:float) -> float:
        return self.getY(self.map(u))