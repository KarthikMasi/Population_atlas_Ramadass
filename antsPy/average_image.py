#!/usr/bin/env python

import ants
import os
import argparse

def add_to_parser():
    """
    Arguments reader
    """
    parser = argparse.ArgumentParser()
    parser.add_argument("--input_dir", dest='indir', default=None,\
                        help='Input directory with all files')
    parser.add_argument("--out", dest='outImage',default=None,\
                        help='Output Image path')
    return parser

def avg_images(indir):
    """
    Input directory string
    return: averaged ANTsPy image after adding and dividing by 21
    """
    for subdir,directory,files in os.walk(indir):
        for i,fname in enumerate(files):
            file = subdir+'/'+fname
            if(i==0):
                add_accumulator = ants.image_read(file)
            else:
                add_component = ants.image_read(file)
                add_accumulator = add_accumulator + add_component
    averaged_image = add_accumulator/21.0
    return averaged_image

if __name__== '__main__':
    parser = add_to_parser()
    OPTIONS = parser.parse_args()
    avg_img = avg_images(OPTIONS.indir)

    #Writing image to outdir
    ants.image_write(avg_img,OPTIONS.outImage)



