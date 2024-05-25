import java.io.BufferedWriter;
import java.io.FileWriter;

PImage bg;
motionProfile testProfile;
boolean addCon = false;
boolean removeCon = false;
boolean genPath = false;
boolean addRot = false;
boolean expt = false;
int rotState = 0;

float maxVelocity = 40;
float maxAcceleration = 10;
float timeInveral = 0.02;

void setup() {
  background(0);
  size(1000, 750);
  
  bg = loadImage("fieldImage_Rotated.png");
  testProfile = new motionProfile(100,"Test");
}

void draw() {
  background(0);
  background(bg);

  //Side text for motion options.
  fill(0);
  noStroke();
  rect(953,0,147,1100);
  fill(255);
  textSize(18);
  text("maxVel = "+maxVelocity, 857, 51);
  text("maxAccel = "+maxAcceleration, 857, 71);
  text("interval = "+timeInveral, 857, 91);

  
 
  //Interface:
  //Add rotation position to path
  if(!genPath){
    if (keyPressed && (key == 'r') && !addRot){
      addRot = true;
      rotState = 0;
      testProfile.addRotationTarget();
    }
    if(addRot){
      text("Rotation = "+testProfile.rotCount, 857, 150);
      text("Curve = "+testProfile.rData[testProfile.rotCount-1].curve, 857, 170);
      text("Percent = "+testProfile.rData[testProfile.rotCount-1].percent, 857, 190);
      text("Heading = "+testProfile.rData[testProfile.rotCount-1].heading, 857, 210);
      switch(rotState){
        case 0: //Move from curve to curve
          if (keyPressed && (keyCode == UP)){
            testProfile.moveRotationCurve(1);
            delay(1000);
          }
          if (keyPressed && (keyCode == DOWN)){
            testProfile.moveRotationCurve(-1);
            delay(1000);
          }
          if (mousePressed)
            rotState++;
          break;
        case 1:
          if (!mousePressed)
            rotState++;
          break;
        case 2:
          //Curve percent + rotation
          if (keyPressed && (keyCode == UP))
            testProfile.moveRotationPercent(0.01,0);
          if (keyPressed && (keyCode == DOWN))
            testProfile.moveRotationPercent(-0.01,0);
          if (keyPressed && (keyCode == RIGHT))
            testProfile.moveRotationPercent(0,1);
          if (keyPressed && (keyCode == LEFT))
            testProfile.moveRotationPercent(0,-1);
          if (mousePressed)
            rotState++;
          break;
        case 3:
          //Done
          addRot = false;
          rotState = 0;
          break;
      }
    }
  }
  //exportPath
  if (keyPressed && (key == 'p') && !expt){
    testProfile.exportPath();
    expt = true;
  }

  // + adds a new curve onto the path
  if (keyPressed && (key == '+') && !addCon){
    testProfile.addCurve();
    addCon = true;
  }

  // - removes the last curve
  if (keyPressed && (key == '-') && !removeCon){
    //revmove curve
    testProfile.removeCurve();
    removeCon = true;
  }

  //Profile editor to move sections of the curves.
  testProfile.editor();


  if (keyPressed && (key == 'g') && !genPath){
    //Generate the motion path
    testProfile.generatePath(maxVelocity,maxAcceleration,timeInveral);
    genPath = true;
  }
  else{
    if(genPath)
      testProfile.renderMotionPath();
    else
      testProfile.renderCurves();
    
  }

  //Switch back to the curve editor when 'e' is pressed.
  if (keyPressed && (key == 'e') && genPath){
    genPath = false;
  }
  //Reset switches when nothing is pressed
  if (!keyPressed){
    addCon = false;
    expt = false;
    removeCon = false;
  }
}

void mouseWheel(MouseEvent event) {
  float e = event.getCount();
  //Motion Settings
  if((mouseX > 857) && (mouseX < 1100)){
    if((mouseY >= 31)&&(mouseY < 51))
      maxVelocity += e;
    if((mouseY >= 51)&&(mouseY < 71))
      maxAcceleration += (e * 0.1);
    if((mouseY >= 71)&&(mouseY < 91))
      timeInveral += (e * 0.01);
  }
}


//*************************************************************************************************
class XYPose {
 public float x,y;
 
