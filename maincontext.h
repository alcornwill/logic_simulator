#ifndef MAINCONTEXT_H
#define MAINCONTEXT_H

class MyGraphicsScene;
class MainWindow;
class MyResources;

// inherit this to provide access to frequently used objects
class MainContext
{
public:
    MainContext();
    static MainWindow *mywindow;
    static MyGraphicsScene *myscene;
    static MyResources *myresources;
};

#endif // MAINCONTEXT_H
