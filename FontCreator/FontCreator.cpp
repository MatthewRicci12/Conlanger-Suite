
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

#ifndef LOG_MSG
#define LOG_MSG(message, ...) sprintf(s, message, __VA_ARGS__); BOOST_LOG_SEV(lg, info) << s;
#endif

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
using namespace logging::trivial; 
//src::severity_logger<severity_level> lg;
//char s[256];

void init()
{
    logging::add_file_log("debug.log");

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
    ID_DIALOG_TEXT = 6,
    ID_SAVE_FONT_FILE = 7,
    ID_LOAD_FONT_FILE = 8,
    ID_TRY_FONT = 9,
    ID_TYPING_WINDOW = 10,
    ID_TYPING_WINDOW_BACK = 11,
    ID_TYPING_WINDOW_CLEAR = 12
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

int MyApp::OnExit()
{
    FontFileSerializer::cleanUpInstance(); //Bug if never made?
    return 0;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    //SetBackgroundColour(*wxWHITE);
    CreateCanvasWindow();
}

void MyFrame::CreateCanvasWindow() {
    MyWindow* w = new MyWindow(this, ID_TOP_WINDOW);

    Refresh();
}

void MyFrame::CreateTypingWindow() {
    MyWindow* window = dynamic_cast<MyWindow*>(FindWindow(ID_TOP_WINDOW));
    decltype(auto) charMappingRef = window->GetMap();
    TypingWindow* tw = new TypingWindow(this, charMappingRef, ID_TYPING_WINDOW, GetSize(), wxDefaultPosition);
    
    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    tw->SetBackgroundColour(*wxBLACK);
    topSizer->Add(tw, 1, wxEXPAND);
    SetSizer(topSizer);
    Refresh();
}

void MyFrame::ShowCanvasWindow() {
    TypingWindow* tw = dynamic_cast<TypingWindow*>(FindWindow(ID_TYPING_WINDOW));
    tw->Destroy();

    MyWindow* topWindow = dynamic_cast<MyWindow*>(FindWindow(ID_TOP_WINDOW));
    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    topSizer->Add(topWindow, 1, wxEXPAND);
    SetSizer(topSizer); 

    topWindow->Show();
    Refresh();
}

BEGIN_EVENT_TABLE(MyWindow, wxWindow)
EVT_BUTTON(ID_CLEAR, MyWindow::Clear)
EVT_BUTTON(ID_SUBMIT, MyWindow::Submit)
EVT_BUTTON(ID_SAVE_FONT_FILE, MyWindow::SaveFontFile)
EVT_BUTTON(ID_LOAD_FONT_FILE, MyWindow::LoadFontFile)
EVT_BUTTON(ID_TRY_FONT, MyWindow::TryFont)
END_EVENT_TABLE()



void MyWindow::Clear(wxCommandEvent& event) 
{
    canvas.ClearDrawing();
}

void MyWindow::Submit(wxCommandEvent& event) 
{

    KeyDialog dialog(this, ID_DIALOG, "Input", wxDefaultPosition, wxSize(300, 150));
 
    dialog.ShowModal();
    FontFileSerializer* instance = FontFileSerializer::getInstance();

    charMapping[dialog.curKeyCode] = instance->downSizeLines(canvas.lines);
}

void MyWindow::SaveFontFile(wxCommandEvent& event) {

    FileNameDialog dialog(this, "What would you like to name the font file?");
    dialog.ShowModal();

    std::string fileName = dialog.GetValue().ToStdString();
    FontFileSerializer* instance = FontFileSerializer::getInstance();
    instance->saveFontFile(fileName, charMapping);
}

void MyWindow::LoadFontFile(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, _("Open font file"), "", "",
        "Text files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    openFileDialog.SetDirectory(MyApp::projectRoot + R"(\FontFiles\)");
    if (openFileDialog.ShowModal() == wxID_CANCEL) return;

    std::string fileName = openFileDialog.GetPath().ToStdString();

    FontFileSerializer* instance = FontFileSerializer::getInstance();
    instance->loadFontFile(fileName, charMapping);
}

void MyWindow::TryFont(wxCommandEvent& event) {
    Hide();
    MyFrame* parent = dynamic_cast<MyFrame*>(GetParent());
    parent->CreateTypingWindow();
}

std::unordered_map<char, Lines>& MyWindow::GetMap() const {
    return const_cast<std::unordered_map<char, Lines>&>(charMapping);
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
    if (point.x < 0 || point.x > 200 || point.y < 0 || point.y > 200) {
        return;
    }

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
    : wxWindow(parent, id, pos, size, style), canvas(this, ID_DRAWING_WINDOW, wxSize(CANVAS_D, CANVAS_D), wxBORDER_SIMPLE),
    charMapping(26)
{
    wxBoxSizer* windowSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* buttonSizerTop = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* buttonSizerBottom = new wxBoxSizer(wxHORIZONTAL);

    windowSizer->AddStretchSpacer();

    windowSizer->Add(buttonSizerTop,
        wxSizerFlags(0).Center());

    buttonSizerTop->Add(new wxButton(this, ID_CLEAR, "Clear"),
        wxSizerFlags(0).Center());
    buttonSizerTop->Add(new wxButton(this, ID_SUBMIT, "Submit"),
        wxSizerFlags(0).Center());

    windowSizer->Add(&canvas, wxSizerFlags(0).Center());

    windowSizer->Add(buttonSizerBottom,
        wxSizerFlags(0).Center());

    buttonSizerBottom->Add(new wxButton(this, ID_LOAD_FONT_FILE, "Load Font File"),
        wxSizerFlags(0).Center());
    buttonSizerBottom->Add(new wxButton(this, ID_SAVE_FONT_FILE, "Save Font File"),
        wxSizerFlags(0).Center());
    windowSizer->AddStretchSpacer();

    windowSizer->Add(new wxButton(this, ID_TRY_FONT, "Try out font"),
        wxSizerFlags(0).Center());

    windowSizer->AddStretchSpacer();
    SetSizerAndFit(windowSizer);
}

MyPanel::MyPanel(wxWindow* parent, wxWindowID id, const wxSize& size, long style)
    : wxPanel(parent, id, wxDefaultPosition, size, style)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
}


BEGIN_EVENT_TABLE(KeyDialog, wxDialog)
EVT_KEY_UP(KeyDialog::KeyPressed)
END_EVENT_TABLE()

KeyDialog::KeyDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos,
    const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
    SetFocus();
    wxStaticText* text = new wxStaticText(this, ID_DIALOG_TEXT, "Enter a key to map this glyph to.");

}

