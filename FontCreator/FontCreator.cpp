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
    ID_DRAWING_WINDOW = 2,
    ID_CLEAR = 3,
    ID_SUBMIT = 4
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame("", wxPoint(50, 50), wxSize(0.85 * wxGetDisplaySize()));
    frame->Show(true);
    return true;
}

BEGIN_EVENT_TABLE(MyWindow, wxWindow)
EVT_BUTTON(ID_CLEAR, MyWindow::Clear)
EVT_BUTTON(ID_SUBMIT, MyWindow::Submit)
END_EVENT_TABLE()

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    MyWindow* w = new MyWindow(this, ID_TOP_WINDOW);
}

void MyWindow::Clear(wxCommandEvent& event) {
    canvas.ClearDrawing();
}

void MyWindow::Submit(wxCommandEvent& event) {
    POPUP("Submit");
}

BEGIN_EVENT_TABLE(MyPanel, wxPanel)
EVT_MOTION(MyPanel::OnMotion)
EVT_LEFT_DOWN(MyPanel::OnMouseLeftDown)
EVT_LEFT_UP(MyPanel::OnMouseLeftUp)
EVT_PAINT(MyPanel::OnPaint)
END_EVENT_TABLE()



void MyPanel::OnMotion(wxMouseEvent& event)
{
    if (HasCapture() && event.Dragging() && event.LeftIsDown())
        AddPoint(event.GetPosition());
}

void MyPanel::OnMouseLeftDown(wxMouseEvent& event)
{
    CaptureMouse();

    lines.push_back(Line());
    AddPoint(event.GetPosition());
}

void MyPanel::OnMouseLeftUp(wxMouseEvent&)
{
    if (HasCapture())
    {
        ReleaseMouse();
    }
}

void MyPanel::OnPaint(wxPaintEvent&)
{

    wxAutoBufferedPaintDC dc(this);

    dc.SetBackground(*wxBLACK_BRUSH);
    dc.Clear();

    // draw lines the user created
    wxPen linePen(wxPenInfo().Colour(*wxWHITE).Width(3));
    wxDCPenChanger lineUserPenChanger(dc, linePen);

    for (const auto& line : lines)
    {
        dc.DrawLines(line.size(), &line[0]);
    }
}
void MyPanel::AddPoint(const wxPoint& point)
{
    lines.back().push_back(point);

    Refresh();
    Update();
}

void MyPanel::ClearDrawing()
{
    lines.clear();

    Refresh();
    Update();

}


MyWindow::MyWindow(wxWindow* parent, wxWindowID id, const wxSize& size, const wxPoint& pos, long style)
    : wxWindow(parent, id, pos, size, style) , canvas(this, ID_DRAWING_WINDOW, wxSize(200, 200), wxBORDER_SIMPLE)
{
    wxBoxSizer* windowSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    windowSizer->AddStretchSpacer();

    windowSizer->Add(buttonSizer,
        wxSizerFlags(0).Center());

    buttonSizer->Add(new wxButton(this, ID_CLEAR, "Clear"),
        wxSizerFlags(0).Center());
    buttonSizer->Add(new wxButton(this, ID_SUBMIT, "Submit"),
        wxSizerFlags(0).Center());
    windowSizer->Add(&canvas, wxSizerFlags(0).Center());

    windowSizer->AddStretchSpacer();
    SetSizerAndFit(windowSizer);
}

MyPanel::MyPanel(wxWindow* parent, wxWindowID id, const wxSize& size, long style)
    : wxPanel(parent, id, wxDefaultPosition, size, style)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
}
