
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
void CviewerDoc::LoadScene()
{
	Scene* scene = new Scene();
	//Sphere* sp = new Sphere( { 0.f, 0.f, -10.f }, 2.f );
	//scene->addShape( *sp );

  Plane* sp = new Plane(linearmath::vec4<float>{ 0.f, 1.f, 0.0f, -100.f });
  scene->addShape(*sp);

  Sphere* sp1 = new Sphere({0.f, 15.f, 0.f}, 10.f);
  scene->addShape(*sp1);

 Sphere* sp2 = new Sphere({ -4.f, 4.f, 2.f }, 2.f);
 scene->addShape(*sp2);

  //Sphere* sp3 = new Sphere({ 4.f, 4.f, 2.f }, 2.f);
  //scene->addShape(*sp3);

  Light* l1 = new Light();
  l1->setPosition({0.f,5.f,50.f}, .1f);
  scene->addLight(*l1);


	// Tracing

	tracer->setScene(scene);
	this->message = tracer->Run().c_str();
}

// CviewerDoc commands
