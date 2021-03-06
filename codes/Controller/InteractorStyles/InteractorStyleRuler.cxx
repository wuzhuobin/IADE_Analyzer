#include "InteractorStyleRuler.h"

#include <vtkAxisActor2D.h>
#include <vtkCellArray.h>
#include <vtkProperty2D.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <InteractorStyleSwitch.h>
#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation2D.h>
#include <vtkPointSetToLabelHierarchy.h>
#include <vtkStringArray.h>
#include <vtkLabelPlacementMapper.h>
#include <vtkObjectFactory.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkImageViewer2.h>
#include <vtkProperty.h>
#include <vtkPointHandleRepresentation3D.h>
#include <vtkImageActorPointPlacer.h>
//#include <vtkPointHandleRepresentation2D.h>
//#include <vtkDistanceRepresentation2D.h>



vtkStandardNewMacro(InteractorStyleRuler);

void InteractorStyleRuler::SetCustomEnabled(bool flag)
{
	InteractorStyleNavigation::SetCustomEnabled(flag);
	if (flag)
	{
		vtkImageActorPointPlacer* imageActorPointPlacer =
			vtkImageActorPointPlacer::New();
		imageActorPointPlacer->SetImageActor(GetVtkImageViewer2()->GetImageActor());

		vtkPointHandleRepresentation3D* pointHandleRepresentation3D = 
			vtkPointHandleRepresentation3D::New();
		pointHandleRepresentation3D->SetPointPlacer(imageActorPointPlacer);
		imageActorPointPlacer->Delete();

		vtkDistanceRepresentation2D* distanceRepresentation2D = 
			vtkDistanceRepresentation2D::New();
		distanceRepresentation2D->SetLabelFormat("%-#11.2f mm");
		distanceRepresentation2D->SetHandleRepresentation(pointHandleRepresentation3D);
		pointHandleRepresentation3D->Delete();

		if (!m_distanceWidget) {
			m_distanceWidget = vtkDistanceWidget::New();
		}
		m_distanceWidget->SetRepresentation(distanceRepresentation2D);
		distanceRepresentation2D->Delete();
		m_distanceWidget->SetInteractor(this->Interactor);

		//m_distanceWidget->SetRepresentation(m_distanceRepresentation3D);

		//m_pointHandleRepresentation2D = vtkPointHandleRepresentation2D::New();

		//distanceRepresentation2D->SetHandleRepresentation(m_pointHandleRepresentation2D);
		//m_distanceWidget->SetRepresentation(distanceRepresentation2D);
		//m_distanceWidget->GetDistanceRepresentation()->InstantiateHandleRepresentation();
		m_distanceWidget->EnabledOn();

		//UpdateMaximumWallThicknessLabel();

	}
	else
	{
		if (m_distanceWidget != nullptr) {
			m_distanceWidget->SetInteractor(nullptr);
			m_distanceWidget->EnabledOff();
			m_distanceWidget->Delete();
			m_distanceWidget = nullptr;
		}
		//if (distanceRepresentation2D) {
		//	distanceRepresentation2D->Delete();
		//	distanceRepresentation2D = nullptrptr;
		//}
		//if (GetVtkImageViewer2()->GetRenderer()->HasViewProp(this->m_lineActor)) {
		//	GetVtkImageViewer2()->GetRenderer()->RemoveActor(this->m_lineActor);

		//}
		//if (GetVtkImageViewer2()->GetRenderer()->HasViewProp(this->m_labelActor)) {
		//	GetVtkImageViewer2()->GetRenderer()->RemoveActor(this->m_labelActor);
		//}
		GetVtkImageViewer2()->Render();
	}
}

//void InteractorStyleRuler::EnableMaximumWallThickneesLabel(bool flag)
//{
//	m_MaximumWallThickneesLabelFlag = flag;
//	UpdateMaximumWallThicknessLabel();
//}

void InteractorStyleRuler::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	//SAFE_DOWN_CAST_IMAGE_CONSTITERATOR(InteractorStyleRuler, UpdateMaximumWallThicknessLabel());
}

InteractorStyleRuler::~InteractorStyleRuler()
{
	//if (this->m_pointHandleRepresentation2D != nullptr) {
	//	this->m_pointHandleRepresentation2D->Delete();
	//	this->m_pointHandleRepresentation2D = nullptr;
	//}
	//if (this->distanceRepresentation2D != nullptr) {
	//	this->distanceRepresentation2D->Delete();
	//	this->distanceRepresentation2D = nullptr;

	//}
	if (this->m_distanceWidget != nullptr) {
		this->m_distanceWidget->Delete();
		this->m_distanceWidget = nullptr;
	}
}