 XYPose(){
   x = 0;
   y = 0;
 }
 
 XYPose(float X, float Y){
   x = X;
   y = Y;
 }
}

//100 = 1 foot = 12 in
float inchToPixle(float in){return ((in / 12) * 61);}
float pixleToIn(float pixle){return ((pixle / 61) * 12);}
float mToIn(float m){return (m * 39.37);}
float inToM(float in){return (in / 39.37);}

float convertXToField(float x){return (inchToPixle(x) + 7);}
float convertYToField(float y){return (743 - inchToPixle(y));}

float convertRadiansToField(float h) {return (h - (PI / 2));}
float convertDegreesToField(float h) {return (h - 90);}
XYPose convertToField(XYPose pose){return new XYPose(convertXToField(pose.x),convertYToField(pose.y));}



//*************************************************************************************************
class BezierCurve{
  public XYPose p1;
  public XYPose p2;
  public XYPose c1;
  public XYPose c2;
  public float length;

  private float[] arcLengths;
  
  public BezierCurve(XYPose ip1, XYPose ip2, XYPose ic1, XYPose ic2){
    p1 = ip1;
    p2 = ip2;
    c1 = ic1;
    c2 = ic2;
  }

  public float getX(float t){
    float t2 = t * t;
    float t3 = t * t * t;
    float it2 = (1-t) * (1-t);
    float it3 = (1-t) * (1-t) * (1-t);
    return (p1.x * it3) + (3 * t * it2 * c1.x) + (3 * t2 * (1-t) * c2.x) + (t3 * p2.x);
  }

  public float getY(float t){
    float t2 = t * t;
    float t3 = t * t * t;
    float it2 = (1-t) * (1-t);
    float it3 = (1-t) * (1-t) * (1-t);
    return (p1.y * it3) + (3 * t * it2 * c1.y) + (3 * t2 * (1-t) * c2.y) + (t3 * p2.y);
  }

  public float getLength(float sample){
    //Get total length and lengths of individual sections.
    arcLengths = new float[101];
    length = 0;
    float ox = p1.x, oy = p1.y;
    int j = 0;
    for(float i=0;i<1;i+=0.01){
      float x = getX(i), y = getY(i);
      float dx = ox - x, dy = oy - y;
      length += sqrt(dx * dx + dy * dy);
      arcLengths[j] = length;
      ox = x;
      oy = y;
      j++;
    }
    return length;
  }

  private float map(float u) {
    float target = u * arcLengths[100]; //Get desired distance based on the total length - the arcLengths is the summed distance from the beginning, so [100] == total length.

    //Binary search algorithm
    int low = 0, high = 100, index = 0;
    while (low < high){
      index = low + ((high - low) / 2);
      if(arcLengths[index] < target)
        low = index + 1;
      else
        high = index;
    }
    if(arcLengths[index] > target)
      index--;
    
    //Now that we have our proper index we need to identify the proper percentage to get as close to the desired target.
    float lenBefore = arcLengths[index];
    if(lenBefore == target)
      return index / 100; //If the target happens to land exactly at the start of this index, the math is very easy.
    else
      return (index + (target - lenBefore) / (arcLengths[index + 1] - lenBefore)) / 100;
  }

  public float mx(float u){
    return getX(map(u));
  }

  public float my(float u){
    return getY(map(u));
  }
}



//*************************************************************************************************
class motionData{
  public float x;
  public float y;
  public float heading;
  public float vel;
  public float accel;
  public boolean intake;
  public boolean fly;
  public float percent;
  public float time;

  public motionData(){}
}

class rotationPoint{
  public float x;
  public float y;
  public float heading;
  public int curve;
  public float percent;
  public int motionIndex;
  public rotationPoint(){}
}

class motionProfile{
  private BezierCurve curves[];
  private motionData mData[];
  public rotationPoint rData[];
  public int rotCount = 0;
  private int motionCount = 0;
  private int curveCount = 0;
  private int curveMax = 0;
  private int editorIdx = -1;
  private int editorItem = -1;
  public int size = 5;
  public float sampling = 0.02;
  public String name;

