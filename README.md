# Population_atlas_Ramadass

To execute the code for the C++ code, the paths will have to be changed from within the script for averaging and affine registration and can be run without arguments.


For deformable registration, the usage is
./deformable_registration moving image file input fixed image file output registered image file

For ANTsPy, install the libraries in the requirements.txt file.

(antspy) ramadak@DESKTOP-MK1MSLU:~/git_repo/Population_atlas_Ramadass/antsPy$ p
ython affine_registration.py --help
usage: affine_registration.py [-h] [--input_dir INDIR] [--affine_dir AFFDIR]
                              [--outTemplate OUTIMAGE]

optional arguments:
  -h, --help            show this help message and exit
  --input_dir INDIR     Input directory with all files
  --affine_dir AFFDIR   Affine registered images saved here
  --outTemplate OUTIMAGE
                        Output Image path
(antspy) ramadak@DESKTOP-MK1MSLU:~/git_repo/Population_atlas_Ramadass/antsPy$ python deformable_registration.py --help
usage: deformable_registration.py [-h] [--input_dir INDIR]
                                  [--deform_dir DEFORMDIR]
                                  [--affine_template AFFTEMP]
                                  [--outTemplate OUTIMAGE]

optional arguments:
  -h, --help            show this help message and exit
  --input_dir INDIR     Input directory with all files
  --deform_dir DEFORMDIR
                        Deformably registered images saved here
  --affine_template AFFTEMP
                        Affine template
  --outTemplate OUTIMAGE
                        Output Image path
                        
(antspy) ramadak@DESKTOP-MK1MSLU:~/git_repo/Population_atlas_Ramadass/antsPy$ p
ython average_image.py --help
usage: average_image.py [-h] [--input_dir INDIR] [--out OUTIMAGE]

optional arguments:
  -h, --help         show this help message and exit
  --input_dir INDIR  Input directory with all files
  --out OUTIMAGE     Output Image path
