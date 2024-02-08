class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};
class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    void Clear(wxCommandEvent& event);
    void Submit(wxCommandEvent& event);
private:
    wxDECLARE_EVENT_TABLE();
};

class MyWindow : public wxWindow
{
public:
    MyWindow(wxWindow* parent, wxWindowID id = wxID_ANY, const wxSize& size = wxDefaultSize, const wxPoint& pos = wxDefaultPosition, long style = wxTAB_TRAVERSAL);
};

class MyPanel : public wxPanel
{
public:
    MyPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL);
    void OnMotion(wxMouseEvent& event);
    wxDECLARE_EVENT_TABLE();
};