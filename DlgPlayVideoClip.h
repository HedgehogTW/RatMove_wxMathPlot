#ifndef DLGPLAYVIDEOCLIP_H
#define DLGPLAYVIDEOCLIP_H
#include "play_video_clip.h"


#include <wx/gdicmn.h>
#include <opencv2/opencv.hpp>
class DlgPlayVideoClip : public DlgPlayVideoClipBase
{
public:
    DlgPlayVideoClip(wxWindow* parent);
    virtual ~DlgPlayVideoClip();
	
	MyPlotSegment* getPanelProfile() { return m_panelProfile; }
	
	void PlayVideoClip(int start, int end);
	void setImage(cv::Mat& mat);
	void showProfileSegment(int start, int end);
	void setSignalSegment(vector<float> &vecFD, vector<float>& vecSmoothFD, 
			vector<float>& vecDesired, vector<float>& vecPredict);
	
	int 		m_szSignal;
	bool 		m_bStop;
	bool 		m_bPause;
	wxBitmap* 	m_pBitmap;
protected:
    virtual void OnPause(wxCommandEvent& event);
    virtual void OnPlay(wxCommandEvent& event);
    virtual void OnPaint(wxPaintEvent& event);
};
#endif // DLGPLAYVIDEOCLIP_H
