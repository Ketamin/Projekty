import matplotlib.pyplot as plt
from skimage.transform import resize
import numpy as np

ascii_char = " .',:;+*?%S#@"

def to_gray(img):
    """Takes img and change colors to grayscale"""
    R, G, B = img[:,:,0], img[:,:,1], img[:,:,2]
    img = 0.29 * R + 0.58 * G + 0.11 * B
    return img

def change_range(img):
    """Takes img and changes range of colors to new values"""

    old_min = 0
    old_max = np.max(img)
    # Use global char list
    new_max = len(ascii_char) - 1
    new_min = 0

    r, k = np.shape(img)[0], np.shape(img)[1]
    #img = img[:,:,0]
    for i in range(r):
        for j in range(k):
            img[i,j] = (((img[i,j] - old_min) * (new_max - new_min)) / (old_max - old_min)) + new_min

    img = np.round(img).astype(int)

    return img

def img_to_ascii(img):
    """Takes img and returns 2d matrix of ASCII char similar to img"""

    img = resize(img, (img.shape[0]//10, img.shape[1]//10))

    img = to_gray(img)
    img = change_range(img)

    r, k = img.shape[0], img.shape[1]

    ascii_matrix = np.empty((r, k), dtype=str)

    for i in range(r):
        for j in range(k):
            ascii_matrix[i, j] = ascii_char[img[i, j]]

    return ascii_matrix

if __name__ == '__main__':

    img = plt.imread('kot.jpg')

    ascii_img = img_to_ascii(img)

    np.savetxt('img_ascii.txt', ascii_img, fmt='%s')