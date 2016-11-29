#ifndef MYVIDEOPANEL_H
#define MYVIDEOPANEL_H
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <opencv2/opencv.hpp>
class MyVideoPanel : public wxPanel
{
public:
	MyVideoPanel(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition,
		const wxSize &size = wxDefaultSize,	long style = wxTAB_TRAVERSAL);
	~MyVideoPanel();

	void setImage(cv::Mat& mat);
	
	
	wxBitmap* 	m_pBitmap;
	
	void OnPaint(wxPaintEvent& event);
};

#endif // MYVIDEOPANEL_H
