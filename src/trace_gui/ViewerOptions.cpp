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
  //assert(pComboBox);

  RenderOptions options;

  std::vector<std::string> platforms;
  options.GetRenderPlatforms(platforms);
  
  size_t platform_num = 1;
  for (const auto &item : platforms) {
    std::wstring item_lbl = L"#" + std::to_wstring(platform_num++) + L" " + 
                            std::wstring(item.begin(), item.end());
    pComboBox->AddString(item_lbl.c_str());
  }
  pComboBox->SetCurSel(0);
  //_platform_info_list.ModifyStyle(0, LVS_REPORT, 0);
  InitListBox();
  return 0;
}

void CViewerOptions::DoDataExchange(CDataExchange * pDX) {
  DDX_Control(pDX, IDC_PLATFORMS_SEL, _platform_cbox);
  //  DDX_Control(pDX, IDC_PLATFORM_INFO_LIST, _platform_info_lst);
  DDX_Control(pDX, IDC_PLATFORM_INFO_LIST, _platform_info_list);
}

void CViewerOptions::OnPlatformSelect() {

  // Obtain data for the selected platform 
  //_platform_info_list.RemoveAllGroups();//ResetContent();
  _platform_info_list.DeleteAllItems();
  int platform_id =  _platform_cbox.GetCurSel();

   RenderOptions options;
  std::map<std::string,std::string> info;
  options.GetPlatfomInfo(platform_id,info);

  for (const auto &key : info) {
    std::wstring name(key.first.cbegin(), key.first.cend());
    std::wstring val(key.second.cbegin(), key.second.cend());
    //_platform_info_list.AddString(name.c_str());
    //_platform_info_list.AddString(val.c_str());

    LVITEM lvItem;
    //int nItem;

    lvItem.mask = LVIF_TEXT;
    lvItem.iItem = _platform_info_list.GetItemCount();;
    lvItem.iSubItem = 0;
    lvItem.pszText = (LPWSTR)name.c_str();
    auto cur_item = _platform_info_list.InsertItem(&lvItem);

    _platform_info_list.SetItemText(cur_item, 0, name.c_str());
    _platform_info_list.SetItemText(cur_item, 1, val.c_str());
  }

  // Obtain devices and show their core features
  PlatformSpec devices_info;
  options.GetDevices(platform_id, devices_info);
  size_t device_num = 1;
  for(const auto &device : devices_info) {
    // Print device label
    std::wstring label = L"DEVICE #" + std::to_wstring(device_num);
    LVITEM lvItem;

    lvItem.mask = LVIF_TEXT;
    lvItem.iItem = _platform_info_list.GetItemCount();;
    lvItem.iSubItem = 0;
    lvItem.pszText = (LPWSTR)label.c_str();
    auto cur_item = _platform_info_list.InsertItem(&lvItem);

    _platform_info_list.SetItemText(cur_item, 0, label.c_str());
    _platform_info_list.SetItemText(cur_item, 1, L"");

    // Print obtained features
    auto &features = devices_info[device_num-1];
    for (auto &feature : features) {
      LVITEM lvItem;

      lvItem.mask = LVIF_TEXT;
      lvItem.iItem = _platform_info_list.GetItemCount();;
      lvItem.iSubItem = 0;
      lvItem.pszText = (LPWSTR)feature.first.c_str();
      auto cur_item = _platform_info_list.InsertItem(&lvItem);


      _platform_info_list.SetItemText(cur_item, 0, feature.first.c_str());
      _platform_info_list.SetItemText(cur_item, 1, feature.second.c_str());
    }

    ++device_num;
  }
}

void CViewerOptions::InitListBox() {
  // TODO: Add extra initialization here
  LVCOLUMN lvColumn;
  int nCol;

  lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
  lvColumn.fmt = LVCFMT_LEFT;
  lvColumn.cx = 120;
  lvColumn.pszText = L"Property";
  nCol = _platform_info_list.InsertColumn(0, &lvColumn);

  lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
  lvColumn.fmt = LVCFMT_LEFT;
  lvColumn.cx = 1000;
  lvColumn.pszText = L"Value";
  _platform_info_list.InsertColumn(1, &lvColumn);
}
