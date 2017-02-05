#include "IADEImageManager.h"

#include "IADEOverlay.h"

#include <itkCastImageFilter.h>

IADEImageManager::IADEImageManager(unsigned int numOfImages, QObject * parent)
	:ImageManager(numOfImages, parent)
{
}

bool IADEImageManager::setOverlay(IVtkImageData::itkImageType::Pointer image)
{
	typedef itk::CastImageFilter<IVtkImageData::itkImageType, Overlay::OverlayImageData::itkImageType> CastImageFilter;

	if (image != nullptr) {
		CastImageFilter::Pointer castImageFilter = CastImageFilter::New();
		castImageFilter->SetInput(image);
		castImageFilter->Update();

		m_overlay = QSharedPointer<Overlay>(new IADEOverlay(castImageFilter->GetOutput()));

		return true;
	}
	else
		return false;
}

IADEOverlay * IADEImageManager::getIADEOverlay() const
{
	return static_cast<IADEOverlay*>(m_overlay.data());
}