//void InteractorStyleRuler::UpdateMaximumWallThicknessLabel()
//{
	//if (GetSliceOrientation() != vtkImageViewer2::SLICE_ORIENTATION_XY ||
	//	!m_MaximumWallThickneesLabelFlag || m_imageViewer->GetAllBlack()) {
	//	if (m_imageViewer->GetAnnotationRenderer()->HasViewProp(this->m_lineActor)) {
	//		m_imageViewer->GetAnnotationRenderer()->RemoveActor(this->m_lineActor);

	//	}
	//	if (m_imageViewer->GetAnnotationRenderer()->HasViewProp(this->m_labelActor)) {
	//		m_imageViewer->GetAnnotationRenderer()->RemoveActor(this->m_labelActor);
	//	}
	//	m_imageViewer->Render();
	//	return;
	//}
	//int currentSlice = GetSlice();
	//m_imageViewer->GetOverlay()->Measure2D(currentSlice);

	//MaximumWallThickness* maximumWallThickness =
	//	m_imageViewer->GetOverlay()->GetMeasurementFor2D(currentSlice).mwt;
	//if ((m_imageViewer->GetOverlay()->Get2DMeasurementsStrings(currentSlice))[4] != "Normal") {
	//	if (m_imageViewer->GetAnnotationRenderer()->HasViewProp(this->m_lineActor)) {
	//		m_imageViewer->GetAnnotationRenderer()->RemoveActor(this->m_lineActor);

	//	}
	//	if (m_imageViewer->GetAnnotationRenderer()->HasViewProp(this->m_labelActor)) {
	//		m_imageViewer->GetAnnotationRenderer()->RemoveActor(this->m_labelActor);
	//	}
	//	m_imageViewer->Render();
	//	return;
	//}
	//// clean variables
	//this->m_labelArray->Reset();
	//this->m_sizeArray->Reset();

	//vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	//vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
	//std::vector<MaximumWallThickness::DistanceLoopPair> looppairs = maximumWallThickness->GetDistanceLoopPairVect();
	//for (int i = 0; i < looppairs.size(); i++)
	//{
	//	MaximumWallThickness::DistanceLoopPair l_lp = looppairs.at(i);
	//	double p1[3], p2[3];
	//	if (l_lp.LoopPair.first->GetNumberOfPoints() == 0) {
	//		//	// #DisplayErrorMsgHere
	//		//	this->ui->MWTTextBrowser->setText("Loop pair extraction incorrect!");
	//		//	for (int j = 0; j < 3; i++)
	//		//	{
	//		//		if (m_mainWnd->GetMyImageViewer(j)->GetSliceOrientation() == 2) {
	//		//			m_mainWnd->GetMyImageViewer(j)->GetAnnotationRenderer()->RemoveActor(this->m_lineActor);
	//		//			m_mainWnd->GetMyImageViewer(j)->GetAnnotationRenderer()->RemoveActor2D(this->m_labelActor);
	//		//		}
	//		//	}
	//		//	m_mainWnd->RenderAll2DViewers();
	//		return;
	//	}

	//	l_lp.LoopPair.first->GetPoint(l_lp.PIDs.first, p1);
	//	l_lp.LoopPair.second->GetPoint(l_lp.PIDs.second, p2);

	//	points->InsertNextPoint(p1);
	//	points->InsertNextPoint(p2);

	//	cells->InsertNextCell(2);
	//	cells->InsertCellPoint(points->GetNumberOfPoints() - 2);
	//	cells->InsertCellPoint(points->GetNumberOfPoints() - 1);

	//	// Update UI
	//	this->m_sizeArray->InsertNextValue(3);
	//	this->m_sizeArray->InsertNextValue(3);

	//	char distanceString[25];
	//	sprintf_s(distanceString, "%.3f mm", l_lp.Distance);
	//	this->m_labelArray->InsertNextValue(distanceString);
	//	this->m_labelArray->InsertNextValue(" ");

	//}

	//this->m_displayPD->SetPoints(points);
	//this->m_displayPD->SetLines(cells);
	//this->m_displayPD->GetPointData()->AddArray(this->m_labelArray);
	//this->m_displayPD->GetPointData()->AddArray(this->m_sizeArray);

	//this->m_p2labelfilter->SetInputData(this->m_displayPD);
	//this->m_p2labelfilter->Update();

	//m_labelMapper->SetInputConnection(this->m_p2labelfilter->GetOutputPort());
	//this->m_labelActor->SetMapper(m_labelMapper);

	//m_mapper->SetInputData(this->m_displayPD);
	//m_mapper->Update();

	//this->m_lineActor->SetMapper(m_mapper);

	//if (!m_imageViewer->GetAnnotationRenderer()->HasViewProp(this->m_lineActor)) {
	//	m_imageViewer->GetAnnotationRenderer()->AddActor(this->m_lineActor);

	//}
	//if (!m_imageViewer->GetAnnotationRenderer()->HasViewProp(this->m_labelActor)) {
	//	m_imageViewer->GetAnnotationRenderer()->AddActor2D(this->m_labelActor);
	//}
	//m_imageViewer->Render();
//}

InteractorStyleRuler::InteractorStyleRuler()
{
	//m_pointHandleRepresentation2D = vtkPointHandleRepresentation2D::New();
	//distanceRepresentation2D = vtkDistanceRepresentation2D::New();
	//distanceRepresentation2D = nullptr;
	m_distanceWidget = nullptr;

	// Maximum wall thickness stuff 
	//this->m_labelActor = vtkSmartPointer<vtkActor2D>::New();
	//this->m_lineActor = vtkSmartPointer<vtkActor>::New();
	//this->m_displayPD = vtkSmartPointer<vtkPolyData>::New();
	//this->m_labelArray = vtkSmartPointer<vtkStringArray>::New();
	//this->m_labelArray->SetName("Distance");
	//this->m_sizeArray = vtkSmartPointer<vtkIntArray>::New();
	//this->m_sizeArray->SetName("Sizes");
	//this->m_p2labelfilter = vtkSmartPointer<vtkPointSetToLabelHierarchy>::New();
	//this->m_p2labelfilter->SetLabelArrayName("Distance");
	//this->m_p2labelfilter->SetPriorityArrayName("Sizes");
	//this->m_labelMapper = vtkSmartPointer<vtkLabelPlacementMapper>::New();
	//this->m_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();


	//this->m_lineActor->GetProperty()->SetColor(0, 1, 0);
	//this->m_lineActor->GetProperty()->SetLineWidth(2);
}

