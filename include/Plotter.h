#ifndef PLOTTER_H
#define PLOTTER_H

#include <queue>
#include "global.h"
#include "fftw/fftw3.h"
#include <FL/Fl.H>
#include <FL/Fl_Overlay_Window.H>
#include <FL/Fl_Button.H>
#include <FL/fl_draw.H>
#include "cartesian.h"

#include "jssynchornize.h"

using std::queue;

struct DataPacket
{
    Sample *data;
    int size;
};

class Ca_Line_Safe:public Ca_Line
{
private:
    double*m_base;
public:
    Ca_Line_Safe(int _n, double *_data, double *_data_2, int _line_style, int _line_width=0, Fl_Color color=FL_BLACK, int style=CA_SIMPLE, int size=DEFAULT_POINT_SIZE, Fl_Color border_color=FL_BLACK, int border_width=0)
    :Ca_Line(_n,_data,_data_2,_line_style,_line_width,color,style,size,border_color,border_width)
    {
        m_base=new double[2*n];
        memcpy(m_base,data,n);
        memcpy(m_base+n,data_2,n);
        data=m_base;
        data_2=m_base+n;
    }

    ~Ca_Line_Safe()
    {
        delete []m_base;
    }

};

class Plotter
{
    friend void PlotterThread(void*);
    friend void APIENTRY APCWrapper(ULONG_PTR);
    public:
        Plotter();
        void SetBGColor(float R,float G,float B);
        void Plot(Real*buf,int size);
        void SafePlot(Real*buf,int size);
        void PlotSpectrum(Real*buf,int size);
        void SetXText(const char*label){Fl::lock();m_x->copy_label(label);Fl::unlock();}
        void SetXMin(double num){Fl::lock();m_x->minimum(num);Fl::unlock();}
        void SetXMax(double num){Fl::lock();m_x->maximum(num);Fl::unlock();}
        void SetYText(const char*label){Fl::lock();m_y->copy_label(label);Fl::unlock();}
        void SetYMin(double num){Fl::lock();m_y->minimum(num);Fl::unlock();}
        void SetYMax(double num){Fl::lock();m_y->maximum(num);Fl::unlock();}

        void SetTitle(const char*title){m_window->copy_label(title);}
        virtual ~Plotter();
    protected:
    private:
        JSEvent m_alerter;
        static int m_instance;
        static HANDLE m_thread;
        Fl_Double_Window*m_window;
        Ca_Canvas*m_canvas;
        Ca_X_Axis*m_x;
        Ca_Y_Axis*m_y;
        Fl_Group*m_group;
        queue<DataPacket> m_pending;
};

#endif // PLOTTER_H