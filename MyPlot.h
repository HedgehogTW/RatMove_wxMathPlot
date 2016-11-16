#ifndef MYPLOT_H
#define MYPLOT_H

#include <wx/sizer.h>
#include <wx/panel.h>


#include "mathplot.h"
#include <vector>

#define GTNUM	11
extern int lickRangeLow[];
extern int lickRangeUp[];

using namespace std;
class MyPlot : public wxPanel
{
public:
	MyPlot(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition,
		const wxSize &size = wxDefaultSize,	long style = wxTAB_TRAVERSAL);
		
	~MyPlot();

	mpWindow * GetPlotWin() { return m_plot; }
	void plotSignal(vector<float> &fd, vector<float>& smoothFD, 
					vector<float>& desired, vector<float>& predict);
//	wxBoxSizer *topsizer;
	mpWindow        *m_plot;	
private:	
//    int axesPos[2];
//    bool ticks;
    mpInfoCoords *nfo; // mpInfoLayer* nfo;
	
public:
	virtual bool HasTransparentBackground();
	virtual bool IsCanvasWindow() const;
};

class DesiredLabel : public mpFX //mpAX
{
 //   int m_x;
public:
    DesiredLabel() : mpFX( wxT("Desired Label")) {  }

    virtual float GetY( float /*int ?? */ x )
    {
		bool bLick = false;
		for(int i=0; i<GTNUM; i++) {
			if(x >=lickRangeLow[i] && x <= lickRangeUp[i]) {				
				bLick = true;
				break;
			}
		}
		
        if (bLick)
            return 1000;
        else
            return 500;
    }
};


// MyLissajoux

class MyLissajoux : public mpFXY
{
    float m_rad;
    int    m_idx;
public:
    MyLissajoux(float rad) : mpFXY( wxT("Lissajoux")) { m_rad=rad; m_idx=0; m_drawOutsideMargins = false;}
    virtual bool GetNextXY( float & x, float & y )
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
    virtual float GetMinX() { return -m_rad; }
    virtual float GetMaxX() { return  m_rad; }
    virtual float GetMinY() { return -m_rad; }
    virtual float GetMaxY() { return  m_rad; }
};

#endif // MYPLOT_H
