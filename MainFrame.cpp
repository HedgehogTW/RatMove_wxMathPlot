#include "MainFrame.h"
#include <wx/aboutdlg.h>
#include <wx/filedlg.h>
#include <wx/bitmap.h>
#include <wx/log.h>
#include <wx/config.h>

#include <wx/dcclient.h>
#include <wx/dirdlg.h> 
#include <wx/msgdlg.h> 
#include <wx/filename.h>
#include <wx/utils.h> 
#include <wx/dir.h>
#include <wx/msgdlg.h>
#include <wx/sound.h>
#include <wx/filefn.h> 
#include <wx/numdlg.h> 
#include <wx/textdlg.h> 

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

MainFrame *	MainFrame::m_pThis=NULL;

MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBaseClass(parent)
{
#if defined(__WXMAC__)
/*
	wxIcon  icon;
	wxBitmap bitmap(wxT("ratty32.png"), wxBITMAP_TYPE_PNG); 
	icon.CopyFromBitmap(bitmap); 
	SetIcon(icon);
*/
#else
	SetIcon(wxICON(frame_icon));
#endif

	m_pThis = this;
	int statusWidth[4] = {100, 150, 140, 150};
	m_statusBar->SetFieldsCount(4, statusWidth);
	
	wxFont font(wxFontInfo(9).FaceName("Helvetica").Italic());
	m_auimgr19->GetArtProvider()->SetFont(wxAUI_DOCKART_CAPTION_FONT, font);
	myMsgOutput("Hello.... Cute Rat ...\n");	

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__) 	
	m_DataPath = "~/tmp/";
#else
	m_DataPath = "D:/Dropbox/Rat_Lick/data/";
#endif	
	
	ShowSignal();
}

MainFrame::~MainFrame()
{
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    wxUnusedVar(event);
    Close();
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxUnusedVar(event);
    wxAboutDialogInfo info;
    info.SetCopyright(_("My MainFrame"));
    info.SetLicence(_("GPL v2 or later"));
    info.SetDescription(_("Short description goes here"));
    ::wxAboutBox(info);
}
void MainFrame::OnDataShow(wxCommandEvent& event)
{
	ShowSignal();

}
void MainFrame::ShowSignal()
{
	string 	strProfileName  = m_DataPath + "_labelData.csv";
	
	LoadProfileData(strProfileName);
	m_panelPlot->plotSignal(m_vSignalFD, m_vSmoothFD);	
	
}
bool MainFrame::LoadProfileData(std::string& filename)
{
	FILE *fp = fopen(filename.c_str(), "r");
	if(fp == NULL) {
		wxString msg = "cannot open "+filename + "\n";
		MainFrame::myMsgOutput( msg);		
		wxMessageBox( msg,"Error", wxICON_ERROR);
		return false;		
	}	
	
//	m_vFrameNo.clear(); 
	m_vSignalFD.clear();
	m_vSmoothFD.clear();
	m_vLabel.clear();
	
	char title [200];
	fgets(title, 200, fp );
	while(!feof(fp)) {
		int label;
		float fd, smoothFD;
		int n = fscanf(fp, "%*d,%f,%f,%*f,%*f,%d", &fd, &smoothFD, &label);
		if(n!=3)  break;
		m_vSignalFD.push_back(fd);		
		m_vSmoothFD.push_back(smoothFD);
		m_vLabel.push_back(label);
	}
	fclose(fp);	
	MainFrame::myMsgOutput( "LoadProfileData(): read _labelData.csv, size %d\n", m_vSignalFD.size() );	
	return true;
	
}
void MainFrame::OnMouseLeftDown(wxMouseEvent& event)
{
	myMsgOutput("OnMouseLeftDown\n");	
	event.Skip();
}
