#include "InteractorStyleSurfaceCenterLineSimpleClipping.h"

#include "CenterlineSurfaceViewer.h"

#include <vtkObjectFactory.h>
#include <vtkClipPolyData.h>
#include <vtkBox.h>
#include <vtkImageData.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkThreshold.h>
#include <vtkDataSetAttributes.h>
#include <vtkGeometryFilter.h>
#include <vtkButtonWidget.h>
#include <vtkTexturedButtonRepresentation2D.h>
#include <vtkFreeTypeUtilities.h>
#include <vtkTextProperty.h>
#include <vtkImageData.h>
#include <vtkCallbackCommand.h>
#include <vtkProperty2D.h>

#include <vtkvmtkCapPolyData.h>
#include <vtkvmtkPolyDataCenterlines.h>


vtkStandardNewMacro(InteractorStyleSurfaceCenterLineSimpleClipping);
const char InteractorStyleSurfaceCenterLineSimpleClipping::RADIUS[] = "Radius";
void InteractorStyleSurfaceCenterLineSimpleClipping::SetCustomEnabled(bool flag)
{
	AbstractSurfaceCenterLine::SetCustomEnabled(flag);
	if (flag) {
		vtkSmartPointer<vtkImageData> reClipButton =
			vtkSmartPointer<vtkImageData>::New();
		vtkSmartPointer<vtkImageData> ChangeSourceButton =
			vtkSmartPointer<vtkImageData>::New();

		vtkSmartPointer<vtkTextProperty> textProperty = 
			vtkSmartPointer<vtkTextProperty>::New();
		textProperty->SetColor(1.0, 1.0, 1.0); 
		textProperty->SetFontSize(15);
		vtkSmartPointer<vtkFreeTypeUtilities> freeType = vtkSmartPointer<vtkFreeTypeUtilities>::New();
		freeType->RenderString(textProperty, "Re-generate", reClipButton);
		freeType->RenderString(textProperty, "Change Source", ChangeSourceButton);

		vtkSmartPointer<vtkCallbackCommand> reClipCallback =
			vtkSmartPointer<vtkCallbackCommand>::New();
		reClipCallback->SetClientData(this);
		reClipCallback->SetCallback([](vtkObject*, unsigned long, void* clientData, void* calldata) {
			InteractorStyleSurfaceCenterLineSimpleClipping* self =
				reinterpret_cast<InteractorStyleSurfaceCenterLineSimpleClipping*>(clientData);
			self->CreateCenterLine(true);
		});



		double bds1[6] = { 
			0, 
			reClipButton->GetExtent()[1], 
			15, 
			15 + reClipButton->GetExtent()[3], 
			0, 
			0 };

		m_reClipButtonRep = vtkSmartPointer<vtkTexturedButtonRepresentation2D>::New();
		m_reClipButtonRep->GetProperty()->SetOpacity(1);
		m_reClipButtonRep->SetNumberOfStates(1);
		m_reClipButtonRep->SetButtonTexture(0, reClipButton);
		m_reClipButtonRep->SetPlaceFactor(1);
		m_reClipButtonRep->PlaceWidget(bds1);

		m_reClipButtonWidget = vtkSmartPointer<vtkButtonWidget>::New();
		m_reClipButtonWidget->SetRepresentation(m_reClipButtonRep);
		m_reClipButtonWidget->SetInteractor(this->Interactor);
		m_reClipButtonWidget->AddObserver(vtkCommand::StateChangedEvent, reClipCallback);
		m_reClipButtonWidget->EnabledOn();


		vtkSmartPointer<vtkCallbackCommand> changeSourceCallback =
			vtkSmartPointer<vtkCallbackCommand>::New();
		changeSourceCallback->SetClientData(this);
		changeSourceCallback->SetCallback([](vtkObject*, unsigned long, void* clientData, void* calldata) {
			InteractorStyleSurfaceCenterLineSimpleClipping* self =
				reinterpret_cast<InteractorStyleSurfaceCenterLineSimpleClipping*>(clientData);
			self->CreateCenterLine(false);
		});

		double bds2[6] = {
			0,
			ChangeSourceButton->GetExtent()[1],
			30 + reClipButton->GetExtent()[3],
			30 + reClipButton->GetExtent()[3] + ChangeSourceButton->GetExtent()[3],
			0,
			0 };
		m_ChangeSourceButtonRep = vtkSmartPointer<vtkTexturedButtonRepresentation2D>::New();
		m_ChangeSourceButtonRep->GetProperty()->SetOpacity(1);
		m_ChangeSourceButtonRep->SetNumberOfStates(1);
		m_ChangeSourceButtonRep->SetButtonTexture(0, ChangeSourceButton);
		m_ChangeSourceButtonRep->SetPlaceFactor(1);
		m_ChangeSourceButtonRep->PlaceWidget(bds2);


		m_ChangeSourceButtonWidget = vtkSmartPointer<vtkButtonWidget>::New();
		m_ChangeSourceButtonWidget->SetRepresentation(m_ChangeSourceButtonRep);
		m_ChangeSourceButtonWidget->SetInteractor(this->Interactor);
		m_ChangeSourceButtonWidget->AddObserver(vtkCommand::StateChangedEvent, changeSourceCallback);
		m_ChangeSourceButtonWidget->EnabledOn();
	}
	else {
		if (m_reClipButtonWidget) {
			m_reClipButtonWidget->EnabledOff();
		}
		m_reClipButtonWidget = nullptr;
		m_reClipButtonRep = nullptr;
		if (m_ChangeSourceButtonWidget) {
			m_ChangeSourceButtonWidget->EnabledOff();
		}
		m_ChangeSourceButtonWidget = nullptr;
		m_ChangeSourceButtonRep = nullptr;
	}
}

