#include "MyPlotSegment.h"
#include "MainFrame.h"
#include <wx/log.h>
#include <wx/dcclient.h>


MyPlotSegment::MyPlotSegment(wxWindow *parent, wxWindowID id,	const wxPoint &pos,	const wxSize &size,	long style )
		:wxPanel(parent, id)
{
	m_plot = NULL;
	m_szSignal = 0;
}

MyPlotSegment::~MyPlotSegment()
{
}

bool MyPlotSegment::HasTransparentBackground()
{
}
bool MyPlotSegment::IsCanvasWindow() const
{
}

void MyPlotSegment::showProfileSegment(int start, int end)
{
	if(m_plot == NULL)  return;

	int from, to;
	if(start -500 > 0) 
		from = start -500;
	else
		from = 0;
		
	if(end+ 500 < m_szSignal) 
		to = end+ 500;
	else
		to = m_szSignal;
		
	m_plot->Fit(from, to, 0, 2000);		

}
void MyPlotSegment::setSignalSegment(vector<float> &vecFD, vector<float>& vecSmoothFD, 
		vector<float>& vecDesired, vector<float>& vecPredict)
{
	if(m_plot != NULL) {
		m_plot->DelAllLayers(true);
		delete m_plot;
		m_plot = NULL;
	}
	mpFXYVector* vectorLayerFD = new mpFXYVector(_("FD"));
	mpFXYVector* vectorLayersmoothFD = new mpFXYVector(_("smoothFD"));
	mpFXYVector* vectorLayerDesired = new mpFXYVector(_("Desired"));
	mpFXYVector* vectorLayerPredict = new mpFXYVector(_("Predict"));
	
	// Create two vectors for x,y and fill them with data
	
	int szSignal = vecFD.size();
	std::vector<float> vectorx(szSignal);
	for (unsigned int p = 0; p < szSignal; p++) {
		vectorx[p] = p;
	}	
	m_szSignal = szSignal;
	
	// FD
	vectorLayerFD->SetData(vectorx, vecFD);
	vectorLayerFD->SetContinuity(true);
	wxPen vectorpenFD(wxColour(0x00A5FF), 1, wxSOLID);
	vectorLayerFD->SetPen(vectorpenFD);
	vectorLayerFD->SetDrawOutsideMargins(false);
	
	// smoothFD
	vectorLayersmoothFD->SetData(vectorx, vecSmoothFD);
	vectorLayersmoothFD->SetContinuity(true);
	wxPen vectorpenSmooth(wxColour(0xff0000), 1, wxSOLID);
	vectorLayersmoothFD->SetPen(vectorpenSmooth);
	vectorLayersmoothFD->SetDrawOutsideMargins(false);	

	// Desired
	vectorLayerDesired->SetData(vectorx, vecDesired);
	vectorLayerDesired->SetContinuity(true);
	wxPen vectorpenDesired(wxColour(0x0000ff), 1, wxSOLID);
	vectorLayerDesired->SetPen(vectorpenDesired);
	vectorLayerDesired->SetDrawOutsideMargins(false);	
	
	// Predict
	vectorLayerPredict->SetData(vectorx, vecPredict);
	vectorLayerPredict->SetContinuity(true);
	wxPen vectorpenPredict(wxColour(0x006400), 1, wxSOLID);
	vectorLayerPredict->SetPen(vectorpenPredict);
	vectorLayerPredict->SetDrawOutsideMargins(false);
	
	wxClientDC dc(this);
	int neww, newh;
    dc.GetSize( &neww, &newh );

    m_plot = new mpWindow( this, -1, wxPoint(0,0), wxSize(neww, newh), wxSIMPLE_BORDER );
    mpScaleX* xaxis = new mpScaleX(wxT("X"), mpALIGN_BOTTOM, true, mpX_NORMAL);
    mpScaleY* yaxis = new mpScaleY(wxT("Y"), mpALIGN_LEFT, true);
	
	wxFont graphFont(wxFontInfo(9).FaceName("Helvetica"));
    xaxis->SetFont(graphFont);
    yaxis->SetFont(graphFont);
    xaxis->SetDrawOutsideMargins(false);
    yaxis->SetDrawOutsideMargins(false);
	// Fake axes formatting to test arbitrary format string
	// xaxis->SetLabelFormat(wxT("%.2f €"));
	yaxis->SetLabelFormat(wxT("%.1f"));
	
	
	std::vector<float> car_xs(2), car_ys(2);
	car_xs[0]=0;  car_ys[0]=-100;
    car_xs[1]=0;   car_ys[1]=2000;
    m_pLine = new mpPolygon( wxT(""));

    m_pLine->SetPen( wxPen(*wxGREEN, 2, wxDOT_DASH) );
    m_pLine->setPoints( car_xs,car_ys, true );
	m_pLine->SetDrawOutsideMargins(false);    
	
	m_plot->SetMargins(30, 30, 50, 60);

    m_plot->AddLayer( xaxis );
    m_plot->AddLayer( yaxis );	
    m_plot->AddLayer( vectorLayerFD );
    m_plot->AddLayer( vectorLayersmoothFD );
	m_plot->AddLayer( vectorLayerPredict );
	m_plot->AddLayer( vectorLayerDesired );	
    m_plot->AddLayer( m_pLine );	
//	m_pLine->SetVisible(false);
	
//	wxBrush hatch(wxColour(200,200,200), wxSOLID);
//    m_plot->AddLayer( m_nfo = new mpInfoCoords(wxRect(700,20,10,40), wxTRANSPARENT_BRUSH)); //&hatch));
//    m_nfo->SetVisible(false);
//    wxBrush hatch2(wxColour(163,208,212), wxSOLID);
//    mpInfoLegend* leg;
//    m_plot->AddLayer( leg = new mpInfoLegend(wxRect(200,20,40,40), wxTRANSPARENT_BRUSH)); //&hatch2));
//    leg->SetVisible(false);
    
    // m_plot->EnableCoordTooltip(true);
    // set a nice pen for the lissajoux

//    m_log = new wxTextCtrl( this, -1, wxT("This is the log window.\n"), wxPoint(0,0), wxSize(100,100), wxTE_MULTILINE );
	wxTextCtrl* logText = MainFrame::m_pThis->GetTextCtrlMsg();
    wxLog *old_log = wxLog::SetActiveTarget( new wxLogTextCtrl( logText));
    delete old_log;
    
 //   topsizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer* topsizer = (wxBoxSizer*)GetSizer();
    topsizer->Add( m_plot, 1, wxEXPAND );
//    topsizer->Add( m_log, 0, wxEXPAND );

    SetAutoLayout( TRUE );
    SetSizer( topsizer );
 
    m_plot->EnablefloatBuffer(true);
    m_plot->SetMPScrollbars(false);

    m_plot->Fit(500, 1500, 0, 2000);	

}
