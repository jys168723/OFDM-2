#ifndef PLOTTER_H
#define PLOTTER_H

#include <queue>
#include "global.h"
#include <fftw/fftw3.h>
#include <FL/Fl.H>
#include <FL/Fl_Overlay_Window.H>
#include <FL/Fl_Button.H>
#include <FL/fl_draw.H>
#include "Cartesian.H"
#include <thread>
#include <mutex>
#include <condition_variable>

using std::queue;

struct DataPacket
{
    Sample *data;
    int size;
};


class Plotter
{
    friend void PlotterThread(void*);
    friend void APCWrapper(void*);
    public:
        Plotter(double xmin=0,double xmax=100,double ymin=0,double ymax=100);

        bool InvokeRequired(){return std::this_thread::get_id()!=m_tid;}

        void SetBGColor(float R,float G,float B);
        void Plot(Real*buf,int size);
        void SafePlot(Real*buf,int size);
        void Plot2D(Real*data1,Real*data2,int size);

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
        static std::mutex m_mu;
        static std::unique_lock<std::mutex> m_lock;
        static std::condition_variable m_cond;
        static int m_instance;
        static std::thread* m_thread;
        static std::thread::id m_tid;
        double m_xmin,m_xmax,m_ymin,m_ymax;
        Fl_Window*m_window;
        Ca_Canvas*m_canvas;
        Ca_X_Axis*m_x;
        Ca_Y_Axis*m_y;
        Fl_Group*m_group;
        queue<DataPacket> m_pending;
};

#define GUARD(func,...) {\
    if(InvokeRequired())\
    {\
        Invoke(WRAPCALL(&func,__VA_ARGS__));\
        return;\
    }}


#endif // PLOTTER_H
