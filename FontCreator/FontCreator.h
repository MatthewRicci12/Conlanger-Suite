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

private:
    typedef std::vector<wxPoint> Line;
    typedef std::vector<Line> Lines;
    Lines lines;

    wxDECLARE_EVENT_TABLE();
};


class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyWindow : public wxWindow
{
public:
    MyWindow(wxWindow* parent, wxWindowID id = wxID_ANY, const wxSize& size = wxDefaultSize, const wxPoint& pos = wxDefaultPosition, long style = wxTAB_TRAVERSAL);
    void Clear(wxCommandEvent& event);
    void Submit(wxCommandEvent& event);
private:
    MyPanel canvas;
    wxDECLARE_EVENT_TABLE();
};


class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
};