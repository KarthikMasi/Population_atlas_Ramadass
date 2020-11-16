#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkNaryAddImageFilter.h"
#include "itkDivideImageFilter.h"
#include <fstream>
#include <iostream>
#include <experimental/filesystem>
#include <vector>
#include <string>

namespace fs = std::experimental::filesystem;
const unsigned int nDims = 3;

typedef itk::Image <double, nDims> ImageType;
typedef itk::ImageFileReader <ImageType> ImageReaderType;
typedef itk::ImageFileWriter <ImageType> ImageWriterType;
typedef itk::NaryAddImageFilter<ImageType, ImageType>AddImageType;
typedef itk::DivideImageFilter<ImageType,ImageType,ImageType> \
            DivideImageType;

int main(int argc, char * argv[])
{ 
  ImageWriterType::Pointer Kirby_writer = ImageWriterType::New();
  std::vector<std::string> kirby_filenames;
  ImageReaderType::Pointer Kirby[21];
  AddImageType::Pointer kirby_adder=AddImageType::New();
  DivideImageType::Pointer kirby_divider=DivideImageType::New();
  for (auto& p: fs::directory_iterator\
          ("/home/ramadak/KKI2009-ALL-MPRAGE_results/deformable_registered_images_i100"))
  {
    kirby_filenames.push_back(p.path());
  }

  //Have all filenames now. 
  //Reading images with the reader and adding it to the adder object
  //Trying to add all the images on top each other.
  //And divide.

 for (int i=0; i <kirby_filenames.size(); i++)
  {
    Kirby[i]= ImageReaderType::New();
    Kirby[i]->SetFileName(kirby_filenames[i]);
    Kirby[i]->Update();
    kirby_adder->SetInput(i, Kirby[i]->GetOutput());
  }
  kirby_adder->Update();
  kirby_divider->SetInput(kirby_adder->GetOutput());
  //SetConstant(Input2ImagePixelType ct) ----> test 1
  //SetConstant1(const Input1ImagePixelType &input1)l
  //SetConstant2(const Input2ImagePixelType &input2)
  kirby_divider->SetConstant(21.0);
  kirby_divider->Update();
  Kirby_writer->SetFileName\
("/home/ramadak/KKI2009-ALL-MPRAGE_results/deformable_template_i100.nii.gz");
  Kirby_writer->SetInput(kirby_divider->GetOutput());
  Kirby_writer->Update();
  return 0; 
}
