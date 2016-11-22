#ifndef MyPlotSegmentSEGMENT_H
#define MyPlotSegmentSEGMENT_H

#include <wx/sizer.h>
#include <wx/panel.h>


#include "mathplot.h"
#include <vector>

#define GTNUM	11
extern int lickRangeLow[];
extern int lickRangeUp[];

using namespace std;
class MyPlotSegment : public wxPanel
{
public:
	MyPlotSegment(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition,
		const wxSize &size = wxDefaultSize,	long style = wxTAB_TRAVERSAL);
		
	~MyPlotSegment();

	mpWindow * GetPlotWin() { return m_plot; }
	void setSignalSegment(vector<float> &fd, vector<float>& smoothFD, 
					vector<float>& desired, vector<float>& predict);
	void showProfileSegment(int start, int end);
	void SetInfoCoordsVisible(bool bVisible) { m_nfo->SetVisible(bVisible); }
	mpMovableObject*  GetLineObjPtr() { return m_pLine; }
//	wxBoxSizer *topsizer;
	mpWindow *      m_plot;	
	mpPolygon*		m_pLine;
	int 			m_szSignal;
private:	
    mpInfoCoords *	m_nfo; // mpInfoLayer* nfo;
//    int axesPos[2];
//    bool ticks;

	
public:
	virtual bool HasTransparentBackground();
	virtual bool IsCanvasWindow() const;
};



#endif // MyPlotSegment_H
