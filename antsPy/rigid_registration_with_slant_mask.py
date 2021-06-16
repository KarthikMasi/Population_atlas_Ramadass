#!/usr/bin/env python

import ants
import os
import argparse
import numpy as np


def binarize_mask(mask_path):
    """
    :param  mask: String path Input file for Slant seg file
    :return binarized antsImage
    """
    mask = ants.image_read(mask_path)
    mask_np = mask.numpy()
    binary_mask = np.where(mask_np >0, 0.0,1.0)
    return ants.from_numpy(binary_mask)

def rigid_registration(threet,sevent,mask,fname):
    """
    :param threeT: String path Input file for 3T
    :param sevenT: String path Input file for 7T
    :param fname: String path 7T to 3T registered image
    return none
    """
    fixed_image = ants.image_read(threet)
    moving_image = ants.image_read(sevent)
    transform = ants.registration(fixed_image,moving_image,\
               'Rigid',mask=mask,verbose=True)
    rigid_img = ants.apply_transforms(fixed_image,moving_image,transform['fwdtransforms'])
    ants.image_write(rigid_img,fname)

def add_to_parser():
    """
    Arguments reader
    """
    parser = argparse.ArgumentParser()
    parser.add_argument("--3T", dest='threeT', default=None,\
                        help='Input file for 3T')
    parser.add_argument("--7T",dest='sevenT', default=None,\
                        help='Input file for 7T')
    parser.add_argument("--mask",dest='mask', default=None,\
                        help='T1_seg file from Slant')
    parser.add_argument("--out", dest='outImage',default=None,\
                        help='7T to 3T registered image')
    return parser


if __name__== '__main__':
    parser = add_to_parser()
    OPTIONS = parser.parse_args()
    binary_mask = binarize_mask(OPTIONS.mask)
    rigid_registration(OPTIONS.threeT,OPTIONS.sevenT,binary_mask,OPTIONS.outImage)