  public motionProfile(int numWayPnts, String nm){
    curves = new BezierCurve[numWayPnts];
    rData = new rotationPoint[100];
    name = nm;
    curveMax = numWayPnts;
    curveCount = 0;
    rotCount = 0;
  }

  public void addCurve(){
    if(curveCount == curveMax)
      return;
    XYPose start = new XYPose();
    XYPose end = new XYPose();
    XYPose c1 = new XYPose();
    XYPose c2 = new XYPose();
    if(curveCount != 0)
      start = curves[curveCount - 1].p2;
    else{
      start.x = convertXToField(48);
      start.y = convertYToField(24);
    }    
    end.x = start.x + 100;
    end.y = start.y + 100;
    c1.x = start.x + 50;
    c1.y = start.y;
    c2.x = end.x + 50;
    c2.y = end.y;
    
    BezierCurve curve = new BezierCurve(start,end,c1,c2);

    curves[curveCount] = curve;
    curveCount++;
  }
  void removeCurve(){
    if(curveCount != 0)
      curveCount--;
  }

  void addRotationTarget(){
    //Find position on curve nearest given points.
    rData[rotCount] = new rotationPoint();
    rData[rotCount].curve = 0;
    rData[rotCount].percent = 0;
    rData[rotCount].heading = 0;
    rData[rotCount].motionIndex = 0;
    rotCount++;
  }

  void moveRotationCurve(int direction){
    rData[rotCount-1].curve += direction;
    if(rData[rotCount-1].curve < 0)
      rData[rotCount-1].curve = 0;
    if(rData[rotCount-1].curve == curveCount)
      rData[rotCount-1].curve = curveCount - 1;
  }

  void moveRotationPercent(float direction, int rotation){
    rData[rotCount-1].percent += direction;
    if(rData[rotCount-1].percent > 1)
      rData[rotCount-1].percent = 1;
    if(rData[rotCount-1].percent < 0)
      rData[rotCount-1].percent = 0;
    rData[rotCount-1].heading += rotation;
  }

