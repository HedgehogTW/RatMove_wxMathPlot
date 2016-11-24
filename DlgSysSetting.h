#ifndef DLGSYSSETTING_H
#define DLGSYSSETTING_H
#include "dlg_setting.h"
#include <string>
class DlgSysSetting : public DlgSysSettingBase
{
public:
    DlgSysSetting(wxWindow* parent);
    virtual ~DlgSysSetting();
	
	
	void SetDataPath(std::string path);
	void GetDataPath(std::string& path);
	
	std::string  m_path;
protected:
    virtual void OnButtonBrowse(wxCommandEvent& event);
};
#endif // DLGSYSSETTING_H
