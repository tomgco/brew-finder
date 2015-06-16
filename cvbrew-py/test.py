import cv2
import numpy as np
import argparse
import glob

# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-i", "--images", required=True,
        help="path to input dataset of images")
args = vars(ap.parse_args())

def auto_canny(image, sigma=0.33):
    # compute the median of the single channel pixel intensities
    v = np.median(image)

    accumEdged = np.zeros(image.shape[:2], dtype="uint8")

    # apply automatic Canny edge detection using the computed median
    lower = int(max(0, (1.0 - sigma) * v))
    upper = int(min(255, (1.0 + sigma) * v))
    edged = cv2.Canny(image, lower, upper)
    accumEdged = cv2.bitwise_or(accumEdged, edged)

    # return the edged image
    return accumEdged

def canny_filtered(image, gray):
    blurred = cv2.GaussianBlur(gray, (3, 3), 0)

    auto = auto_canny(blurred)

    (contours, _) = cv2.findContours(auto.copy(), cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
    contours = sorted(contours, key = cv2.contourArea, reverse = True)[:20]

    for c in contours:
        approx = cv2.approxPolyDP(c, 2, True)
        x,y,w,h = cv2.boundingRect(approx)
        item = cv2.rectangle(image, (x, y), (x + w, y + h), (0, 255, 0), 2)
        cv2.drawContours(image, [approx], -1, (0, 255, 0), 3)

    # return np.hstack([auto])
    return [image, auto]

def canny(image):
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    image, filtered = canny_filtered(image, gray)

    return [image, filtered]

def threshold(image):
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    _, thresh = cv2.threshold(gray, 121, 255, cv2.THRESH_TRUNC + cv2.THRESH_OTSU)
    return [image, [gray, thresh]]

def process(x):
    pass

# loop over the images
for imagePath in glob.glob(args["images"] + "/*.jpg"):
    # load the image, convert it to grayscale, and blur it slightly
    image = cv2.imread(imagePath)
    # image, edges = canny(image)
    image, labels = threshold(image)
    image, edge = canny_filtered(image, labels[0])
    # show the images
    cv2.imshow("Edges", np.hstack([edge, np.hstack(labels)]))
    cv2.imshow("Original", image)
    cv2.createTrackbar("", "Original",0,255,process)

    cv2.waitKey(0)
