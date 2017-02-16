
// viewerView.cpp : implementation of the CviewerView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "viewer.h"
#endif

#include "viewerDoc.h"
#include "viewerView.h"

#include "..\tracelib\tracer\RayTracer.h"

#include "..\tracelib\tracer\imagemap.h"
#include "..\tracelib\linearmath\vector3d.h"
#include "..\tracelib\linearmath\mat3x3.h"

#include "ViewerOptions.h"

#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CviewerView

IMPLEMENT_DYNCREATE(CviewerView, CView)

BEGIN_MESSAGE_MAP(CviewerView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
  ON_COMMAND(ID_VIEW_OPTIONS, &CviewerView::OnOptions)
  ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
  ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CviewerView construction/destruction

CviewerView::CviewerView() :
  hBitmap_(NULL) {
  statistics_ = {0};
  statistics_.min_time = 9999999999;
  statistics_.max_time = 0.0;
}

CviewerView::~CviewerView(){
  int i = 0;
}

BOOL CviewerView::PreCreateWindow(CREATESTRUCT& cs) {
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CView::PreCreateWindow(cs);
}

void CviewerView::OnDraw(CDC* /*pDC*/)
{
  CviewerDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
  if (!pDoc)
    return;
  Imagemap& image = pDoc->tracer->imgBuffer_;

  //CDC dcSource;

  BITMAPINFO bmi = {0};
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = static_cast<LONG>(image.width());
  bmi.bmiHeader.biHeight = static_cast<LONG>(image.height());
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 24;

  CClientDC dc(this);
  SetDIBitsToDevice(dc, 0, 0, 
                    static_cast<DWORD>(image.width()), static_cast<LONG>(image.height()), 
                    0, 0, 0, 
                    static_cast<UINT>(image.height()), image.getData(), &bmi, 0);
  auto pDC = GetDC();
  
  double fps = pDoc->tracer->render_time_;
  if (fps > statistics_.max_time)
    statistics_.max_time = fps;
  else if (fps < statistics_.min_time)
    statistics_.min_time = fps;
  statistics_.rendered_frames_++;
  statistics_.time_sum+=fps;
  
  std::string frames = "Frames : " + std::to_string(statistics_.rendered_frames_);
  std::string mintime = "Highest FPS : " + std::to_string(statistics_.max_time);
  std::string maxtime = "Lowest  FPS : " + std::to_string(statistics_.min_time);
  std::string avgtime = "Average FPS : " + std::to_string(statistics_.time_sum/statistics_.rendered_frames_);

  pDC->SetBkMode(TRANSPARENT);
  pDC->TextOutW(10, 10, CString(pDoc->tracer->message.c_str()));

  pDC->TextOutW(510, 30, CString(frames.c_str()));
  pDC->TextOutW(510, 50, CString(mintime.c_str()));
  pDC->TextOutW(510, 70, CString(maxtime.c_str()));
  pDC->TextOutW(510, 100, CString(avgtime.c_str()));

}

// CviewerView printing

void CviewerView::OnOptions() {
  CViewerOptions dlg;
  dlg.DoModal();
}

BOOL CviewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CviewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CviewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CviewerView diagnostics

#ifdef _DEBUG
void CviewerView::AssertValid() const
{
	CView::AssertValid();
}

void CviewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CviewerDoc* CviewerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CviewerDoc)));
	return (CviewerDoc*)m_pDocument;
}
#endif //_DEBUG


// CviewerView message handlers


void CviewerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CviewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	linearmath::vec3<float> light = pDoc->tracer->getLight();
	switch ( nChar )
	{
	case 'W':
		light.z += 1; 
		pDoc->tracer->setLight(light);
		pDoc->tracer->refresh();
		pDoc->UpdateAllViews(nullptr);
		break;
	case 'S':
		light.z -= 1;
		pDoc->tracer->setLight(light);
		pDoc->tracer->refresh();
		pDoc->UpdateAllViews(nullptr);
		break;
	case 'D':
		light.x += 1; 
		pDoc->tracer->setLight(light);
		pDoc->tracer->refresh();
		pDoc->UpdateAllViews(nullptr);
		break;
	case 'A':
		light.x -= 1; 
		pDoc->tracer->setLight(light);
		pDoc->tracer->refresh();
		pDoc->UpdateAllViews(nullptr);
		break;
	case 'Z':
		light.y += 1;
		pDoc->tracer->setLight(light);
		pDoc->tracer->refresh();
		pDoc->UpdateAllViews(nullptr);
		break;
	case 'X':
		light.y -= 1;
		pDoc->tracer->setLight(light);
		pDoc->tracer->refresh();
		pDoc->UpdateAllViews(nullptr);
		break;
	default:
		break;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}




void CviewerView::OnMouseMove(UINT nFlags, CPoint point) {
  if (nFlags && MK_LBUTTON) {
    CPoint mouseDelta = point - prevMousePos;
    const float oneDegree = 3.14156f / 180.f;
    float rotateAxis = oneDegree * (mouseDelta.x / 5.f);
    float rotateAxis2 = oneDegree * (mouseDelta.y / 5.f);

    CviewerDoc* pDoc = GetDocument();

    auto rMat = linearmath::mat3x3<float>::rotation_around_y_axis(-rotateAxis);
    auto rMat1 = linearmath::mat3x3<float>::rotation_around_x_axis(rotateAxis2);

    auto pos1 = rMat*pDoc->tracer->cameraPos;
    auto pos2 = rMat1*pos1;


    CString msg;
    msg.Format(L"Camera pos: %.2f , %.2f , %.2f", pos2.x, pos2.y, pos2.z);
    //AfxMessageBox(msg);

    pDoc->tracer->cameraPos = pos2;

    this->prevMousePos = point;
    pDoc->tracer->refresh();
    pDoc->UpdateAllViews(nullptr);

  }
  CView::OnMouseMove(nFlags, point);
}

void CviewerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	this->prevMousePos = point;
	CView::OnLButtonDown(nFlags, point);
}

BOOL CviewerView::OnMouseWheel(UINT fFlags, short zDelta, CPoint point) {
  CviewerDoc* pDoc = GetDocument();
  pDoc->tracer->cameraPos .z += (zDelta > 0.0f)? 1.0f : -1.0f;
  pDoc->tracer->refresh();
  pDoc->UpdateAllViews(nullptr);
  CView::OnMouseWheel(fFlags,zDelta,point);
  return TRUE;
}
