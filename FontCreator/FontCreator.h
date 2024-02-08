class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyWindow : public wxWindow
{
public:
    MyWindow(wxWindow* parent, wxWindowID id = wxID_ANY, const wxSize& size = wxDefaultSize, const wxPoint& pos = wxDefaultPosition, long style = wxTAB_TRAVERSAL);
};

class MyPanel : public wxPanel
{
friend void MyFrame::Clear(wxCommandEvent& event);

public:
    MyPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL);
    wxDECLARE_EVENT_TABLE();
private:
    typedef std::vector<wxPoint> Line;
    typedef std::vector<Line> Lines;
    Lines lines;

    void OnMouseLeftDown(wxMouseEvent& evt);
    void OnMouseLeftUp(wxMouseEvent&);
    void OnMotion(wxMouseEvent& evt);
    void OnPaint(wxPaintEvent&);
    void AddPoint(const wxPoint& point);
    void ClearDrawing();
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