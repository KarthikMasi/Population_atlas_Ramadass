#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegistrationMethod.h"
#include "itkAffineTransform.h"
#include "itkMeanSquaresImageToImageMetric.h"
#include "itkRegularStepGradientDescentOptimizer.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkResampleImageFilter.h"
#include "itkMultiResolutionImageRegistrationMethod.h"
#include "itkCommand.h"
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
typedef itk::AffineTransform<double, 3> AffineTransform;
//typedef itk::ImageRegistrationMethod<ImageType, ImageType> Registration;
typedef itk::MultiResolutionImageRegistrationMethod<ImageType, ImageType> Registration;
typedef itk::MeanSquaresImageToImageMetric<ImageType, ImageType> Metric;
typedef itk::RegularStepGradientDescentOptimizer OptimizerType;
typedef itk::LinearInterpolateImageFunction<ImageType> Interpolator;
typedef itk::ResampleImageFilter<ImageType,ImageType> Resample;

class OptimizerIterationCallback : public itk::Command
{
    public:
        typedef OptimizerIterationCallback Self;
        typedef itk::Command Superclass;
        typedef itk::SmartPointer<OptimizerIterationCallback> Pointer ;
        itkNewMacro(OptimizerIterationCallback);
        typedef itk::RegularStepGradientDescentOptimizer OptimizerType ;
        typedef const OptimizerType * OptimizerPointerType ;

        void Execute(itk::Object * caller, const itk::EventObject & event)
        {
            Execute((const itk::Object *)caller, event);              
        }
        void Execute(const itk::Object * caller, const itk::EventObject & event)
        {
            OptimizerPointerType optimizer = \
                 dynamic_cast<OptimizerPointerType> (caller);
            std::cout << optimizer->GetCurrentIteration() \
                << " " << optimizer->GetValue() << std::endl;
        }       
}
;

class RegistrationIterationCallback : public itk::Command
{
public:
  // itk set up such that things like pointers and new() are available for this class
  //OptimizerIterationCallback::Pointer myCallback = OptimizerIterationCallback::New() ;
  // standard things we need in every itk-inheriting class
  typedef RegistrationIterationCallback Self ;
  typedef itk::Command Superclass ;
  typedef itk::SmartPointer<RegistrationIterationCallback> Pointer ;
  itkNewMacro(RegistrationIterationCallback);

  // specific typedefs we need for our observer
  typedef OptimizerType * OptimizerPointerType ;
  
  typedef itk::MultiResolutionImageRegistrationMethod < ImageType, ImageType > RegistrationMethodType ;
  typedef RegistrationMethodType * RegistrationPointerType ;

  // if i want to change things in my caller
 void
 Execute(itk::Object * caller, const itk::EventObject & event) 
  {
    // somehow get my hands on the registration method 
    // caller is the registration method
    RegistrationPointerType registration = dynamic_cast < RegistrationPointerType > ( caller ) ;
    std::cout << "Level: " << registration->GetCurrentLevel () << std::endl ;

    OptimizerPointerType optimizer = dynamic_cast < OptimizerPointerType > ( registration->GetModifiableOptimizer() ) ;

    optimizer->SetMaximumStepLength ( optimizer->GetMaximumStepLength() * 0.5 ) ;
    /*
    if ( registration->GetCurrentLevel() == 0 )
      optimizer->SetMaximumStepLength ( 0.25 ) ;
    else if ( registration->GetCurrentLevel() == 1 )
      optimizer->SetMaximumStepLength ( 0.125 ) ;
    else
      optimizer->SetMaximumStepLength ( 0.125 ) ;
    */
  }
 
  // if i am just observing the caller (no changes)
  void
  Execute(const itk::Object * caller, const itk::EventObject & event) 
  {
    // nothing
  }
};





int main(int argc, char * argv[])
{ 
    ImageWriterType::Pointer Kirby_writer[20];
    std::vector<std::string> kirby_filenames;
    std::vector<std::string> kirby_id;
    OptimizerIterationCallback::Pointer myCallback[20];
    RegistrationIterationCallback::Pointer myRegCallback[20]; 
    ImageReaderType::Pointer fixed_reader = ImageReaderType::New();
    ImageReaderType::Pointer moving_reader[20];
    AffineTransform::Pointer affine[20];
    Registration::Pointer registration[20];
    OptimizerType::Pointer optimizer[20];
    Metric::Pointer metric[20];
    Interpolator::Pointer interpolator[20];
    Resample::Pointer resample[20];

    for (auto& p: fs::directory_iterator\
          ("/home/ramadak/KKI2009-ALL-MPRAGE"))
    {
        kirby_filenames.push_back(p.path());
        kirby_id.push_back(p.path().filename());
    }
    std::cout<<"Registering all to "<< kirby_filenames[0] <<std::endl;
    fixed_reader->SetFileName(kirby_filenames[0]);
    fixed_reader->Update();
    std::cout<<""<<kirby_filenames[0]<<" loaded as fixed image" <<std::endl;
  //Have all filenames now. 
  //Reading images with the reader and adding it to the adder object
  //Trying to add all the images on top each other.
  //And divide
  
  //Need 
  //Affine transform type
  //Registration type
  //StepGradient
  //Metric
  //Interpolator
  //Resample

    for (int i=1; i<kirby_filenames.size(); i++)
    {
        std::cout<<"Registering " << kirby_filenames[i] << " now" <<std::endl;
        moving_reader[i]= ImageReaderType::New();
        moving_reader[i]->SetFileName(kirby_filenames[i]);
        moving_reader[i]->Update();
        Kirby_writer[i] = ImageWriterType::New();
        affine[i] = AffineTransform::New();
        registration[i] = Registration::New();
        optimizer[i] = OptimizerType::New();
        metric[i] = Metric::New();
        interpolator[i] = Interpolator::New();
        resample[i] = Resample::New();
        myCallback[i] = OptimizerIterationCallback::New();
        myRegCallback[i]  = RegistrationIterationCallback::New();

        
        registration[i]->SetMovingImage(moving_reader[i]->GetOutput());
        registration[i]->SetFixedImage(fixed_reader->GetOutput());
        registration[i]->SetOptimizer(optimizer[i]);
        registration[i]->SetMetric(metric[i]);
        registration[i]->SetInterpolator(interpolator[i]);
        registration[i]->SetTransform(affine[i]);
        registration[i]->SetInitialTransformParameters(affine[i]->GetParameters());
        registration[i]->SetFixedImageRegion\
            (fixed_reader->GetOutput()->GetLargestPossibleRegion());
        registration[i]->SetNumberOfLevels(3);
       
        optimizer[i]->SetMinimumStepLength(0);
        optimizer[i]->SetMaximumStepLength(0.125);
        optimizer[i]->SetNumberOfIterations(50);
        affine[i]->SetIdentity();

        optimizer[i]->AddObserver(itk::IterationEvent(), myCallback[i]);
        registration[i]->AddObserver(itk::IterationEvent(), myRegCallback[i]);

        registration[i]->Update();

        resample[i]->SetInput(moving_reader[i]->GetOutput());
        resample[i]->SetTransform(affine[i]);
        resample[i]->SetReferenceImage(fixed_reader->GetOutput());
        resample[i]->UseReferenceImageOn();
        resample[i]->Update();

        Kirby_writer[i]->SetFileName("/home/ramadak/KKI2009-ALL-MPRAGE_results/affine_registered_images_11_10/"+kirby_id[i]);
        Kirby_writer[i]->SetInput(resample[i]->GetOutput());
        Kirby_writer[i]->Update();
    }   

    return 0; 
}