  void exportPath(){
    //Open Path.hpp
    File f = new File(dataPath("../../src/Auto/Paths.hpp"));
    try {
      PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter(f, true)));
      out.println("extern t_motion "+name+"[];");
      out.close();
    }catch (IOException e){
      e.printStackTrace();
    }

    File p = new File(dataPath("../../src/Auto/"+name+".cpp"));
    try {
      PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter(p, true)));
      out.println("#include \"Motion.hpp\"");
      out.println("");
      out.println("std::vector<coachLib::t_motion> const "+name+" = {");
      for(int i=0;i<motionCount;i++){
        float x = pixleToIn((mData[i].x - 50));
        float y = pixleToIn((mData[i].y - 20));
        float h = mData[i].heading;
        float v = pixleToIn(mData[i].vel);
        float a = pixleToIn(mData[i].accel);
        int in = 0;
        int fly = 0;

        out.println("{"+mData[i].time+","+x+","+y+","+h+","+v+","+a+","+in+","+fly+"}"+((i == (motionCount-1)) ? "" : ","));
      }
      out.println("};");
      out.close();
    }catch (IOException e){
      e.printStackTrace();
    }
  }

  void generatePath(float maxVel, float maxAccel, float t){
    //Gen 2 for inline drive
    //Waypoints can have stop identifiers
    //  So the code needs to be able to create multiple trapoids
    //    Can also look at implementation of the S-Curve
    //The first iteration layout the speeds for the robot along the sections
    //The second iteration calculates the speeds of the right and left sides of the robot



    //Starting speed is 0 and it must accelerate.
    float vel = 0;
    float accel = 0;

    //Update each curve's total length:
    float total = 0;
    for(int i=0;i<curveCount;i++)
      total += curves[i].getLength(0.01);
    
    float accelTime = maxVel / maxAccel;
    float fullSpeedTime = total - (accelTime * accelTime * maxAccel);
    float accelDist = 0.5 * maxAccel * accelTime * accelTime;

    if(accelTime > (total / 2)){
      accelTime = sqrt(abs(total / maxAccel));
      accelDist = 0.5 * maxAccel * accelTime * accelTime;
    }

    float endAccel = accelTime;
    float endFullSpeed = endAccel + abs(fullSpeedTime / maxVel);
    float endDeccel = endFullSpeed + accelTime;

    int curve = 0; //Current curve robot is traversing
    int state = 0;
    float tm = 0; //Current time tracker
    float decTm = 0;
    float deccelTime = 0;
    float prevDist = 0; //The previous distance the robot has traversed.
    float distance = 0; //The current distance the robot traversed based on speed and acceleration.
    XYPose pnt = new XYPose(); //Robot's current location
    boolean done = false; //Something to indicate when the generation is complete.

    int ind = round(endDeccel / t);
    motionCount = ind + 50;
    mData = new motionData[motionCount];
    for(int r=0;r<rotCount;r++){
      rData[r].motionIndex = 0;
    }
    ind = 0;
    motionCount = 0;
    while(!done){
      switch(state){
        case 0:
          //Acceleration period
          vel = maxAccel * tm;
          accel = maxAccel;
          prevDist = 0.5 * maxAccel * tm * tm;
          if(tm >= accelTime)
            state++;
          break;
        case 1:
          //Max speed
          vel = maxVel;
          accel = 0;
          prevDist += vel * t;
          if(tm > endFullSpeed){
            state++;
            distance = prevDist;
          }
          break;
        case 2:
          //Deacceleration
          deccelTime = endDeccel - tm;
          if(deccelTime < 0)
            deccelTime = 0;
          vel = maxAccel * deccelTime;
          accel = -maxAccel;
          distance = vel * t;
          prevDist += distance;
          if(deccelTime <= 0)
            done = true;
          break;
      }
      
      //Get position on curve.
      float percent = prevDist / curves[curve].length;
      if(percent > 1){
        float carry = prevDist - curves[curve].length;
        curve += 1;
        prevDist = carry;
        if(curve >= curveCount)
          return;
        percent = carry / curves[curve].length;
      }
      mData[ind] = new motionData();
      mData[ind].x = curves[curve].mx(percent);
      mData[ind].y = curves[curve].my(percent);
      mData[ind].percent = percent;
      mData[ind].vel = vel;
      mData[ind].accel = accel;
      mData[ind].time = tm;
      mData[ind].heading = 0;
      if(ind != 0)
        mData[ind-1].heading = (degrees(atan2((mData[ind].y - mData[ind-1].y),(mData[ind].x - mData[ind-1].x))));
      if(done)
        mData[ind].heading = mData[ind-1].heading;

      for(int r=0;r<rotCount;r++){
        float d = dist(mData[ind].x,mData[ind].y,rData[r].x,rData[r].y);
        float d1 = dist(mData[rData[r].motionIndex].x,mData[rData[r].motionIndex].y,rData[r].x,rData[r].y);
        if (d < d1){
          rData[r].motionIndex = ind;
        }
      }

      tm += t;
      ind++;
    }

    motionCount = ind;

    //The amount of distance from one point to the next will determine the speed of the right and left side.

    //Rotation
    // for(int r=0;r<rotCount;r++){
    //   int idx1 = rData[r].motionIndex;
    //   int idx2 = ind-1;
    //   float theta = 0;
    //   if(r < (rotCount-1)){
    //     idx2 = rData[r+1].motionIndex;
    //     theta = rData[r+1].heading - rData[r].heading;
    //   }

    //   int count = idx2 - idx1;
    //   float delta = theta / count;
    //   for(int i=idx1;i<idx2;i++){
    //     mData[i].heading = rData[r].heading + (delta * (i-idx1));
    //   }
    //   mData[idx2].heading = rData[r].heading;
    //   if(r < (rotCount-1)){
    //     mData[idx2].heading = rData[r+1].heading;
    //   }
    // }
  }

  void renderMotionPath(){
    
    for(int i=0;i<(motionCount-2);i++){
      if(mData[i] == null)
        return;
      fill(255,255,255);
      noStroke();
      ellipse(mData[i].x,mData[i].y,size,size);

      //right side
      float lx = mData[i].x + 15 * cos(radians(mData[i].heading + 90));
      float ly = mData[i].y + 15 * sin(radians(mData[i].heading + 90));
      fill(0,255,0);
      ellipse(lx,ly,size,size);

      //Left speed
      lx = mData[i].x + 15 * cos(radians(mData[i].heading - 90));
      ly = mData[i].y + 15 * sin(radians(mData[i].heading - 90));
      fill(0,255,0);
      ellipse(lx,ly,size,size);

      //Rotation
      // float lx = mData[i].x + 15 * cos(radians(mData[i].heading));
      // float ly = mData[i].y + 15 * sin(radians(mData[i].heading));
      // stroke(255);
      // line(mData[i].x,mData[i].y,lx,ly);
    }        
  }

  boolean renderCurves(){
    for(int i=0;i<curveCount;i++){
      strokeWeight(0);
      stroke(0,0,0);
      curves[i].getLength(0.01);
      for(float j=0;j<=1;j+=sampling){
        float x = curves[i].mx(j);
        float y = curves[i].my(j);
        //println(j,",",x,",",y);
        fill(255,165,0);
        ellipse(x,y,size,size);        
      }
      strokeWeight(1);
      fill(0,0,100);
      rect(curves[i].p1.x-10,curves[i].p1.y-10,20,20);
      fill(100,0,100);
      rect(curves[i].p2.x-10,curves[i].p2.y-10,20,20);
      fill(0,100,100);
      stroke(0,100,100);
      rect(curves[i].c1.x-10,curves[i].c1.y-10,20,20);
      line(curves[i].c1.x,curves[i].c1.y,curves[i].p1.x,curves[i].p1.y);
      fill(100,100,0);
      stroke(100,100,0);
      rect(curves[i].c2.x-10,curves[i].c2.y-10,20,20);
      line(curves[i].c2.x,curves[i].c2.y,curves[i].p2.x,curves[i].p2.y);
    }
    fill(100,0,0);
    stroke(0);
    for(int r=0;r<rotCount;r++){
      rData[r].x = curves[rData[r].curve].mx(rData[r].percent);
      rData[r].y = curves[rData[r].curve].my(rData[r].percent);

      float lx = rData[r].x + 25 * cos(radians(rData[r].heading));
      float ly = rData[r].y + 25 * sin(radians(rData[r].heading));

      ellipse(rData[r].x,rData[r].y,15,15);
      line(rData[r].x,rData[r].y,lx,ly);
    }
    return true;
  }

  void checkForEdits(){
    if ((mousePressed && (mouseButton == LEFT)) && (editorIdx == -1)) {
      for(int i=0;i<curveCount;i++){
        if(dist(curves[i].p1.x-10,curves[i].p1.y-10,mouseX,mouseY) < 20){
          editorIdx = i;
          editorItem = 0;
          break;
        }
        if(dist(curves[i].p2.x-10,curves[i].p2.y-10,mouseX,mouseY) < 20){
          editorIdx = i;
          editorItem = 1;
          break;
        }
        if(dist(curves[i].c1.x-10,curves[i].c1.y-10,mouseX,mouseY) < 20){
          editorIdx = i;
          editorItem = 2;
          break;
        }
        if(dist(curves[i].c2.x-10,curves[i].c2.y-10,mouseX,mouseY) < 20){
          editorIdx = i;
          editorItem = 3;
          break;
        }
      }
    }
    if (!mousePressed){
      editorIdx = -1;
      editorItem = -1;
    }
  }

  void editor(){
    checkForEdits();
    if(editorIdx == -1)
      return;
    if(editorItem == -1)
      return;

    switch(editorItem){
      case 0:
        curves[editorIdx].p1.x = mouseX;
        curves[editorIdx].p1.y = mouseY;
        if(editorIdx != 0){
          curves[editorIdx-1].p2.x = mouseX;
          curves[editorIdx-1].p2.y = mouseY;
        }
        break;
      case 1:
        curves[editorIdx].p2.x = mouseX;
        curves[editorIdx].p2.y = mouseY;
        if(editorIdx != (curveCount - 1)){
          curves[editorIdx+1].p1.x = mouseX;
          curves[editorIdx+1].p1.y = mouseY;
        }
        break;
      case 2:
        curves[editorIdx].c1.x = mouseX;
        curves[editorIdx].c1.y = mouseY;
        break;
      case 3:
        curves[editorIdx].c2.x = mouseX;
        curves[editorIdx].c2.y = mouseY;
        break;
    }
  }
}
