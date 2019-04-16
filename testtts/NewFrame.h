#ifndef NEWFRAME_H
#define NEWFRAME_H

//(*Headers(NewFrame)
#include <mathplot.h>
#include <wx/frame.h>
//*)

class NewFrame: public wxFrame
{
	public:

		NewFrame(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~NewFrame();

		//(*Declarations(NewFrame)
		mpMarker   *Marker1;
		mpScaleX   *XAxis;
		mpScaleY   *YAxis;
		mpWindow* MathPlot1;
		//*)

	protected:

		//(*Identifiers(NewFrame)
		static const long ID_MARKER1;
		static const long ID_MATHPLOT1;
		//*)

	private:

		//(*Handlers(NewFrame)
		void OnMathPlot1Paint(wxPaintEvent& event);
		void OnClose(wxCloseEvent& event);
		void OnMathPlot1Resize(wxSizeEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
