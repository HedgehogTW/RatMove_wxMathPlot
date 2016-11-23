#include "MyVideoPanel.h"
#include <wx/dcclient.h>
#include <wx/log.h> 

MyVideoPanel::MyVideoPanel(wxWindow *parent, wxWindowID id,	const wxPoint &pos,	const wxSize &size,	long style)
					:wxPanel(parent, id)
{
}

MyVideoPanel::~MyVideoPanel()
{
}

void MyVideoPanel::setImage(cv::Mat& mat)
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
		Refresh();
    }else wxLogMessage(wxT("wxIm.Create failed"));	

}