#!/usr/bin/env python

import ants
import os
import argparse

def affine_registration(threet,sevent,fname):
    """
    :param threeT: String path Input file for 3T
    :param sevenT: String path Input file for 7T
    :param fname: String path 7T to 3T registered image
    return none
    """
    fixed_image = ants.image_read(threet)
    moving_image = ants.image_read(sevent)
    transform = ants.registration(fixed_image,moving_image,\
               'Affine',aff_metric='meansquares')
    affine_img = ants.apply_transforms(fixed_image,moving_image,transform['fwdtransforms'])
    ants.image_write(affine_img,fname)

def add_to_parser():
    """
    Arguments reader
    """
    parser = argparse.ArgumentParser()
    parser.add_argument("--3T", dest='threeT', default=None,\
                        help='Input file for 3T')
    parser.add_argument("--7T",dest='sevenT', default=None,\
                        help='Input file for 7T')
    parser.add_argument("--out", dest='outImage',default=None,\
                        help='7T to 3T registered image')
    return parser


if __name__== '__main__':
    parser = add_to_parser()
    OPTIONS = parser.parse_args()
    affine_registration(OPTIONS.threeT,OPTIONS.sevenT,OPTIONS.outImage)