void KeyDialog::KeyPressed(wxKeyEvent& event)
{
    curKeyCode = event.GetKeyCode();
    EndModal(0);
}


FileNameDialog::FileNameDialog(wxWindow* parent, const wxString& message, const wxString& caption,
    const wxString& value, long style, const wxPoint& pos) 
    : wxTextEntryDialog(parent, message, caption, value, style, pos)
{
    SetFocus();
}

BEGIN_EVENT_TABLE(TypingWindow, wxPanel)
EVT_BUTTON(ID_TYPING_WINDOW_BACK, TypingWindow::Back)
EVT_BUTTON(ID_TYPING_WINDOW_CLEAR, TypingWindow::Clear)
EVT_KEY_UP(TypingWindow::KeyPressed)
END_EVENT_TABLE()


TypingWindow::TypingWindow(wxWindow* parent, const std::unordered_map<char, Lines>& charMappingRef, wxWindowID id,
    const wxSize& size, const wxPoint& pos, long style)
    : wxPanel(parent, id, pos, size, style), xOffset(0), yOffset(50), charMapping(charMappingRef)
{
    SetFocusIgnoringChildren();
    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    topSizer->SetMinSize(size);

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->SetMinSize(wxSize(0, size.GetWidth()));


    wxButton* backButton = new wxButton(this, ID_TYPING_WINDOW_BACK, "Back", wxDefaultPosition, wxDefaultSize);
    wxButton* clearButton = new wxButton(this, ID_TYPING_WINDOW_CLEAR, "Clear", wxDefaultPosition, wxDefaultSize);
    backButton->SetCanFocus(false);
    clearButton->SetCanFocus(false);
    backButton->SetForegroundColour(*wxBLACK);
    clearButton->SetForegroundColour(*wxBLACK);

    buttonSizer->Add(backButton, wxSizerFlags(0));
    buttonSizer->Add(clearButton, wxSizerFlags(0));
  
    topSizer->Add(buttonSizer, wxSizerFlags(0).Center());

    SetSizerAndFit(topSizer);
}

void TypingWindow::Back(wxCommandEvent& event) {
    MyFrame* parent = dynamic_cast<MyFrame*>(GetParent());
    parent->ShowCanvasWindow();
}

void TypingWindow::Clear(wxCommandEvent& event) {
    wxClientDC cdc(this);
    wxBufferedDC dc(&cdc);
    dc.Clear();
}


void TypingWindow::KeyPressed(wxKeyEvent& event) {
    char curKeyCode = event.GetKeyCode();

    if (charMapping.find(curKeyCode) != charMapping.end()) {
        wxClientDC cdc(this);
        wxBufferedDC dc(&cdc);

        auto& lines = charMapping.at(curKeyCode);

        dc.SetBackground(*wxBLACK_BRUSH);

        wxPen linePen(wxPenInfo().Colour(*wxWHITE).Width(3));
        wxDCPenChanger lineUserPenChanger(dc, linePen);

        for (const auto& line : lines)
        {
            dc.DrawLines(line.size(), &line[0], xOffset, yOffset);
        }
        xOffset += 50;

        if (xOffset+50 > GetClientSize().GetWidth()) {
            xOffset = 0;
            yOffset += 50;
        }

    }
}

std::string MyApp::projectRoot = MyApp::getProjectRoot();
std::string MyApp::getProjectRoot() {
    TCHAR buffer[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
    std::wstring curDirWide = std::wstring(buffer).substr(0, pos);
    std::string curDir = std::string(curDirWide.begin(), curDirWide.end()); //Gotta be in its own func

    auto posOfProjectSuffix = curDir.rfind(PROJECT_SUFFIX);

    return curDir.substr(0, posOfProjectSuffix);
}