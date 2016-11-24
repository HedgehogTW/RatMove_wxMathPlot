#include "MyPlot.h"
#include "MainFrame.h"
#include <wx/log.h>
#include <wx/dcclient.h>

int lickRangeLow[GTNUM] = {6026, 15392, 16345, 26793, 28358, 29164, 33445, 34680, 35113, 44345, 46753};
int lickRangeUp[GTNUM]  = {6084, 15404, 16362, 27048, 28604, 29360, 34164, 35098, 35449, 44414, 46898};

MyPlot::MyPlot(wxWindow *parent, wxWindowID id,	const wxPoint &pos,	const wxSize &size,	long style )
		:wxPanel(parent, id)
{
	m_plot = NULL;
}

MyPlot::~MyPlot()
{
}

bool MyPlot::HasTransparentBackground()
{
}
bool MyPlot::IsCanvasWindow() const
{
}
void MyPlot::SegmentLabel(int start, int end, int label)
{
	std::vector<float> car_xs(2), car_ys(2);
	car_xs[0]=start;  car_ys[0]=1600;
    car_xs[1]=end;   car_ys[1]=1600;
	
	wxPen  pen;
	wxString str;
	pen.SetWidth(8);
	pen.SetCap( wxCAP_PROJECTING);	
	if(label ==1) {
		pen.SetColour(wxColour(0,200,0));
		str.Printf("%d", start);
	}else if(label ==0) {
		pen.SetColour(wxColour(200,200,0));
		str.Printf("%d", start);
	}else{
		pen.SetColour(wxColour(200,0,0));
		str.Printf("%d", start);
	}
	mpLayer* oldLayer =  m_plot->GetLayerByName(str);
	if(oldLayer)  
		m_plot->DelLayer(oldLayer, true);
	
    mpPolygon*	pLabel = new mpPolygon( str);	
    pLabel->SetPen( pen );
    pLabel->setPoints( car_xs,car_ys, true );
	pLabel->SetDrawOutsideMargins(false);   	
	
	m_plot->AddLayer(pLabel);
	m_plot->UpdateAll();
}
void MyPlot::plotSignal(vector<float> &vecFD, vector<float>& vecSmoothFD, 
		vector<float>& vecDesired, vector<float>& vecPredict)
{
	if(m_plot != NULL) {
		m_plot->DelAllLayers(true);
		delete m_plot;
		m_plot = NULL;
	}
	
	mpFXYVector* vectorLayerFD = new mpFXYVector(_(""));
	mpFXYVector* vectorLayersmoothFD = new mpFXYVector(_("smoothFD"));
	mpFXYVector* vectorLayerDesired = new mpFXYVector(_("Desired"));
	mpFXYVector* vectorLayerPredict = new mpFXYVector(_("Predict"));
	
	// Create two vectors for x,y and fill them with data

	int szSignal = vecFD.size();
	std::vector<float> vectorx(szSignal);
	for (unsigned int p = 0; p < szSignal; p++) {
		vectorx[p] = p;
	}	
	
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
    mpScaleX* xaxis = new mpScaleX(wxT(""), mpALIGN_BOTTOM, true, mpX_NORMAL);
    mpScaleY* yaxis = new mpScaleY(wxT(""), mpALIGN_LEFT, true);
	
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
	
	m_plot->SetMargins(10, 30, 30, 60);

    m_plot->AddLayer( xaxis );
    m_plot->AddLayer( yaxis );	
    m_plot->AddLayer( vectorLayerFD );
    m_plot->AddLayer( vectorLayersmoothFD );
	m_plot->AddLayer( vectorLayerPredict );
	m_plot->AddLayer( vectorLayerDesired );	
    m_plot->AddLayer( m_pLine );	
	
//	m_pLine->SetVisible(false);
	
	wxBrush hatch(wxColour(200,200,200), wxSOLID);
    m_plot->AddLayer( m_nfo = new mpInfoCoords(wxRect(700,20,10,40), wxTRANSPARENT_BRUSH)); //&hatch));
    m_nfo->SetVisible(false);
    wxBrush hatch2(wxColour(163,208,212), wxSOLID);
    mpInfoLegend* leg;
    m_plot->AddLayer( leg = new mpInfoLegend(wxRect(200,20,40,40), wxTRANSPARENT_BRUSH)); //&hatch2));
    leg->SetVisible(false);
    
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
 //   axesPos[0] = 0;
 //   axesPos[1] = 0;
 //   ticks = true;
    m_plot->EnablefloatBuffer(true);
    m_plot->SetMPScrollbars(true);
    m_plot->Fit(0, 3000, 0, 2100);	

}
