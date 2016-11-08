
// viewerDoc.cpp : implementation of the CviewerDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "viewer.h"
#endif

#include "viewerDoc.h"

#include <propkey.h>

#include "..\tracelib\tracer\scene.h"
#include "..\tracelib\tracer\RayTracer.h"

#include "..\tracelib\tracer\light.h"
#include "..\tracelib\shapes\plane.h"
#include "../tracelib/shapes/face3.h"

#include "..\tracelib\linearmath\vector4d.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CviewerDoc

IMPLEMENT_DYNCREATE(CviewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CviewerDoc, CDocument)
//	ON_COMMAND(ID_FILE_OPEN, &CviewerDoc::OnFileOpen() )
END_MESSAGE_MAP()


// CviewerDoc construction/destruction

CviewerDoc::CviewerDoc()
{
	// TODO: add one-time construction code here
	renderedImage = nullptr;
	message = L"";
	tracer = new RayTracer();
	LoadScene();
}

CviewerDoc::~CviewerDoc()
{
	;
}

BOOL CviewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

void CviewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

void AddPyramid(Scene *scene) {
const float ceil = 0;
const float hight = 10;
  linearmath::vec3<float> points1[][4] = {
     // Floor
    {
      {0.0f,  5.0f, ceil},
      {5.0f, -5.0f, ceil},
      {-5.0f,-5.0f, ceil}
    }, {
    // 1-2
      {0.0f,  5.0f, ceil},
      {5.0f, -5.0f, ceil},
      {2.5f,  0.0f, hight}
    },{
    // 2-3
      {2.5f,  0.0f, hight},
      {5.0f, -5.0f, ceil},
      {-5.0f,-5.0f, ceil}
    }, {
      {0.0f,  5.0f, ceil},
      {2.5f,  0.0f, hight},
      {-5.0f,-5.0f, ceil}
    }
  };
  for (int i=0; i<4; ++i)
  scene->addShape(*new Face3(points1[0]));

}
void CviewerDoc::OnFileOpen()
{
	CFileDialog dlg( TRUE, nullptr, nullptr,
					 OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					 L"|*.scn |*.json|",
					 nullptr, 0, true);
	//if (dlg.DoModal() == IDOK)
	{
		// _sceneFileName = dlg.GetFolderPath + L"\\" + dlg.GetFileName();
		LoadScene();
	}
	UpdateAllViews( nullptr );
}
void CviewerDoc::LoadScene() {
  Scene* scene = new Scene();

  scene->addShape(*new Sphere({0.f, 0.f, -10.f}, 2.f));

  linearmath::vec3<float> points[] = {
    { 10.0f, 10.0f, 0.0f},
    { 0.0f, -5.0f, 5.0f},
    {-10.0f, 10.0f, 5.0f}
  };
  //scene->addShape(*new
  //   Face3(points)
  //);
  linearmath::vec3<float> points1[] = {
    {70.0f, 10.0f, -60.0f},
    {30.0f,-10.0f, 30.0f},
    {-50.0f, 10.0f, -60.0f}
  };
  scene->addShape(*new
    Face3(points1)
  );

  //scene->addShape(*new Plane(linearmath::vec3<float>{ 0.f, 1.f, 0.0f}, -100.f));
  //scene->addShape(*new Sphere({  0.f, 5.f, 0.f}, 20.f));
  //scene->addShape(*new Sphere({ -4.f, 4.f , 2.f}, 2.f));
  //scene->addShape(*new Sphere({ 4.f, 4.f, 2.f }, 2.f));

  Light* l1 = new Light();
  l1->setPosition({0.f,5.f,50.f}, .1f);
  scene->addLight(*l1);

  AddPyramid(scene);
	// Tracing

	tracer->setScene(scene);
	this->message = tracer->Run().c_str();
}

// CviewerDoc commands
