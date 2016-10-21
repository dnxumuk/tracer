
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
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CviewerView construction/destruction

CviewerView::CviewerView()
{
	// TODO: add construction code here

}

CviewerView::~CviewerView()
{
}

BOOL CviewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CviewerView drawing

void CviewerView::OnDraw(CDC* /*pDC*/)
{
	CviewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (pDoc->renderedImage == nullptr)
		return;

	auto canvas = GetDC();
	HBITMAP hBitmap = CreateBitmap( pDoc->renderedImage->width(), pDoc->renderedImage->height(), 1, 32, 
		                            pDoc->renderedImage->getData() );
	CClientDC dc(this);
	CBitmap bmp;
	bmp.Attach(hBitmap);
	CDC      dcSource;
	dcSource.CreateCompatibleDC(&dc);

	BITMAP bmpInfo;
	bmp.GetBitmap(&bmpInfo);
	dcSource.SelectObject(&bmp);

	CRect rectDest;
	GetClientRect(&rectDest);
	int st = dc.BitBlt(0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcSource, 0, 0, SRCCOPY ); //returned 1
	DeleteObject(hBitmap);
	auto pDC = GetDC();
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOutW( 10, 10, CString(pDoc->tracer->message.c_str()) );

}


// CviewerView printing

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




void CviewerView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (nFlags && MK_LBUTTON)
	{
		CPoint mouseDelta = point - prevMousePos;
		const float oneDegree = 3.14 / 180.;
		float rotateAxis  = oneDegree * (mouseDelta.x / 5.);
		float rotateAxis2 = oneDegree * (mouseDelta.y / 5.);

		CviewerDoc* pDoc = GetDocument();

		auto rMat  = linearmath::mat3x3<float>::rotation_around_y_axis( -rotateAxis );
		auto rMat1 = linearmath::mat3x3<float>::rotation_around_x_axis( rotateAxis2);

		auto pos1 = rMat*pDoc->tracer->cameraPos;
		auto pos2 = rMat1*pos1;

		CString msg;
		msg.Format( L"Camera pos: %.2f , %.2f , %.2f", pos2.x, pos2.y, pos2.z);
		//AfxMessageBox(msg);

		pDoc->tracer->cameraPos = pos2;
		this->prevMousePos = point;
		pDoc->tracer->refresh();
		pDoc->UpdateAllViews(nullptr);

		//AfxMessageBox( L"Camera rotating detected !");
	}
	CView::OnMouseMove(nFlags, point);
}


void CviewerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	this->prevMousePos = point;
	CView::OnLButtonDown(nFlags, point);
}
