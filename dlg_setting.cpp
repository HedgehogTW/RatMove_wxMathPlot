//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: dlg_setting.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "dlg_setting.h"


// Declare the bitmap loading function
extern void wxC4499InitBitmapResources();

static bool bBitmapLoaded = false;


DlgSysSettingBase::DlgSysSettingBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxC4499InitBitmapResources();
        bBitmapLoaded = true;
    }
    
    wxBoxSizer* boxSizer4 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer4);
    
    wxBoxSizer* boxSizer22 = new wxBoxSizer(wxHORIZONTAL);
    
    boxSizer4->Add(boxSizer22, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText15 = new wxStaticText(this, wxID_ANY, _("Data path"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    boxSizer22->Add(m_staticText15, 0, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlDataPath = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlDataPath->SetHint(wxT(""));
    #endif
    
    boxSizer22->Add(m_textCtrlDataPath, 1, wxALL, WXC_FROM_DIP(5));
    
    m_buttonBrowse = new wxButton(this, wxID_ANY, _("Browse"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    boxSizer22->Add(m_buttonBrowse, 0, wxALL, WXC_FROM_DIP(5));
    
    m_stdBtnSizer6 = new wxStdDialogButtonSizer();
    
    boxSizer4->Add(m_stdBtnSizer6, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_buttonOK = new wxButton(this, wxID_OK, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer6->AddButton(m_buttonOK);
    
    m_buttonCancel = new wxButton(this, wxID_CANCEL, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer6->AddButton(m_buttonCancel);
    m_stdBtnSizer6->Realize();
    
    SetName(wxT("DlgSysSettingBase"));
    SetSize(600,400);
    if (GetSizer()) {
         GetSizer()->Fit(this);
    }
    if(GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
#if wxVERSION_NUMBER >= 2900
    if(!wxPersistenceManager::Get().Find(this)) {
        wxPersistenceManager::Get().RegisterAndRestore(this);
    } else {
        wxPersistenceManager::Get().Restore(this);
    }
#endif
    // Connect events
    m_buttonBrowse->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgSysSettingBase::OnButtonBrowse), NULL, this);
    
}

DlgSysSettingBase::~DlgSysSettingBase()
{
    m_buttonBrowse->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgSysSettingBase::OnButtonBrowse), NULL, this);
    
}
