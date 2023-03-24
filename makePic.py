import numpy as np
import imageio
import string
import cv2
import os
array = []
with open("data.txt") as file:   
    array = file.readlines()
    n = len(array)
    m = len(array[0].split())
    k = 0
    video_name = 'video.avi'
    width = 51
    height = 51
    fps = 60
    video = cv2.VideoWriter(video_name, 0, fps, (width,height))
    while(k < n//(m + 1) ):
    	im = np.zeros((m, m))
    	for i in range (m): 
    	    for j in range(m):
    	    	if(i == 0):
    	            if(j == 0):
    	            	im[i][j] = 64
    	            elif(j == 1):
    	            	im[i][j] = -64
    	    	else:
    	            im[i][j] = int((array[k * (m + 1) + i].split()[j]))
    	    	#print(len(array[k + i].split()))
    	    	#print(k, k*(m+1), i, j)
    	    		
    	    	#im[i][j] = 64 + int((array[k * (m + 1) + i].split()[j]))
    	    	#im[i][j] = 256
    	
    	path = "pictures" + "/pic" + str(k) + ".png"	
    	print(path)
    	imageio.imwrite(path, im)
    	
    	
    	video.write(cv2.imread(os.path.join("pictures", "pic" + str(k) + ".png")))
    	k += 1

    cv2.destroyAllWindows()
    video.release()
    
