#include "stdafx.h"
#include "ViewerOptions.h"
#include "resource.h"
#include <vector>
#include <string>
#include <map>

#include "../tracelib/common/RenderOptions.h"


CViewerOptions::CViewerOptions() : CDialog(CViewerOptions::IDD){
}

//IMPLEMENT_DYNCREATE(CViewerOptions, CView)

BEGIN_MESSAGE_MAP(CViewerOptions, CDialog)
  ON_CBN_SELENDOK(IDC_PLATFORMS_SEL, &CViewerOptions::OnPlatformSelect)
END_MESSAGE_MAP()

CViewerOptions::~CViewerOptions() {}

BOOL CViewerOptions::OnInitDialog() {
  CDialog::OnInitDialog();

  CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_PLATFORMS_SEL);
  assert(pComboBox);

  RenderOptions options;

  std::vector<std::string> platforms;
  options.GetRenderPlatforms(platforms);
  for (const auto &item : platforms) {

    std::wstring item_lbl(item.begin(),item.end());
    pComboBox->AddString(item_lbl.c_str());
  }

  pComboBox->SetCurSel(0);
  return 0;
}

void CViewerOptions::DoDataExchange(CDataExchange * pDX) {
  DDX_Control(pDX, IDC_PLATFORMS_SEL, _platform_cbox);
  DDX_Control(pDX, IDC_PLATFORM_INFO_LIST, _platform_info_lst);
}

void CViewerOptions::OnPlatformSelect() {
  // Obtain data for the selected platform 
  _platform_info_lst.ResetContent();
  int platform_id =  _platform_cbox.GetCurSel();

  RenderOptions options;
  std::map<std::string,std::string> info;
  options.GetPlatfomInfo(platform_id,info);

  for (const auto &key : info) {
    std::wstring name(key.first.cbegin(), key.first.cend());
    std::wstring val(key.second.cbegin(), key.second.cend());
    _platform_info_lst.AddString(name.c_str());
    _platform_info_lst.AddString(val.c_str());
  }
  options.GetDevices(platform_id);
}
