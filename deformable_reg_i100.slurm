#!/bin/bash
#SBATCH --mail-user=karthik.ramadass@vanderbilt.edu
#SBATCH --mail-type=ALL
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=2:00:00 #2 hours
#SBATCH --array=1,2,3,4,5,6,7,8,9,10,12,13,14,16,18,23,26,28,30,32,41 
#SBATCH -o /home/ramadak/KKI2009-ALL-MPRAGE_results/deformable_logs_i100/KKI2009-%A_%a.out
#SBATCH --mem=8000mb
#SBATCH --account=cs8395-oguz

module purge
module restore cs83950modules

/home/ramadak/deformable_reg_build/deformable_registration /home/ramadak/KKI2009-ALL-MPRAGE_results/affine_registered_images_11_10/KKI2009-${SLURM_ARRAY_TASK_ID}-MPRAGE.nii.gz /home/ramadak/KKI2009-ALL-MPRAGE_results/affine_template_11_10.nii.gz /home/ramadak/KKI2009-ALL-MPRAGE_results/deformable_registered_images_i100/KKI2009-${SLURM_ARRAY_TASK_ID}-MPRAGE.nii.gz