void InteractorStyleSurfaceCenterLineSimpleClipping::SetCenterlineOrientation(int Orientation)
{
	this->m_centerLineOrientation = Orientation;
}

int InteractorStyleSurfaceCenterLineSimpleClipping::GetCenterlineOrientation()
{
	return m_centerLineOrientation;
}

void InteractorStyleSurfaceCenterLineSimpleClipping::SetClipingDistantce(int Distance)
{
	m_ClipingDistance = Distance;
}

int InteractorStyleSurfaceCenterLineSimpleClipping::GetClipingDistance()
{
	return m_ClipingDistance;
}

void InteractorStyleSurfaceCenterLineSimpleClipping::SetLumenLabel(int label)
{
	this->m_lumenLabel = label;
}

int InteractorStyleSurfaceCenterLineSimpleClipping::GetLumenDistance()
{
	return this->m_lumenLabel;
}

InteractorStyleSurfaceCenterLineSimpleClipping::InteractorStyleSurfaceCenterLineSimpleClipping()
{
	m_ClipAndCapSurface = vtkSmartPointer<vtkPolyData>::New();
	m_capCenterIds = vtkSmartPointer<vtkIdList>::New();
}

InteractorStyleSurfaceCenterLineSimpleClipping::~InteractorStyleSurfaceCenterLineSimpleClipping()
{
}

bool InteractorStyleSurfaceCenterLineSimpleClipping::CreateCenterLine()
{

	return CreateCenterLine(true);

}

bool InteractorStyleSurfaceCenterLineSimpleClipping::CreateCenterLine(bool reClipSurface)
{
	if (!GetSurfaceViewer()->GetInput()) {
		vtkErrorMacro(<< "SurfaceViewer has no input");
		return false;
	}
	if (reClipSurface) {
		m_sourceIdId = 0;
		ClipAndCap();
	}
	else {
		if (!m_capCenterIds || m_capCenterIds->GetNumberOfIds() <= 0) {
			vtkErrorMacro(<< "no cap surface or center. ");
			vtkErrorMacro(<< "re cap anyway");
			m_sourceIdId = 0;
			ClipAndCap();
		}
		else if (m_sourceIdId < m_capCenterIds->GetNumberOfIds() - 1) {
			++m_sourceIdId;
		}
		else {
			m_sourceIdId = 0;
		}
	}
	if (!m_capCenterIds || m_capCenterIds->GetNumberOfIds() <= 0) {
		vtkErrorMacro(<< "still no cap surface or center. ");
		return false;
	}

	vtkSmartPointer<vtkIdList> sourceIds = nullptr;
	vtkSmartPointer<vtkIdList> targetIds = nullptr;

	sourceIds = vtkSmartPointer<vtkIdList>::New();
	sourceIds->InsertNextId(m_capCenterIds->GetId(m_sourceIdId));

	targetIds = vtkSmartPointer<vtkIdList>::New();
	for (int i = 0; i < m_capCenterIds->GetNumberOfIds(); ++i) {
		if (i != m_sourceIdId) {
			targetIds->InsertNextId(m_capCenterIds->GetId(i));
		}
	}

	vtkSmartPointer<vtkvmtkPolyDataCenterlines> centerlinesFilter = vtkSmartPointer<vtkvmtkPolyDataCenterlines>::New();
	centerlinesFilter->SetInputData(m_ClipAndCapSurface);
	centerlinesFilter->SetSourceSeedIds(sourceIds);
	centerlinesFilter->SetTargetSeedIds(targetIds);
	//centerlinesFilter->AppendEndPointsToCenterlinesOn();
	centerlinesFilter->SetRadiusArrayName(RADIUS);
	//centerlinesFilter->
	//centerlinesFilter->SetEdgeArrayName("Edge");
	//centerlinesFilter->SetEdgePCoordArrayName("PCoord");
	centerlinesFilter->Update();

	//vtkSmartPointer<vtkCleanPolyData> cleanPolyData =
	//	vtkSmartPointer<vtkCleanPolyData>::New();
	//cleanPolyData->SetInputConnection(centerlinesFilter->GetOutputPort());
	//cleanPolyData->PointMergingOn();
	//cleanPolyData->Update();
	GetCenterlineSurfaceViewer()->GetCenterline()->ShallowCopy(centerlinesFilter->GetOutput());
	GetCenterlineSurfaceViewer()->Render();
	//m_centerLine->ShallowCopy();
	return true;
}

