#include "MyVideoPanel.h"
#include <wx/dcclient.h>
#include <wx/log.h> 

MyVideoPanel::MyVideoPanel(wxWindow *parent, wxWindowID id,	const wxPoint &pos,	const wxSize &size,	long style)
					:wxPanel(parent, id)
{
	m_pBitmap = NULL;
}

MyVideoPanel::~MyVideoPanel()
{
	if(m_pBitmap != NULL)	delete m_pBitmap;
}
void MyVideoPanel::OnPaint(wxPaintEvent& event)
{
	
	wxPaintDC dc(this);
    if(m_pBitmap)	dc.DrawBitmap(*m_pBitmap, 0, 0, false);

}
void MyVideoPanel::setImage(cv::Mat& mat)
{
    wxImage wxIm;
    cv::Mat  rgbOutput;
    int type = mat.type();
    int channel = mat.channels();
    bool ret = false;
	cv::rectangle(mat, cv::Point(170, 2), cv::Point(315, 214), cv::Scalar(0, 0, 255), 2);
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
		wxClientDC dc(this);
		if(m_pBitmap)	dc.DrawBitmap(*m_pBitmap, 0, 0, false);
		Refresh();
    }else wxLogMessage(wxT("wxIm.Create failed"));	

}