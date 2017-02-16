#pragma once
#include "afxwin.h"
#include "resource.h"5


class CViewerOptions :
  public CDialog {
public:
  CViewerOptions();
  virtual ~CViewerOptions();
  BOOL OnInitDialog() override;

protected:
  DECLARE_MESSAGE_MAP()
  void DoDataExchange(CDataExchange *pDx) override;
 
private:
  enum { IDD = IDD_DLG_VIEWER_OPTIONS };
  void OnPlatformSelect();
public:
  CComboBox _platform_cbox;
  CListBox _platform_info_lst;
};
