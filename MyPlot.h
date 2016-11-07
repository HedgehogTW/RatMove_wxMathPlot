#ifndef MYPLOT_H
#define MYPLOT_H

#include <wx/sizer.h>
#include <wx/panel.h>


#include "mathplot.h"
#include <vector>

using namespace std;
class MyPlot : public wxPanel
{
public:
	MyPlot(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition,
		const wxSize &size = wxDefaultSize,	long style = wxTAB_TRAVERSAL);
		
	~MyPlot();

	void Init();
	void plotSignal(vector<double> &fd, vector<double>& smoothFD);
//	wxBoxSizer *topsizer;
	mpWindow        *m_plot;	
private:	
    int axesPos[2];
    bool ticks;
    mpInfoCoords *nfo; // mpInfoLayer* nfo;
	
public:
	virtual bool HasTransparentBackground();
	virtual bool IsCanvasWindow() const;
};

class MySIN : public mpFX
{
    double m_freq, m_amp;
public:
    MySIN(double freq, double amp) : mpFX( wxT("f(x) = SIN(x)"), mpALIGN_LEFT) { m_freq=freq; m_amp=amp; m_drawOutsideMargins = false; }
    virtual double GetY( double x ) { return m_amp * sin(x/6.283185/m_freq); }
    virtual double GetMinY() { return -m_amp; }
    virtual double GetMaxY() { return  m_amp; }
};

// MyCOSinverse

class MyCOSinverse : public mpFY
{
    double m_freq, m_amp;
public:
    MyCOSinverse(double freq, double amp) : mpFY( wxT("g(y) = COS(y)"), mpALIGN_BOTTOM) { m_freq=freq; m_amp=amp; m_drawOutsideMargins = false;}
    virtual double GetX( double y ) { return m_amp * cos(y/6.283185/m_freq); }
    virtual double GetMinX() { return -m_amp; }
    virtual double GetMaxX() { return  m_amp; }
};

// MyLissajoux

class MyLissajoux : public mpFXY
{
    double m_rad;
    int    m_idx;
public:
    MyLissajoux(double rad) : mpFXY( wxT("Lissajoux")) { m_rad=rad; m_idx=0; m_drawOutsideMargins = false;}
    virtual bool GetNextXY( double & x, double & y )
    {
        if (m_idx < 360)
        {
            x = m_rad * cos(m_idx / 6.283185*360);
            y = m_rad * sin(m_idx / 6.283185*360*3);
            m_idx++;
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    virtual void Rewind() { m_idx=0; }
    virtual double GetMinX() { return -m_rad; }
    virtual double GetMaxX() { return  m_rad; }
    virtual double GetMinY() { return -m_rad; }
    virtual double GetMaxY() { return  m_rad; }
};

#endif // MYPLOT_H
