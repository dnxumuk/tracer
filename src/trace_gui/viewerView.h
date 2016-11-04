
// viewerView.h : interface of the CviewerView class
//

#pragma once
struct RenderStatistics {
  size_t rendered_frames_;
  double min_time;
  double max_time;
  double time_sum;
};

class CviewerView : public CView
{
protected: // create from serialization only
	CviewerView();
	DECLARE_DYNCREATE(CviewerView)

// Attributes
public:
	CviewerDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CviewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
private:
	CPoint prevMousePos;
  HBITMAP hBitmap_;
  RenderStatistics statistics_;
};

#ifndef _DEBUG  // debug version in viewerView.cpp
inline CviewerDoc* CviewerView::GetDocument() const
   { return reinterpret_cast<CviewerDoc*>(m_pDocument); }
#endif

