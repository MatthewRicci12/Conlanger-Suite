#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <unordered_map>

#ifndef FONTCREATOR_H
#define FONTCREATOR_H

#define PROJECT_SUFFIX R"(\Debug)"

class MyWindow;
class KeyDialog;

typedef std::vector<wxPoint> Line;
typedef std::vector<Line> Lines;

class MyPanel : public wxPanel
{
public:
    MyPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL);
    void OnMouseLeftDown(wxMouseEvent& evt);
    void OnMouseLeftUp(wxMouseEvent&);
    void OnMotion(wxMouseEvent& evt);
    void OnPaint(wxPaintEvent&);
    void AddPoint(const wxPoint& point);
    void ClearDrawing();
    friend MyWindow;
private:
    Lines lines;

    wxDECLARE_EVENT_TABLE();
};

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
    virtual int OnExit();
    static std::string projectRoot;
private:
    static std::string getProjectRoot();
};

class KeyDialog : public wxDialog
{
public:
    KeyDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);
    void KeyPressed(wxKeyEvent& event);
    friend MyWindow;
private:
    int curKeyCode;
    wxDECLARE_EVENT_TABLE();
};

class FileNameDialog : public wxTextEntryDialog
{
public:
    FileNameDialog(wxWindow* parent, const wxString& message, const wxString& caption = wxGetTextFromUserPromptStr,
        const wxString& value = wxEmptyString, long style = wxTextEntryDialogStyle, const wxPoint& pos = wxDefaultPosition);
    friend MyWindow;
};

class MyWindow : public wxWindow
{
public:
    MyWindow(wxWindow* parent, wxWindowID id = wxID_ANY, const wxSize& size = wxDefaultSize, const wxPoint& pos = wxDefaultPosition, long style = wxTAB_TRAVERSAL);
    void Clear(wxCommandEvent& event);
    void Submit(wxCommandEvent& event);
    void SaveFontFile(wxCommandEvent& event);
    void LoadFontFile(wxCommandEvent& event);
    void TryFont(wxCommandEvent& event);
    void ShowMappings(wxCommandEvent& event);
    void ClearMappings(wxCommandEvent& event);
    std::unordered_map<char, Lines>& GetMap() const;
private:
    MyPanel canvas;
    std::unordered_map<char, Lines> charMapping;

    wxDECLARE_EVENT_TABLE();
};

class TypingWindow : public wxPanel
{
public:
    TypingWindow(wxWindow* parent, const std::unordered_map<char, Lines>& charMappingRef, wxWindowID id = wxID_ANY,
        const wxSize& size = wxDefaultSize, const wxPoint& pos = wxDefaultPosition, long style = wxTAB_TRAVERSAL);
private:
    int xOffset;
    int yOffset;
    void KeyPressed(wxKeyEvent& event);
    void OnClick(wxMouseEvent& event);
    void Back(wxCommandEvent& event);
    void Clear(wxCommandEvent& event);
    const std::unordered_map<char, Lines>& charMapping;
    wxDECLARE_EVENT_TABLE();
};

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    friend MyWindow;
    friend TypingWindow;
private:
    void CreateCanvasWindow();
    void CreateTypingWindow();
    void ShowCanvasWindow();
};

class ShowMappingDialog : public wxDialog
{
public:
    ShowMappingDialog(wxWindow* parent, const std::unordered_map<char, Lines>& charMappingRef, wxWindowID id, 
        const wxString& title, const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE, 
        const wxString& name = wxDialogNameStr);
private:
    void OnPaint(wxPaintEvent& event);
    const std::unordered_map<char, Lines>& charMapping;
    int xOffset;
    int yOffset;
    wxDECLARE_EVENT_TABLE();
};


#endif