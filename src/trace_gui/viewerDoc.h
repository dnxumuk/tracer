
// viewerDoc.h : interface of the CviewerDoc class
//


#pragma once

#include "..\tracelib\tracer\imagemap.h"
#include "..\tracelib\tracer\RayTracer.h"


class CviewerDoc : public CDocument
{
protected: // create from serialization only
	CviewerDoc();
	DECLARE_DYNCREATE(CviewerDoc)
// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	Imagemap* renderedImage;
	CString message;
// Implementation
public:
	virtual ~CviewerDoc();

//	virtual void AssertValid() const;
//	virtual void Dump(CDumpContext& dc) const;
	void OnFileOpen();
	void LoadScene();
	RayTracer *tracer;
protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
