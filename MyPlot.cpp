#include "MyPlot.h"
#include "MainFrame.h"
#include <wx/log.h>
#include <wx/dcclient.h>
MyPlot::MyPlot(wxWindow *parent, wxWindowID id,	const wxPoint &pos,	const wxSize &size,	long style )
		:wxPanel(parent, id)
{
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

void MyPlot::plotSignal(vector<double> &vecFD, vector<double>& vecSmoothFD)
{
	mpFXYVector* vectorLayerFD = new mpFXYVector(_("FD"));
	mpFXYVector* vectorLayersmoothFD = new mpFXYVector(_("smoothFD"));
	// Create two vectors for x,y and fill them with data

	int szSignal = vecFD.size();
	std::vector<double> vectorx(szSignal);
	for (unsigned int p = 0; p < szSignal; p++) {
		vectorx[p] = p;
	}	
	
	// FD
	vectorLayerFD->SetData(vectorx, vecFD);
	vectorLayerFD->SetContinuity(true);
	wxPen vectorpenFD(wxColour(0x1E69D2), 1, wxSOLID);
	vectorLayerFD->SetPen(vectorpenFD);
	vectorLayerFD->SetDrawOutsideMargins(false);
	
	// smoothFD
	vectorLayersmoothFD->SetData(vectorx, vecSmoothFD);
	vectorLayersmoothFD->SetContinuity(true);
	wxPen vectorpenSmooth(wxColour(0xff0000), 1, wxSOLID);
	vectorLayersmoothFD->SetPen(vectorpenSmooth);
	vectorLayersmoothFD->SetDrawOutsideMargins(false);	

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
    m_plot->SetMargins(30, 30, 50, 80);

    m_plot->AddLayer( xaxis );
    m_plot->AddLayer( yaxis );	
    m_plot->AddLayer( vectorLayerFD );
    m_plot->AddLayer( vectorLayersmoothFD );
	

    wxBrush hatch(wxColour(200,200,200), wxSOLID);
    m_plot->AddLayer( nfo = new mpInfoCoords(wxRect(400,20,10,40), wxTRANSPARENT_BRUSH)); //&hatch));
    nfo->SetVisible(false);
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
    axesPos[0] = 0;
    axesPos[1] = 0;
    ticks = true;
    m_plot->EnableDoubleBuffer(true);
    m_plot->SetMPScrollbars(true);
    m_plot->Fit(0, 3000, 0, 2500);	

}
void MyPlot::Init()
{
	mpLayer* l;

	// Create a mpFXYVector layer
	mpFXYVector* vectorLayer = new mpFXYVector(_("Vector"));
	// Create two vectors for x,y and fill them with data
	std::vector<double> vectorx, vectory;
	double xcoord;
	for (unsigned int p = 0; p < 100; p++) {
		xcoord = ((double)p-50.0)*5.0;
		vectorx.push_back(xcoord);
		vectory.push_back(0.0001*pow(xcoord, 3));
	}

	vectorLayer->SetData(vectorx, vectory);
	vectorLayer->SetContinuity(true);
	wxPen vectorpen(*wxBLUE, 2, wxSOLID);
	vectorLayer->SetPen(vectorpen);
	vectorLayer->SetDrawOutsideMargins(false);

	wxClientDC dc(this);
	int neww, newh;
    dc.GetSize( &neww, &newh );


	wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    m_plot = new mpWindow( this, -1, wxPoint(0,0), wxSize(neww, newh), wxSUNKEN_BORDER );
    mpScaleX* xaxis = new mpScaleX(wxT("X"), mpALIGN_BOTTOM, true, mpX_NORMAL);
    mpScaleY* yaxis = new mpScaleY(wxT("Y"), mpALIGN_LEFT, true);
    xaxis->SetFont(graphFont);
    yaxis->SetFont(graphFont);
    xaxis->SetDrawOutsideMargins(false);
    yaxis->SetDrawOutsideMargins(false);
	// Fake axes formatting to test arbitrary format string
	// xaxis->SetLabelFormat(wxT("%.2f €"));
	// yaxis->SetLabelFormat(wxT("%p"));
    m_plot->SetMargins(30, 30, 50, 100);
//     m_plot->SetMargins(50, 50, 200, 150);
    m_plot->AddLayer(     xaxis );
    m_plot->AddLayer(     yaxis );
    m_plot->AddLayer(     new MySIN( 10.0, 220.0 ) );
    m_plot->AddLayer(     new MyCOSinverse( 10.0, 100.0 ) );
    m_plot->AddLayer( l = new MyLissajoux( 125.0 ) );
	m_plot->AddLayer(     vectorLayer );
    m_plot->AddLayer(     new mpText(wxT("mpText sample"), 10, 10) );
    wxBrush hatch(wxColour(200,200,200), wxSOLID);
    //m_plot->AddLayer( nfo = new mpInfoLayer(wxRect(80,20,40,40), &hatch));
    m_plot->AddLayer( nfo = new mpInfoCoords(wxRect(80,20,10,10), wxTRANSPARENT_BRUSH)); //&hatch));
    nfo->SetVisible(false);
    wxBrush hatch2(wxColour(163,208,212), wxSOLID);
    mpInfoLegend* leg;
    m_plot->AddLayer( leg = new mpInfoLegend(wxRect(200,20,40,40), wxTRANSPARENT_BRUSH)); //&hatch2));
    leg->SetVisible(true);
    
    // m_plot->EnableCoordTooltip(true);
    // set a nice pen for the lissajoux
    wxPen mypen(*wxRED, 5, wxSOLID);
    l->SetPen( mypen);

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
    axesPos[0] = 0;
    axesPos[1] = 0;
    ticks = true;
//	Refresh();
    m_plot->EnableDoubleBuffer(true);
    m_plot->SetMPScrollbars(true);
    m_plot->Fit();	

}