void InteractorStyleSurfaceCenterLineSimpleClipping::ClipAndCap()
{
	double bounds[6];
	memcpy(bounds, GetSurfaceViewer()->GetSurfaceActor()->GetBounds(), sizeof(bounds));
	bounds[m_centerLineOrientation * 2] = bounds[m_centerLineOrientation * 2]
		+ GetSpacing()[m_centerLineOrientation] * m_ClipingDistance;
	bounds[m_centerLineOrientation * 2 + 1] = bounds[m_centerLineOrientation * 2 + 1]
		- GetSpacing()[m_centerLineOrientation] * m_ClipingDistance;
	vtkSmartPointer<vtkBox> clipBox =
		vtkSmartPointer<vtkBox>::New();
	clipBox->SetBounds(bounds);

	vtkSmartPointer<vtkThreshold> Threshold =
		vtkSmartPointer<vtkThreshold>::New();
	Threshold->SetInputData(GetSurfaceViewer()->GetSurfaceActor()->GetMapper()->GetInput());
	Threshold->ThresholdBetween(m_lumenLabel, m_lumenLabel);
	Threshold->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS, vtkDataSetAttributes::SCALARS);
	Threshold->Update();

	vtkSmartPointer<vtkGeometryFilter> GeometryFilter =
		vtkSmartPointer<vtkGeometryFilter>::New();
	GeometryFilter->SetInputConnection(Threshold->GetOutputPort());
	GeometryFilter->Update();


	vtkSmartPointer<vtkClipPolyData> clipPolyData =
		vtkSmartPointer<vtkClipPolyData>::New();
	clipPolyData->SetInputConnection(GeometryFilter->GetOutputPort());
	clipPolyData->SetClipFunction(clipBox);
	clipPolyData->InsideOutOn();
	clipPolyData->Update();

	vtkSmartPointer<vtkvmtkCapPolyData> capPolyData =
		vtkSmartPointer<vtkvmtkCapPolyData>::New();
	capPolyData->SetInputConnection(clipPolyData->GetOutputPort());
	capPolyData->Update();

	m_ClipAndCapSurface->ShallowCopy(capPolyData->GetOutput());

	m_capCenterIds = capPolyData->GetCapCenterIds();

}

void InteractorStyleSurfaceCenterLineSimpleClipping::OnKeyPress()
{
	std::string key = this->Interactor->GetKeySym();
	cout << key << endl;
	if (key == "space") {
		CreateCenterLine(false);
		//VisualizeCenterLine(m_centerLine);
	}
	else if (key == "Tab") {
		if (m_centerLineOrientation != CENTER_LINE_ORIENTATION_XY) {
			m_centerLineOrientation++;
		}
		else
		{
			m_centerLineOrientation = CENTER_LINE_ORIENTATION_YZ;
		}
		CreateCenterLine(true);
		//VisualizeCenterLine(m_centerLine);
	}
	else {
		AbstractSurfaceCenterLine::OnKeyPress();
	}
}

