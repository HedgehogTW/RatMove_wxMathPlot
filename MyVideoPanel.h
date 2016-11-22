#ifndef MYVIDEOPANEL_H
#define MYVIDEOPANEL_H
#include <wx/sizer.h>
#include <wx/panel.h>

class MyVideoPanel : public wxPanel
{
public:
	MyVideoPanel(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition,
		const wxSize &size = wxDefaultSize,	long style = wxTAB_TRAVERSAL);
	~MyVideoPanel();


	
};

#endif // MYVIDEOPANEL_H
