#ifndef HEXFILEHEADER_H
#define HEXFILEHEADER_H

#include "hexfileview.h"

/*!
 * \brief The HexFileHeader class
 */
class HexFileHeader : public QWidget
{
    Q_OBJECT
public:
    explicit HexFileHeader(HexFileView* view, QWidget *parent = 0);

    virtual void paintEvent(QPaintEvent *);
    void setHighlight(int position);
    void focusIn();
    void focusOut();
    
signals:
    
public slots:

private:
    HexFileView* _view;
    int _height;
    int _hexWidth;
    int _charWidth;
    int _hlPosition;
    bool _hasFocus;
    
};

#endif // HEXFILEHEADER_H
