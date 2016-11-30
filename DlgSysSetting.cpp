#include "DlgSysSetting.h"
#include <wx/dirdlg.h> 

DlgSysSetting::DlgSysSetting(wxWindow* parent)
    : DlgSysSettingBase(parent)
{
	SetSize(500, 350);
}

DlgSysSetting::~DlgSysSetting()
{
}
void DlgSysSetting::SetDataPath(std::string path)
{
	m_textCtrlDataPath->SetValue(path);
	m_path = path;
	

}
void DlgSysSetting::GetDataPath(std::string& path)
{
	m_path = m_textCtrlDataPath->GetValue();
	if(m_path.back() != '/' && m_path.back() != '\\')
		m_path += "/";
		
	path = m_path;
	
}
void DlgSysSetting::OnButtonBrowse(wxCommandEvent& event)
{
	m_path = wxDirSelector("Choose a folder", m_path);
	if ( !m_path.empty() ){
		if(m_path.back() != '/' && m_path.back() != '\\')
		m_path += "/";
		m_textCtrlDataPath->SetValue(m_path);
	}
}
