#!/usr/bin/env python

import ants
import os
import argparse
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

def deformable_registration(indir,aff_template,outdir):
    """

    """
    fixed_image = ants.image_read(aff_template)
    for subdir,directory,files in os.walk(indir):
        for i,fname in enumerate(files):
            file = subdir+'/'+fname
            print(file)
            moving_image = ants.image_read(file)
            transform = ants.registration(fixed_image,moving_image,\
                                    'SyNOnly',reg_iterations=(60,40,0))
            reg_img = ants.apply_transforms(fixed_image,moving_image,transform['fwdtransforms'])
            ants.image_write(reg_img,outdir+'/'+fname)

def add_to_parser():
    """
    Arguments reader
    """
    parser = argparse.ArgumentParser()
    parser.add_argument("--input_dir", dest='indir', default=None,\
                        help='Input directory with all files')
    parser.add_argument("--deform_dir",dest='deformdir', default=None,\
                        help='Deformably registered images saved here')
    parser.add_argument("--affine_template",dest='afftemp', default=None,\
                        help="Affine template")
    parser.add_argument("--outTemplate", dest='outImage',default=None,\
                        help='Output Image path')
    return parser


if __name__== '__main__':
    parser = add_to_parser()
    OPTIONS = parser.parse_args()
    deformable_registration(OPTIONS.indir,OPTIONS.afftemp,OPTIONS.deformdir)

    avg_img = avg_images(OPTIONS.deformdir)

    #Writing image to outdir
    ants.image_write(avg_img,OPTIONS.outImage)



