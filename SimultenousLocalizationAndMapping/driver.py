import cv2
import numpy as np

H = 2160//4
W = 3840//4 


def display(img):
    cv2.imshow('frame',img)
    if(cv2.waitKey(20) and 0xFF==ord('q')):
        return
    
class featureExtracter(object):
    gx = 8
    gy = 6
    def __init__(self):
        self.orb = cv2.ORB_create()

    def extractBasic(self,img):
        y = img.shape[0]
        x = img.shape[1]
        sy = y//self.gy
        sx = x//self.gx
        absKeyPoints = []
        for ry in range(0,y,sy):
            for rx in range(0,x,sx):
                # extraxt the sub image to run feature detector faster instead of pixel by pixel on the whole image
                subImage = img[ry:ry+sy,rx:rx+sx]
                # detect intresting key points, points that are trackable through frames
                subImgKeyPoints = self.orb.detect(subImage,None)
                for p in subImgKeyPoints:
                    # converting subimage keypoint to absolute key point
                    p.pt = (p.pt[0]+rx, p.pt[1]+ry)
                    absKeyPoints.append(p)
        return absKeyPoints
    
    def extractFast(self,img):
        features =  cv2.goodFeaturesToTrack(np.mean(img,axis=2).astype(np.uint8),3000,qualityLevel=0.01,minDistance=3)
        self.orb.compute(img,features)

fe = featureExtracter()
def processFrame(img):
    img = cv2.resize(img,(W,H))
    imgKeyPoints = fe.extractFast(img)
    for pt in imgKeyPoints:
        u,v = map(lambda x:int(round(x)),pt[0])
        cv2.circle(img,(u,v),color=(0,255,0),radius=3)
    display(img)
   
if (__name__=="__main__"):
    # native resolution : 2160 3840
    cap = cv2.VideoCapture("drive-vid.mp4")
    while(cap.isOpened()):
        ret,frame = cap.read()
        if(ret==True):
            processFrame(frame)
        else:
            break