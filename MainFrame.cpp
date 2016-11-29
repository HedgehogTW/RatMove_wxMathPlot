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

#include "DlgSysSetting.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#define DESIRED_LOW 800
#define DESIRED_HIGH 1000

#define PREDICT_LOW 800
#define PREDICT_HIGH 1200
#define LABEL_HIGH 1200

#define MIN_SEGMENT 20
#define MIN_GAP  20

#define PAUSE_TIME 5000
#define WAIT_TIME  10

wxDEFINE_EVENT(wxEVT_MYTHREAD_FINISHED, wxThreadEvent);

bool 		g_bStop;
bool 		g_bPause;
	
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
	
	SetSize(1000, 650);
	Center();	
	
	wxFont font(wxFontInfo(9).FaceName("Helvetica").Italic());
	m_auimgr19->GetArtProvider()->SetFont(wxAUI_DOCKART_CAPTION_FONT, font);
	myMsgOutput("Hello.... Cute Rat ...\n");	

	wxConfigBase *pConfig = wxConfigBase::Get();
	m_DataPath = pConfig->Read("/set/dataPath", "");
	
	if(m_DataPath.empty()) {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__) 	
		m_DataPath = "~/tmp/";
#else
		m_DataPath = "../data/";
#endif	
	}
	
	m_SignalSize = -1;
	m_LeftWidth = 0;
	ShowSignal();
	
	Bind(wxEVT_MYTHREAD_FINISHED, &MainFrame::OnThreadFinished, this);
	
	myMsgOutput("Data path: " + m_DataPath + "\n");
}

