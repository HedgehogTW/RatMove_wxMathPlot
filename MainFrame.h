#ifndef MAINFRAME_H
#define MAINFRAME_H
#include "wxcrafter.h"

#include <vector>
#include <deque>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

void ShowVideoClip(int start);

using namespace std;
class MainFrame : public MainFrameBaseClass
{
public:
    MainFrame(wxWindow* parent);
    virtual ~MainFrame();

	static void myMsgOutput(wxString szFormat,...) {
		wxString strMsg;
		va_list argList;
		va_start(argList, szFormat);
		strMsg.PrintfV(szFormat, argList);
		va_end(argList); // 以上這幾行是用來處理類似printf 的參數

		m_pThis->m_textCtrlMsg->AppendText(strMsg);
		m_pThis->m_textCtrlMsg->ShowPosition(m_pThis->m_textCtrlMsg->GetLastPosition());
	}

    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	
	void ShowSignal();
	bool LoadProfileData(std::string& filename);
	bool LoadPredictData(std::string& filename);
	void Merge_Prune(std::vector<float> & vLabel, int low, int high);
	int  checkLabel(int x);
	
	static MainFrame *	m_pThis;
	std::string 	m_DataPath;
	int				m_DataCount;
	int				m_LeftWidth;

//	std::vector<int>  		m_vFrameNo; 
	std::vector<float>  	m_vSignalFD;
	std::vector<float>  	m_vSmoothFD;	
	std::vector<float>  	m_vDesired;
	std::vector<float>  	m_vPredict;

protected:
    virtual void OnScrollNext(wxCommandEvent& event);
    virtual void OnScrollPrevious(wxCommandEvent& event);
    virtual void OnScrollPause(wxCommandEvent& event);
    virtual void OnDataAutoScrolling(wxCommandEvent& event);
    virtual void OnScrollbarTimer(wxTimerEvent& event);
    virtual void OnVideoPause(wxCommandEvent& event);
    virtual void OnVideoStop(wxCommandEvent& event);
    virtual void OnMouseLeftDown(wxMouseEvent& event);
    //virtual void OnDataShow(wxCommandEvent& event);
};
#endif // MAINFRAME_H
