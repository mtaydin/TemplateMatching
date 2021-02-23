import cv2
import numpy as np
import sys
import argparse
import time
def parser():
    parser = argparse.ArgumentParser(description="Locating a Defined Area in the Star Map")
    parser.add_argument("--reference", type=str,
                        help="Reference Image Path")
    parser.add_argument("--template", type=str,
                        help="Template Image Path")
    parser.add_argument("--threshold",type=float, default=0.7,
                        help="Matching Threshold")

    return parser.parse_args()

def rotate_image(image, angle):

  image_center = tuple(np.array(image.shape[1::-1]) / 2)  #rotate function returning mat object with parametres imagefile and angle
  rot_mat = cv2.getRotationMatrix2D(image_center, angle, 1.0) #Mat object for storing after rotation
  result = cv2.warpAffine(image, rot_mat, image.shape[1::-1], flags=cv2.INTER_LINEAR) # apply an affine transformation to image.
  return result # returning Mat format for output image file

if __name__ == '__main__':

    start_time=time.time()
    args = parser()
    reference = cv2.imread(args.reference)  # read Reference Image
    template = cv2.imread(args.template)  # read Template Image
    threshold = args.threshold # matching threshold
    for rotate in range(0, 360, 30):#try rotation angles
        template = rotate_image(template, rotate)  #apply rotation
        h, w = template.shape[0], template.shape[1] # get h and w value from template for draw rectangle

        res = cv2.matchTemplate(reference, template, cv2.TM_CCOEFF_NORMED) # calculate  Correlation coefficient (TM_CCOEFF)

        (_, max_val, _, pt) = cv2.minMaxLoc(res) #get best matching coord 

        if max_val>=threshold:
            
            cv2.rectangle(reference, pt, (pt[0] + w, pt[1] + h), (0, 255, 0), 2)  # draw rectangle
             # Write positions
            cv2.putText(reference, f"[{pt[0]},{pt[1]}]", (pt[0] - 150, pt[1]), cv2.FONT_HERSHEY_SIMPLEX, 0.9,
                        (36, 255, 12), 1) 
            cv2.putText(reference, f"[{pt[0] + w},{pt[1]}]", ((pt[0] + w) + 15, pt[1]), cv2.FONT_HERSHEY_SIMPLEX, 0.9,
                        (36, 255, 12), 1)
            cv2.putText(reference, f"[{pt[0]},{pt[1] + h}]", (pt[0] - 150, pt[1] + h), cv2.FONT_HERSHEY_SIMPLEX, 0.9,
                        (36, 255, 12), 1)
            cv2.putText(reference, f"[{pt[0] + w},{pt[1] + h}]", ((pt[0] + w) + 15, pt[1] + h),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.9,
                        (36, 255, 12), 1)



    print("Total Process Time : {} seconds".format(round(float(time.time()-start_time),2)))
    cv2.imwrite("results/template_match_result.png", reference)
    cv2.imshow("result", reference) #show position of template on reference image
    cv2.waitKey(0)#exit with any key

