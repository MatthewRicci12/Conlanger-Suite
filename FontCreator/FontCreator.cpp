
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "FontCreator.h"
#include "FontFileSerializer.h"
#include <wx/dcbuffer.h>
#include <string>

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#define POPUP(message) wxMessageDialog z(this, message); z.ShowModal()
#define LOG_MSG(message) using namespace logging::trivial; src::severity_logger<severity_level> lg; char s[128]; sprintf(s, message); BOOST_LOG_SEV(lg, info) << s;

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

void init()
{
    logging::add_file_log("sample.log");

    logging::core::get()->set_filter
    (
        logging::trivial::severity >= logging::trivial::info
    );
}





enum
{
    ID_TOP_WINDOW = 1,
    ID_DRAWING_WINDOW = 2,
    ID_CLEAR = 3,
    ID_SUBMIT = 4,
    ID_DIALOG = 5,
    ID_DIALOG_TEXT = 6
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    init();
    logging::add_common_attributes();

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
    MyDialog* d = new MyDialog(this, ID_DIALOG, "Input", wxDefaultPosition, wxSize(300, 150));
    //set Icon
    d->ShowModal();

    wxWindowDC wdc(d);
    wxMemoryDC dc(&wdc);
    wxSize bitmapSize = dc.GetAsBitmap().GetSize();
    int height = bitmapSize.GetHeight();
    int width = bitmapSize.GetWidth();

    LOG_MSG("Height: %d, width: %d\n", height, width);

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
    wxMemoryDC dc(bm);

    wxPen linePen(wxPenInfo().Colour(*wxWHITE).Width(5));
    wxDCPenChanger lineUserPenChanger(dc, linePen);

    for (const auto& line : lines)
    {
        dc.DrawLines(line.size(), &line[0]);
    }
    dc.SelectObject(wxNullBitmap);

    
    wxAutoBufferedPaintDC pdc(this);
    pdc.DrawBitmap(bm, wxPoint(0, 0));
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
    : wxWindow(parent, id, pos, size, style), canvas(this, ID_DRAWING_WINDOW, wxSize(CANVAS_D, CANVAS_D), wxBORDER_SIMPLE)
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
    : wxPanel(parent, id, wxDefaultPosition, size, style), bm(CANVAS_D, CANVAS_D)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
}


BEGIN_EVENT_TABLE(MyDialog, wxDialog)
EVT_KEY_UP(MyDialog::KeyPressed)
EVT_PAINT(MyDialog::OnPaint)
END_EVENT_TABLE()

MyDialog::MyDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos,
    const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
}


void MyDialog::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    dc.SetPen(*wxRED_PEN);
    //dc.SetBrush(*wxRED_BRUSH);
    dc.SetBackground(*wxBLUE);
    dc.Clear();

}

void MyDialog::KeyPressed(wxKeyEvent& event)
{
    int keyCode = event.GetKeyCode();
    wxLogMessage("You pressed: %c", keyCode);
    EndModal(0);
}