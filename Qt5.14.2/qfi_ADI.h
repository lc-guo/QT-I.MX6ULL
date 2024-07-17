#ifndef QFI_ADI_H
#define QFI_ADI_H

////////////////////////////////////////////////////////////////////////////////

#include <QGraphicsView>
#include <QGraphicsSvgItem>

////////////////////////////////////////////////////////////////////////////////

/** Attitude Director Indicator widget. */
class qfi_ADI : public QGraphicsView
{
    Q_OBJECT

public:

    /** Constructor. */
    qfi_ADI( QWidget *parent = nullptr );

    /** Destructor. */
    virtual ~qfi_ADI();

    /** Reinitiates widget. */
    void reinit();

    /** Refreshes (redraws) widget. */
    void update();

    /** @param roll angle [deg] */
    void setRoll( float roll );

    /** @param pitch angle [deg] */
    void setPitch( float pitch );

protected:

    void resizeEvent( QResizeEvent *event );

private:

    QGraphicsScene *m_scene;

    QGraphicsSvgItem *m_itemBack;
    QGraphicsSvgItem *m_itemFace;
    QGraphicsSvgItem *m_itemRing;
    QGraphicsSvgItem *m_itemCase;

    float m_roll;
    float m_pitch;

    float m_faceDeltaX_new;
    float m_faceDeltaX_old;
    float m_faceDeltaY_new;
    float m_faceDeltaY_old;

    float m_scaleX;
    float m_scaleY;

    const int m_originalHeight;
    const int m_originalWidth;

    const float m_originalPixPerDeg;

    QPointF m_originalAdiCtr;

    const int m_backZ;
    const int m_faceZ;
    const int m_ringZ;
    const int m_caseZ;

    void init();

    void reset();

    void updateView();
};

////////////////////////////////////////////////////////////////////////////////

#endif // QFI_ADI_H
