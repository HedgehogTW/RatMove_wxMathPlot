#include "DlgPlayVideoClip.h"
#include <wx/dcclient.h>
#include <wx/log.h> 
#include <wx/utils.h> 
#include <opencv2/highgui/highgui.hpp>
#include "MainFrame.h"

#define WAIT_TIME  10
#define PAUSE_TIME 5000

DlgPlayVideoClip::DlgPlayVideoClip(wxWindow* parent)
    : DlgPlayVideoClipBase(parent)
{
	m_pBitmap = NULL;
	m_szSignal = 0;
}

DlgPlayVideoClip::~DlgPlayVideoClip()
{
	m_bStop = true;
	if(m_pBitmap != NULL)	delete m_pBitmap;

}

void DlgPlayVideoClip::setSignalSegment(vector<float> &vecFD, vector<float>& vecSmoothFD, 
		vector<float>& vecDesired, vector<float>& vecPredict)
{
	m_szSignal = vecFD.size();
	m_panelProfile->setSignalSegment(vecFD, vecSmoothFD, vecDesired, vecPredict);
}

void DlgPlayVideoClip::showProfileSegment(int start, int end)
{
	if(m_panelProfile->m_plot == NULL)  return;

	int from, to;
	if(start -500 > 0) 
		from = start -500;
	else
		from = 0;
		
	if(end+ 500 < m_szSignal) 
		to = end+ 500;
	else
		to = m_szSignal;
		
	m_panelProfile->m_plot->Fit(from, to, 0, 2000);		
	
}
void DlgPlayVideoClip::OnPaint(wxPaintEvent& event)
{
	
	wxPaintDC dc(m_panelVideo);
    if(m_pBitmap)	dc.DrawBitmap(*m_pBitmap, 0, 0, false);

}
void DlgPlayVideoClip::setImage(cv::Mat& mat)
{
    wxImage wxIm;
    cv::Mat  rgbOutput;
    int type = mat.type();
    int channel = mat.channels();
    bool ret = false;
    if(type ==CV_8UC1) {
        cvtColor(mat, rgbOutput, CV_GRAY2RGB);
        ret = wxIm.Create(mat.cols, mat.rows, rgbOutput.data, true);	
    }else if(type == CV_8UC3) {
        cvtColor(mat, rgbOutput, CV_BGR2RGB);
		ret = wxIm.Create(mat.cols, mat.rows, rgbOutput.data, true);
    }else if(channel ==1){
		cv::Mat  m8UC1;
		double min, max, a;
		cv::minMaxLoc(mat, &min, &max);
		a = 255./(max - min);
		mat.convertTo(m8UC1, CV_8UC1, a, -min*a );
		cv::cvtColor(m8UC1, rgbOutput, CV_GRAY2RGB);
		ret = wxIm.Create(mat.cols, mat.rows, rgbOutput.data, true);
    }
    if(ret) {
		if(m_pBitmap)  delete m_pBitmap;
		m_pBitmap = new wxBitmap( wxIm );	
//		int w = wxIm.GetWidth();
//		int h = wxIm.GetHeight();
//		SetVirtualSize( w, h );
		wxClientDC dc(m_panelVideo);
		if(m_pBitmap)	dc.DrawBitmap(*m_pBitmap, 0, 0, false);
		Refresh();
    }else wxLogMessage(wxT("wxIm.Create failed"));	

}


void DlgPlayVideoClip::PlayVideoClip(int start, int end)
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
	MainFrame::myMsgOutput("ShowVideoClip frame [%d..%d] \n", start, end);

//	MyPlotSegment* pPlotSegment = GetPanelProfile();
//	pPlotSegment->showProfileSegment(start, end);
//	Refresh();
//	cv::namedWindow( "Video", 0 );
//	cv::setMouseCallback( "Video", OnMouseVideo, 0 );
	
	if(start -30 > 0) start -= 30;
	if(end + 30 < m_szSignal)  end += 30;
	
	int frameNumber = 0;
	m_bStop = m_bPause = false;
	wxBeginBusyCursor();
	while(frameNumber < start){
		frameNumber++;	
		vidCap >> img_input;
		if (img_input.empty()) break;
	}
	wxEndBusyCursor();
	
//	mpWindow *pPlotWin = GetPanelPlot()->GetPlotWin();
//	mpMovableObject* pLine = GetPanelPlot()->GetLineObjPtr();
//	pLine->SetVisible(true);
//	pLine->SetCoordinateBase(frameNumber, 100);
//    pPlotWin->UpdateAll();
	

		
	
	do{
		if(m_bPause)  {
			m_bPause = false;
			cv::waitKey(PAUSE_TIME);
		}		
		if(m_bStop)  break;
		
	
		
		vidCap >> img_input;
		if (img_input.empty()) break;	
		setImage(img_input);
		//cv::imshow("Video", img_input);
		
		float sec = frameNumber /fps;
		int mm = sec / 60;
		int ss = sec - mm*60;
		wxString str;
		str.Printf("Frame No. %d, %02d:%02d", frameNumber, mm, ss);
		
		wxStatusBar* statusBar = MainFrame::m_pThis->GetStatusBar() ;
		statusBar->SetStatusText(str, 3);
		
//		pLine->SetCoordinateBase(frameNumber, -100);
//		pPlotWin->UpdateAll();
		frameNumber++;
		if(frameNumber > end)  break;
		wxMilliSleep(500) ;
		
	}while(1);		
	
}
void DlgPlayVideoClip::OnPause(wxCommandEvent& event)
{
	m_bPause = true;
	MainFrame::myMsgOutput( "OnVideoPause: wait %d milliseconds.\n", PAUSE_TIME);		
}
void DlgPlayVideoClip::OnPlay(wxCommandEvent& event)
{
//	m_bStop = true;
//	MainFrame::myMsgOutput( "OnVideoStop\n");	
}