MainFrame::~MainFrame()
{
	g_bStop = true;
	wxConfigBase *pConfig = wxConfigBase::Get();
	pConfig->Write("/set/dataPath", wxString(m_DataPath));
	
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
void MainFrame::OnSetting(wxCommandEvent& event)
{
	DlgSysSetting  dlg(this);
	dlg.SetDataPath(m_DataPath);
	if(dlg.ShowModal() == wxID_CANCEL)  return;
		
	dlg.GetDataPath(m_DataPath);	
	myMsgOutput("Data path: " + m_DataPath + "\n");
	
	wxConfigBase *pConfig = wxConfigBase::Get();
	pConfig->Write("/set/dataPath", wxString(m_DataPath));	
	
	ShowSignal();	
}
void MainFrame::OnFileLoadData(wxCommandEvent& event)
{
	wxString strPath = wxDirSelector("Choose a folder", m_DataPath);
	if ( strPath.empty() )  return;
	
	if(strPath.Last() != '/' || strPath.Last() != '\\')
		strPath += "/";
		
	m_SignalSize = -1;
	m_LeftWidth = 0;	
	m_DataPath = strPath;
	
	myMsgOutput("Data path: " + m_DataPath + "\n");
	
	wxConfigBase *pConfig = wxConfigBase::Get();
	pConfig->Write("/set/dataPath", wxString(m_DataPath));	
	
	ShowSignal();	
}

void MainFrame::ShowSignal()
{
	string 	strProfileName  = m_DataPath + "_labelData.csv";
	string 	strPredictName  = m_DataPath + "pred_w10o60.svm.csv";
	LoadProfileData(strProfileName);
	LoadPredictData(strPredictName);
	
	m_panelPlot->plotSignal(m_vSignalFD, m_vSmoothFD, m_vDesired, m_vPredict);	
	
	mpWindow *pPlotWin = GetPanelPlot()->GetPlotWin();
	int leftMar = pPlotWin->GetMarginLeft();
	float xscale = pPlotWin->GetXscl();
	m_LeftWidth = leftMar / xscale ; 
	int XScreen = pPlotWin->GetXScreen();
	m_CenterX = (XScreen-leftMar )/ (2*xscale);
//	myMsgOutput("XScreen %d, Xscale %f, %f\n", XScreen, xscale, XScreen/ xscale);
//	myMsgOutput("leftMar %d, Xscale %f, %f, centerX %d\n", leftMar, xscale, leftMar/ xscale, m_CenterX);
	
	m_start = m_end = -1;
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
	m_vDesired.clear();
	
	char title [200];
	fgets(title, 200, fp );
	while(!feof(fp)) {
		int label;
		float fd, smoothFD;
		int n = fscanf(fp, "%*d,%f,%f,%*f,%*f,%d", &fd, &smoothFD, &label);
		if(n!=3)  break;
		m_vSignalFD.push_back(fd);		
		m_vSmoothFD.push_back(smoothFD);
		if(label ==0)
			m_vDesired.push_back(DESIRED_LOW);
		else
			m_vDesired.push_back(DESIRED_HIGH);
	}
	fclose(fp);	
	
	m_SignalSize =  m_vSignalFD.size();
	
	Merge_Prune(m_vDesired, DESIRED_LOW, DESIRED_HIGH);
	
	MainFrame::myMsgOutput( "LoadProfileData(): read _labelData.csv, size %d\n",m_SignalSize);	
	return true;
	
}
bool MainFrame::LoadPredictData(std::string& filename)
{
	FILE *fp = fopen(filename.c_str(), "r");
	if(fp == NULL) {
		wxString msg = "cannot open "+filename + "\n";
		MainFrame::myMsgOutput( msg);		
		wxMessageBox( msg,"Error", wxICON_ERROR);
		return false;		
	}	
	
	if(m_SignalSize > 0) {
		m_vPredict.resize(m_SignalSize, PREDICT_LOW);
	}

	int count = 0;
	int old_start = -1;
	char title [200];
	fgets(title, 200, fp );
	fgets(title, 200, fp );
	while(!feof(fp)) {
		int label, predict, start, end;

		int n = fscanf(fp, "%*d,%d,%d,%d,%d", &label, &predict, &start, &end);
		if(n!=4)  break;
		if(predict ==1) {
			for(int i=start; i<end; i++)
				m_vPredict[i] = PREDICT_HIGH;			
		}

		if(start < old_start) break;
		old_start = start;
		count ++;
	}
	fclose(fp);	
	
	Merge_Prune(m_vPredict, PREDICT_LOW, PREDICT_HIGH);
	
	MainFrame::myMsgOutput( "LoadPredictData(): count %d\n", count );	
	return true;	
}
void MainFrame::Merge_Prune(std::vector<float> & vLabel, int low, int high)
{
	int size = vLabel.size();
	int len = 0;
	bool bYes = false;
	int start = 0;
	
	// merge
	for(int i=0; i<size; i++) {		
		if(vLabel[i] ==low ) {
			if(bYes==false) start = i;
			bYes = true;
			len++;
		}else {
			if(bYes) {
//				myMsgOutput( "Predict Len %d\n", len);
				if(len < MIN_GAP) {
					for(int k=start; k<=i; k++)
						vLabel[k] = high;
				}
			}
			bYes = false;
			len = 0;
		}		
	}
	
	// prune
	len = 0;
	bYes = false;
	start = 0;
	
	for(int i=0; i<size; i++) {
		
		if(vLabel[i] ==high ) {
			if(bYes==false) start = i;
			bYes = true;
			len++;
		}else {
			if(bYes) {
//				myMsgOutput( "Predict Len %d\n", len);
				if(len < MIN_SEGMENT) {
					for(int k=start; k<=i; k++)
						vLabel[k] = low;
				}
			}
			bYes = false;
			len = 0;
		}		
	}
}

wxThread::ExitCode MainFrame::Entry()
{
	g_bStop = g_bPause = false;

	if(m_start > 0 && m_end > 0) {
		PlayVideoClip(m_start, m_end);
		wxBell();
		if(! g_bStop)
			PlayVideoClip(m_start, m_end);
		wxBell();
	}
    wxQueueEvent(this, new wxThreadEvent(wxEVT_MYTHREAD_FINISHED));
    return (wxThread::ExitCode)0;
}
void MainFrame::PlayVideoClip(int start, int end)
{
	cv::VideoCapture vidCap;
	int waitTime = WAIT_TIME;
	
	cv::Mat img_input;
	string 	strVideoName  = MainFrame::m_pThis->m_DataPath + "1218(4).AVI";
	vidCap.open(strVideoName);
	if(vidCap.isOpened()==false) {
		MainFrame::myMsgOutput( "Load ... " + strVideoName + " ERROR\n");
		return;
	}

	double fps = vidCap.get(CV_CAP_PROP_FPS);
	MainFrame::myMsgOutput("ShowVideoClip from frame %d \n", m_start);	

	if(start -15 > 0) start -= 15;
	if(end + 15 < m_SignalSize)  end += 15;
	
	int frameNumber = 0;

	wxBeginBusyCursor();
	while(frameNumber < m_start){
		frameNumber++;	
		vidCap >> img_input;
		if (img_input.empty()) break;
	}
	wxEndBusyCursor();
	
	mpWindow *pPlotWin = GetPanelPlot()->GetPlotWin();
	mpMovableObject* pLine = GetPanelPlot()->GetLineObjPtr();

	pLine->SetVisible(true);
	pLine->SetCoordinateBase(frameNumber, 100);
    pPlotWin->UpdateAll();
	
	do{
		if(g_bPause)  {
			g_bPause = false;
			cv::waitKey(PAUSE_TIME);
		}		
		if(g_bStop)  break;
		
		vidCap >> img_input;
		if (img_input.empty()) break;	
		m_panelVideo->setImage(img_input);
		//cv::imshow("Video", img_input);
		
		float sec = frameNumber /fps;
		int mm = sec / 60;
		int ss = sec - mm*60;
		wxString str;
		str.Printf("Frame No. %d, %02d:%02d", frameNumber, mm, ss);
		
		wxStatusBar* statusBar = MainFrame::m_pThis->GetStatusBar() ;
		statusBar->SetStatusText(str, 3);
		
		pLine->SetCoordinateBase(frameNumber, -100);
		pPlotWin->UpdateAll();
		frameNumber++;
		if(frameNumber > end)  break;
		//if(cv::waitKey(1) >= 0) break;
		wxMilliSleep(30);
	}while(1);		
}

void MainFrame::OnScrollbarTimer(wxTimerEvent& event)
{
	mpWindow *pPlotWin = GetPanelPlot()->GetPlotWin();
	
	static int lick_start = -1;
	static int lick_end = -1;
	float x = pPlotWin->GetXpos();
	int start = x + m_LeftWidth + m_CenterX;
	int lickFrame = -1;
	if(lick_start ==-1 && lick_end ==-1)
		lickFrame = checkLabel(start, lick_start, lick_end);
	else if(start <lick_start || start> lick_end)
			lickFrame = checkLabel(start, lick_start, lick_end);
		
	if(lickFrame > 0) {
		myMsgOutput("x %.1f, start %d, lickFrame %d, [%d, %d]\n", x, start, lickFrame, lick_start, lick_end);
		m_timerScroll->Stop();
		m_start = lick_start;
		m_end = lick_end;
		if (CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR) {
			wxLogError("Could not create the worker thread!");
			return;
		}
		if(GetThread()->Run() != wxTHREAD_NO_ERROR) {
			wxLogError("Could not run the worker thread!");
			return;
		}
		myMsgOutput( "CreateThread...\n");
		//PlayVideoClip(lick_start, lick_end);
//		wxBell();
//		PlayVideoClip(lick_start, lick_end);
//		wxBell();
		x = lick_end - m_LeftWidth - m_CenterX +2;
	}else
		x += 200;
	if(x < m_SignalSize-2000 )
		pPlotWin->SetPosX(x);
	else
		m_timerScroll->Stop();
}

int MainFrame::checkLabel(int start, int& lick_start, int& lick_end )
{
	bool bFound = false;
	int i, end;
	lick_start = lick_end = -1;

	end = start + 200;
	for(i=start; i<end; i++)
		if(m_vPredict[i] == PREDICT_HIGH) {
			lick_start = i;
			bFound = true;
			break;
		}
	
	if(bFound) {
		for(i=lick_start; i<m_SignalSize; i++)
			if(m_vPredict[i] == PREDICT_LOW) {
				lick_end = i-1;
				break;
			}
		if(i==m_SignalSize)
			lick_end = m_SignalSize -1;
			
	}
	
	int ret = -1;
	if(bFound) ret = lick_start;
	
	return ret;
}
void MainFrame::OnDataAutoScrolling(wxCommandEvent& event)
{
	
	m_timerScroll->Start(100);
}

void MainFrame::OnScrollPause(wxCommandEvent& event)
{
	myMsgOutput( "OnScrollPause: \n");
	m_timerScroll->Stop();
}

void MainFrame::OnToggleShowCoord(wxCommandEvent& event)
{
//	bool b = m_toggleButtonCoord->GetValue();
//	GetPanelPlot()->SetInfoCoordsVisible(b);
	
}
void MainFrame::OnPaint(wxPaintEvent& event)
{
	m_panelVideo->OnPaint(event);
//	wxPaintDC dc(this);
//    if(m_pBitmap)	dc.DrawBitmap(*m_pBitmap, 0, 0, false);

}
void MainFrame::OnClose(wxCloseEvent& event)
{
	if (GetThread() && // DoStartALongTask() may have not been called
		GetThread()->IsRunning()) {
			g_bStop = true;
			GetThread()->Wait();
		}
	Destroy();	
}
void MainFrame::OnThreadFinished(wxThreadEvent& evt)
{
	myMsgOutput( "OnThreadFinished.\n");
	
}
void MainFrame::OnVideoReplay(wxCommandEvent& event)
{
	if(GetThread()==NULL)  return;
	
	if (GetThread()->IsAlive())  {
		GetThread()->Resume();
		return;
	}
	if (CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR) {
		wxLogError("Could not create the worker thread!");
		return;
	}
	if(GetThread()->Run() != wxTHREAD_NO_ERROR) {
		wxLogError("Could not run the worker thread!");
		return;
	}
	myMsgOutput( "OnVideoReplay::CreateThread...\n");	
}
void MainFrame::OnVideoPause(wxCommandEvent& event)
{
	if(GetThread()==NULL)  return;
	if(GetThread()->IsAlive()) {
		GetThread()->Pause();
	}
}
void MainFrame::OnVideoStop(wxCommandEvent& event)
{
	if(GetThread()==NULL)  return;
	GetThread()->Resume();
	g_bStop = true;
	
//	if (GetThread()->IsAlive()){		
//		GetThread()->Wait();
//	}	
}
void MainFrame::OnVideoPlay(wxCommandEvent& event)
{
	if(GetThread()==NULL)  return;
	if (GetThread()->IsAlive()){
		GetThread()->Resume();
	}	
}


void MainFrame::OnUpdateUIAutoScroll(wxUpdateUIEvent& event)
{
	if(GetThread()) {
		bool bAlive = GetThread()->IsAlive();
		event.Enable(!bAlive);
	}else
		event.Enable(true);
}
void MainFrame::OnAccept(wxCommandEvent& event)
{
	if(m_start < 0 || m_end < 0)  return;
	m_panelPlot->SegmentLabel(m_start, m_end, 1);
	myMsgOutput( "Accept frame %d\n", m_start);	
}
void MainFrame::OnPartialAccept(wxCommandEvent& event)
{
	if(m_start < 0 || m_end < 0)  return;
	m_panelPlot->SegmentLabel(m_start, m_end, 0);
	myMsgOutput( "Partial Accept frame %d\n", m_start);	
}
void MainFrame::OnReject(wxCommandEvent& event)
{
	if(m_start < 0 || m_end < 0)  return;
	
	m_panelPlot->SegmentLabel(m_start, m_end, -1);
	myMsgOutput( "Reject frame %d\n", m_start);	
}
