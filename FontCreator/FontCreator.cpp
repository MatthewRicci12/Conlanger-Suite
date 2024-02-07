#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "FontCreator.h"
#include <wx/dcbuffer.h>
#include <string>


#define POPUP(message) wxMessageDialog z(this, message); z.ShowModal()
enum
{
    ID_TOP_WINDOW = 1,
    ID_DRAWING_WINDOW = 2
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame("", wxPoint(50, 50), wxSize(0.85 * wxGetDisplaySize()));
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    MyWindow* w = new MyWindow(this, ID_TOP_WINDOW);
}

BEGIN_EVENT_TABLE(MyPanel, wxPanel)
EVT_MOTION(MyPanel::OnMotion)
END_EVENT_TABLE()

void MyPanel::OnMotion(wxMouseEvent& event)
{
    if (event.Dragging())
    {
        wxClientDC dc(this);
        wxBufferedDC bdc(&dc);
        wxPen pen(*wxRED, 1); // red pen of width 1
        bdc.SetPen(pen);
        bdc.DrawPoint(event.GetPosition());
        bdc.SetPen(wxNullPen);
    }
}

MyWindow::MyWindow(wxWindow* parent, wxWindowID id, const wxSize& size, const wxPoint& pos, long style)
    : wxWindow(parent, id, pos, size, style)
{
    wxGridSizer* windowSizer = new wxGridSizer(1, 1, 1, 1);
    windowSizer->Add(new MyPanel(this, ID_DRAWING_WINDOW, wxSize(200, 200), wxBORDER_SIMPLE),
        wxSizerFlags(1).Center()
    );
    SetSizerAndFit(windowSizer);
}

MyPanel::MyPanel(wxWindow* parent, wxWindowID id, const wxSize& size, long style)
    : wxPanel(parent, id, wxDefaultPosition, size, style)
{
    SetBackgroundColour(*wxGREEN);
}
