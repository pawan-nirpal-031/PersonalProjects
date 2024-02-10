# Important NOTE: Use opencv >=4.4 
import cv2

# Loading the image
img = cv2.imread('/home/panirpal/workspace/Projects/ComputerVision/data/images/astro1.jpg')

# Converting image to grayscale
gray= cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)

# Applying SIFT detector
sift = cv2.SIFT_create()
kp = sift.detect(gray, None)

# Marking the keypoint on the image using circles
img=cv2.drawKeypoints(gray ,
					kp ,
					img)

cv2.imwrite('image-with-keypoints.jpg', img)
