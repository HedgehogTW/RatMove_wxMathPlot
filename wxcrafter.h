//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: wxcrafter.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef _RATMOVE_WXMATHPLOT_WXCRAFTER_BASE_CLASSES_H
#define _RATMOVE_WXMATHPLOT_WXCRAFTER_BASE_CLASSES_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/frame.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/aui/framemanager.h>
#include <wx/aui/dockart.h>
#include <wx/pen.h>
#include <wx/aui/auibar.h>
#include <map>
#include <wx/toolbar.h>
#include <wx/panel.h>
#include "MyPlot.h"
#include "MyVideoPanel.h"
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

#ifdef WXC_FROM_DIP
#undef WXC_FROM_DIP
#endif
#if wxVERSION_NUMBER >= 3100
#define WXC_FROM_DIP(x) wxWindow::FromDIP(x, NULL)
#else
#define WXC_FROM_DIP(x) x
#endif


class MainFrameBaseClass : public wxFrame
{
public:
    enum {
        wxID_DATA_AUTO_SCROLL = 10001,
        wxID_PAUSE = 10002,
    };
protected:
    wxMenuBar* m_menuBar;
    wxMenu* m_nameFile;
    wxMenuItem* m_menuItem7;
    wxMenu* m_menuData;
    wxMenuItem* m_menuItemDataAutoScroll;
    wxMenu* m_nameHelp;
    wxMenuItem* m_menuItem9;
    wxStatusBar* m_statusBar;
    wxAuiManager* m_auimgr19;
    wxAuiToolBar* m_auibar21;
    MyPlot* m_panelPlot;
    wxPanel* m_panelVideoView;
    wxPanel* m_panel69;
    MyVideoPanel* m_panelVideo;
    wxPanel* m_panel83;
    wxButton* m_buttonAccept;
    wxButton* m_buttonPartialAccept;
    wxButton* m_buttonReject;
    wxPanel* m_panel71;
    wxButton* m_buttonReplay;
    wxButton* m_buttonPlay;
    wxButton* m_buttonPause;
    wxButton* m_buttonStop;
    wxPanel* m_panelMsg;
    wxTextCtrl* m_textCtrlMsg;
    wxTimer* m_timerScroll;

protected:
    virtual void OnClose(wxCloseEvent& event) { event.Skip(); }
    virtual void OnExit(wxCommandEvent& event) { event.Skip(); }
    virtual void OnDataAutoScrolling(wxCommandEvent& event) { event.Skip(); }
    virtual void OnAbout(wxCommandEvent& event) { event.Skip(); }
    virtual void OnUpdateUIAutoScroll(wxUpdateUIEvent& event) { event.Skip(); }
    virtual void OnScrollPause(wxCommandEvent& event) { event.Skip(); }
    virtual void OnPaint(wxPaintEvent& event) { event.Skip(); }
    virtual void OnAccept(wxCommandEvent& event) { event.Skip(); }
    virtual void OnPartialAccept(wxCommandEvent& event) { event.Skip(); }
    virtual void OnReject(wxCommandEvent& event) { event.Skip(); }
    virtual void OnVideoReplay(wxCommandEvent& event) { event.Skip(); }
    virtual void OnVideoPlay(wxCommandEvent& event) { event.Skip(); }
    virtual void OnVideoPause(wxCommandEvent& event) { event.Skip(); }
    virtual void OnVideoStop(wxCommandEvent& event) { event.Skip(); }
    virtual void OnScrollbarTimer(wxTimerEvent& event) { event.Skip(); }

public:
    wxMenuBar* GetMenuBar() { return m_menuBar; }
    wxStatusBar* GetStatusBar() { return m_statusBar; }
    wxAuiToolBar* GetAuibar21() { return m_auibar21; }
    MyPlot* GetPanelPlot() { return m_panelPlot; }
    MyVideoPanel* GetPanelVideo() { return m_panelVideo; }
    wxButton* GetButtonAccept() { return m_buttonAccept; }
    wxButton* GetButtonPartialAccept() { return m_buttonPartialAccept; }
    wxButton* GetButtonReject() { return m_buttonReject; }
    wxPanel* GetPanel83() { return m_panel83; }
    wxPanel* GetPanel69() { return m_panel69; }
    wxButton* GetButtonReplay() { return m_buttonReplay; }
    wxButton* GetButtonPlay() { return m_buttonPlay; }
    wxButton* GetButtonPause() { return m_buttonPause; }
    wxButton* GetButtonStop() { return m_buttonStop; }
    wxPanel* GetPanel71() { return m_panel71; }
    wxPanel* GetPanelVideoView() { return m_panelVideoView; }
    wxTextCtrl* GetTextCtrlMsg() { return m_textCtrlMsg; }
    wxPanel* GetPanelMsg() { return m_panelMsg; }
    wxAuiManager* GetAuimgr19() { return m_auimgr19; }
    wxTimer* GetTimerScroll() { return m_timerScroll; }
    MainFrameBaseClass(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("My Frame"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(950,650), long style = wxCAPTION|wxRESIZE_BORDER|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxSYSTEM_MENU|wxCLOSE_BOX);
    virtual ~MainFrameBaseClass();
};

#endif
