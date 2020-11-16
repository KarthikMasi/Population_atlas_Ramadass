#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkImageRegistrationMethodv4.h"
#include "itkMeanSquaresImageToImageMetricv4.h"
#include "itkBSplineTransform.h"
#include "itkLBFGSBOptimizerv4.h"
#include "itkResampleImageFilter.h"
#include "itkBSplineTransformInitializer.h"

const unsigned int nDims = 3;

typedef itk::Image<double, nDims> ImageType;
typedef itk::Vector<double, nDims> VectorType;
typedef itk::Image<VectorType, nDims> DisplacementFieldType;

typedef itk::ImageFileReader <ImageType> ImageReaderType;
typedef itk::ImageFileWriter <ImageType> ImageWriterType;
typedef itk::LBFGSBOptimizerv4 OptimizerType;
typedef itk::BSplineTransform<double, 3, 3> BSplineTransformType;
typedef itk::BSplineTransformInitializer<BSplineTransformType, ImageType> BSplineInitializer;
typedef itk::MeanSquaresImageToImageMetricv4<ImageType, ImageType> Metric;
typedef itk::ImageRegistrationMethodv4<ImageType, ImageType> Registration;
typedef itk::ResampleImageFilter<ImageType,ImageType> Resample;

class IterationCallback : public itk::Command
{
    public:
        typedef IterationCallback Self;
        typedef itk::Command Superclass;
        typedef itk::SmartPointer<IterationCallback> Pointer;
        itkNewMacro(IterationCallback);
        
       
        typedef const OptimizerType * OptimizerPointerType;


        void Execute(itk::Object * caller, const itk::EventObject & event)
        {
            Execute((const itk::Object *)caller, event);                
        }
        void Execute(const itk::Object * caller, const itk::EventObject & event)
        {
            OptimizerPointerType optimizer = dynamic_cast<OptimizerPointerType> (caller);
            std::cout <<"Iteration: " <<optimizer->GetCurrentIteration() << std::endl;
            std::cout <<"MetricLevel: " <<optimizer->GetCurrentMetricValue() <<std::endl;
            std::cout <<"InfinityNormProjectedGradient: " <<optimizer->GetInfinityNormOfProjectedGradient()<<std::endl;
        }
        
};


int main(int argc, char * argv[])
{
    if(argc < 4)
    {
        std::cerr << "Usage: " << std::endl ;
        std::cerr << argv[0] << " inputMovingImageFile inputFixedImageFile outputRegisteredImageFile" << std::endl ;
        return -1 ;
    }

    const unsigned int numberOfLevels = 1;
    ImageReaderType::Pointer movingReader = ImageReaderType::New();
    ImageReaderType::Pointer fixedReader = ImageReaderType::New();
    ImageWriterType::Pointer outputWriter = ImageWriterType::New();

    movingReader->SetFileName(argv[1]);
    movingReader->Update();
    
    fixedReader->SetFileName(argv[2]);
    fixedReader->Update();
    
    IterationCallback::Pointer callback = IterationCallback::New();
    Registration::Pointer registration = Registration::New();
    OptimizerType::Pointer optimizer = OptimizerType::New();
    Metric::Pointer metric = Metric::New();
    BSplineTransformType::Pointer transform = BSplineTransformType::New();    
    BSplineInitializer::Pointer transform_initializer = BSplineInitializer::New();
    Resample::Pointer resample = Resample::New();

    registration->SetMovingImage(movingReader->GetOutput());
    registration->SetFixedImage(fixedReader->GetOutput());
    registration->SetMetric(metric);
    registration->SetOptimizer(optimizer);

    BSplineTransformType::MeshSizeType mesh_size;
    mesh_size.Fill(2);
    
    transform_initializer->SetTransform(transform);
    transform_initializer->SetImage(fixedReader->GetOutput());
    transform_initializer->SetTransformDomainMeshSize(mesh_size);
    transform_initializer->InitializeTransform();
    
    typedef BSplineTransformType::ParametersType Parameters;
    Parameters parameters(transform->GetNumberOfParameters());
    parameters.Fill(0.0);
    transform->SetParameters(parameters);

    
    registration->SetInitialTransform(transform);
    registration->InPlaceOn();

    Registration::ShrinkFactorsArrayType shrinkFactorsPerLevel;
    shrinkFactorsPerLevel.SetSize(numberOfLevels);
    shrinkFactorsPerLevel[0] = 1;

    Registration::SmoothingSigmasArrayType smoothingSigmasPerLevel;
    smoothingSigmasPerLevel.SetSize(numberOfLevels);
    smoothingSigmasPerLevel[0] = 0;
           
    registration->SetNumberOfLevels(numberOfLevels);
    registration->SetSmoothingSigmasPerLevel(smoothingSigmasPerLevel);
    registration->SetShrinkFactorsPerLevel(shrinkFactorsPerLevel);
    
    const unsigned int numParameters = transform->GetNumberOfParameters();
    OptimizerType::BoundSelectionType boundSelect(numParameters);
    OptimizerType::BoundValueType upperBound(numParameters);
    OptimizerType::BoundValueType lowerBound(numParameters);

    boundSelect.Fill(OptimizerType::UNBOUNDED);
    upperBound.Fill(0.0);
    lowerBound.Fill(0.0);

    optimizer->SetBoundSelection(boundSelect);
    optimizer->SetUpperBound(upperBound);
    optimizer->SetLowerBound(lowerBound);
           
    optimizer->SetCostFunctionConvergenceFactor(1e+12);
    optimizer->SetGradientConvergenceTolerance(1.0e-35);
    optimizer->SetNumberOfIterations(100); //500 changed to 20
    optimizer->SetMaximumNumberOfFunctionEvaluations(500);
    optimizer->SetMaximumNumberOfCorrections(5);

    optimizer->AddObserver(itk::IterationEvent(), callback);
    

    registration->Update();
    
    resample->SetTransform(transform);
    resample->SetInput(movingReader->GetOutput());
    resample->SetSize(fixedReader->GetOutput()->GetLargestPossibleRegion().GetSize());
    resample->SetOutputOrigin(fixedReader->GetOutput()->GetOrigin());
    resample->SetOutputSpacing(fixedReader->GetOutput()->GetSpacing());
    resample->SetOutputDirection(fixedReader->GetOutput()->GetDirection());
    resample->SetDefaultPixelValue(100);

    outputWriter->SetFileName(argv[3]);
    outputWriter->SetInput(resample->GetOutput());
    outputWriter->Update();


    return 0;
}
