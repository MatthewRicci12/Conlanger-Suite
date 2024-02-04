#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "FontCreator.h"
#include <wx/dcbuffer.h>
#include <string>
#include <format>
#define POPUP(message) wxMessageDialog z(this, message); z.ShowModal()
enum
{
    ID_TOP_WINDOW = 1,
    ID_DRAWING_WINDOW = 2
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    //int width = 0.75 * wxGetDisplaySize()
    //int height = 0.75 * wxGetDisplaySize();
    MyFrame* frame = new MyFrame("", wxPoint(50, 50), wxSize(0.85 * wxGetDisplaySize())); //Make sure this is always even
    frame->Show(true);
    return true;
}

//PARENT
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    MyWindow* w = new MyWindow(this, ID_TOP_WINDOW);
}



MyWindow::MyWindow(wxWindow* parent, wxWindowID id, const wxSize& size, const wxPoint& pos, long style)
    : wxWindow(parent, id, pos, size, style)
{
    wxGridSizer* windowSizer = new wxGridSizer(1,1,1,1);
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
